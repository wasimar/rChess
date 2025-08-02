#include "core/Game.h"
#include "utils/Utils.h"
#include <algorithm>

Game::Game() 
    : current_player_(PieceColor::WHITE), 
      game_status_(GameStatus::ONGOING),
      halfmove_clock_(0),
      fullmove_number_(1),
      draw_offered_(false) {
    startNewGame();
}

void Game::startNewGame() {
    board_.initializeBoard();
    current_player_ = PieceColor::WHITE;
    game_status_ = GameStatus::ONGOING;
    move_history_.clear();
    halfmove_clock_ = 0;
    fullmove_number_ = 1;
    draw_offered_ = false;
    updateGameStatus();
}

bool Game::makeMove(const Position& from, const Position& to) {
    Move move = createMove(from, to);
    return makeMove(move);
}

bool Game::makeMove(const Move& move) {
    if (game_status_ != GameStatus::ONGOING && game_status_ != GameStatus::CHECK) {
        return false;
    }
    
    const Piece* piece = board_.getPiece(move.getFrom());
    if (!piece || piece->getColor() != current_player_) {
        return false;
    }
    
    if (!isValidMove(move.getFrom(), move.getTo())) {
        return false;
    }
    
    // Create a new move for execution
    Move executedMove(move.getFrom(), move.getTo(), move.getType());
    if (move.isPromotion()) {
        executedMove = Move(move.getFrom(), move.getTo(), move.getPromotionPiece());
    }
    
    // Handle captures
    if (!board_.isSquareEmpty(move.getTo())) {
        auto capturedPiece = board_.removePiece(move.getTo());
        executedMove.setCapturedPiece(std::move(capturedPiece));
    }
    
    // Handle special moves
    if (move.getType() == MoveType::CASTLING_KINGSIDE || move.getType() == MoveType::CASTLING_QUEENSIDE) {
        if (!executeCastling(executedMove)) return false;
    } else if (move.getType() == MoveType::EN_PASSANT) {
        if (!executeEnPassant(executedMove)) return false;
    } else if (move.getType() == MoveType::PAWN_PROMOTION) {
        if (!executePromotion(executedMove)) return false;
    } else {
        // Normal move
        board_.movePiece(move.getFrom(), move.getTo());
    }
    
    // Update move counters
    if (piece->getType() == PieceType::PAWN || executedMove.isCapture()) {
        halfmove_clock_ = 0;
    } else {
        halfmove_clock_++;
    }
    
    if (current_player_ == PieceColor::BLACK) {
        fullmove_number_++;
    }
    
    // Set en passant target for pawn double moves
    board_.clearEnPassantTarget();
    if (piece->getType() == PieceType::PAWN && Utils::abs(move.getTo().row - move.getFrom().row) == 2) {
        int enPassantRow = (move.getFrom().row + move.getTo().row) / 2;
        board_.setEnPassantTarget(Position(enPassantRow, move.getFrom().col));
    }
    
    move_history_.push_back(std::move(executedMove));
    switchPlayer();
    updateGameStatus();
    draw_offered_ = false;
    
    return true;
}

void Game::undoLastMove() {
    if (move_history_.empty()) return;
    
    const Move& lastMove = move_history_.back();
    
    // Store move info before removing from history
    Position from = lastMove.getFrom();
    Position to = lastMove.getTo();
    MoveType type = lastMove.getType();
    
    move_history_.pop_back();
    
    // Move piece back
    board_.movePiece(to, from);
    
    // Handle special move undos
    if (type == MoveType::CASTLING_KINGSIDE || type == MoveType::CASTLING_QUEENSIDE) {
        int row = from.row;
        if (type == MoveType::CASTLING_KINGSIDE) {
            board_.movePiece(Position(row, 5), Position(row, 7));
        } else {
            board_.movePiece(Position(row, 3), Position(row, 0));
        }
    }
    
    switchPlayer();
    updateGameStatus();
}

const Move& Game::getLastMove() const {
    static Move emptyMove(Position(-1, -1), Position(-1, -1));
    if (move_history_.empty()) {
        return emptyMove;
    }
    return move_history_.back();
}

bool Game::isValidMove(const Position& from, const Position& to) const {
    const Piece* piece = board_.getPiece(from);
    if (!piece || piece->getColor() != current_player_) {
        return false;
    }
    
    if (!piece->isValidMove(from, to, board_)) {
        return false;
    }
    
    return !board_.wouldBeInCheck(from, to, current_player_);
}

std::vector<Position> Game::getValidMoves(const Position& from) const {
    const Piece* piece = board_.getPiece(from);
    if (!piece || piece->getColor() != current_player_) {
        return {};
    }
    
    auto possibleMoves = piece->getPossibleMoves(from, board_);
    std::vector<Position> validMoves;
    
    for (const auto& move : possibleMoves) {
        if (!board_.wouldBeInCheck(from, move, current_player_)) {
            validMoves.push_back(move);
        }
    }
    
    return validMoves;
}

bool Game::isGameOver() const {
    return game_status_ == GameStatus::CHECKMATE || 
           game_status_ == GameStatus::STALEMATE || 
           game_status_ == GameStatus::DRAW;
}

std::string Game::getGameStatusString() const {
    switch (game_status_) {
        case GameStatus::ONGOING:
            return Utils::colorToString(current_player_) + " to move";
        case GameStatus::CHECK:
            return Utils::colorToString(current_player_) + " is in check";
        case GameStatus::CHECKMATE:
            return Utils::colorToString(current_player_) + " is checkmated. " + 
                   Utils::colorToString(current_player_ == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE) + " wins!";
        case GameStatus::STALEMATE:
            return "Stalemate - Draw";
        case GameStatus::DRAW:
            return "Draw";
        default:
            return "Unknown game status";
    }
}

void Game::setPlayer(PieceColor color, std::unique_ptr<Player> player) {
    if (color == PieceColor::WHITE) {
        white_player_ = std::move(player);
    } else {
        black_player_ = std::move(player);
    }
}

Player* Game::getPlayer(PieceColor color) const {
    return (color == PieceColor::WHITE) ? white_player_.get() : black_player_.get();
}

bool Game::offerDraw() {
    draw_offered_ = true;
    return true;
}

bool Game::acceptDraw() {
    if (draw_offered_) {
        game_status_ = GameStatus::DRAW;
        return true;
    }
    return false;
}

void Game::resignGame(PieceColor color) {
    game_status_ = GameStatus::CHECKMATE;
    current_player_ = color;
}

void Game::switchPlayer() {
    current_player_ = (current_player_ == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
}

void Game::updateGameStatus() {
    if (board_.isInCheckmate(current_player_)) {
        game_status_ = GameStatus::CHECKMATE;
    } else if (board_.isInStalemate(current_player_)) {
        game_status_ = GameStatus::STALEMATE;
    } else if (isFiftyMoveRule() || isThreefoldRepetition() || isInsufficientMaterial()) {
        game_status_ = GameStatus::DRAW;
    } else if (board_.isInCheck(current_player_)) {
        game_status_ = GameStatus::CHECK;
    } else {
        game_status_ = GameStatus::ONGOING;
    }
}

bool Game::isThreefoldRepetition() const {
    return false;
}

bool Game::isFiftyMoveRule() const {
    return halfmove_clock_ >= 100;
}

bool Game::isInsufficientMaterial() const {
    auto whitePieces = board_.getAllPiecesPositions(PieceColor::WHITE);
    auto blackPieces = board_.getAllPiecesPositions(PieceColor::BLACK);
    
    if (whitePieces.size() <= 2 && blackPieces.size() <= 2) {
        bool whiteHasOnlyKing = whitePieces.size() == 1;
        bool blackHasOnlyKing = blackPieces.size() == 1;
        
        if (whiteHasOnlyKing && blackHasOnlyKing) {
            return true;
        }
        
        // King vs King + Knight/Bishop
        if ((whiteHasOnlyKing && blackPieces.size() == 2) || 
            (blackHasOnlyKing && whitePieces.size() == 2)) {
            return true;
        }
    }
    
    return false;
}

Move Game::createMove(const Position& from, const Position& to) {
    const Piece* piece = board_.getPiece(from);
    if (!piece) return Move(from, to);
    
    // Check for castling
    if (piece->getType() == PieceType::KING && Utils::abs(to.col - from.col) == 2) {
        if (to.col > from.col) {
            return Move(from, to, MoveType::CASTLING_KINGSIDE);
        } else {
            return Move(from, to, MoveType::CASTLING_QUEENSIDE);
        }
    }
    
    // Check for en passant
    if (piece->getType() == PieceType::PAWN && board_.getEnPassantTarget() == to) {
        return Move(from, to, MoveType::EN_PASSANT);
    }
    
    // Check for pawn promotion
    if (piece->getType() == PieceType::PAWN) {
        int promotionRow = (piece->getColor() == PieceColor::WHITE) ? 0 : 7;
        if (to.row == promotionRow) {
            return Move(from, to, PieceType::QUEEN);
        }
    }
    
    // Check for capture
    if (!board_.isSquareEmpty(to)) {
        return Move(from, to, MoveType::CAPTURE);
    }
    
    return Move(from, to, MoveType::NORMAL);
}

bool Game::executeCastling(const Move& move) {
    int row = move.getFrom().row;
    
    // Move the king
    board_.movePiece(move.getFrom(), move.getTo());
    
    // Move the rook
    if (move.getType() == MoveType::CASTLING_KINGSIDE) {
        board_.movePiece(Position(row, 7), Position(row, 5));
    } else {
        board_.movePiece(Position(row, 0), Position(row, 3));
    }
    
    return true;
}

bool Game::executeEnPassant(const Move& move) {
    // Move the pawn
    board_.movePiece(move.getFrom(), move.getTo());
    
    // Remove the captured pawn
    int capturedPawnRow = move.getFrom().row;
    Position capturedPawnPos(capturedPawnRow, move.getTo().col);
    board_.removePiece(capturedPawnPos);
    
    return true;
}

bool Game::executePromotion(const Move& move) {
    // Remove the pawn
    auto pawn = board_.removePiece(move.getFrom());
    if (!pawn) return false;
    
    // Create promoted piece
    std::unique_ptr<Piece> promotedPiece;
    switch (move.getPromotionPiece()) {
        case PieceType::QUEEN:
            promotedPiece = std::make_unique<Queen>(pawn->getColor());
            break;
        case PieceType::ROOK:
            promotedPiece = std::make_unique<Rook>(pawn->getColor());
            break;
        case PieceType::BISHOP:
            promotedPiece = std::make_unique<Bishop>(pawn->getColor());
            break;
        case PieceType::KNIGHT:
            promotedPiece = std::make_unique<Knight>(pawn->getColor());
            break;
        default:
            promotedPiece = std::make_unique<Queen>(pawn->getColor());
            break;
    }
    
    // Place promoted piece
    board_.placePiece(std::move(promotedPiece), move.getTo());
    
    return true;
}