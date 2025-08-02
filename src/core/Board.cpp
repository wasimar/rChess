#include "core/Board.h"
#include "core/Piece.h"
#include <iostream>
#include <algorithm>

Board::Board() : en_passant_target_(-1, -1) {
    initializeBoard();
}

Board::Board(const Board& other) : en_passant_target_(other.en_passant_target_) {
    copyBoard(other);
}

Board& Board::operator=(const Board& other) {
    if (this != &other) {
        en_passant_target_ = other.en_passant_target_;
        copyBoard(other);
    }
    return *this;
}

void Board::initializeBoard() {
    clearBoard();
    
    // Place pawns
    for (int col = 0; col < 8; ++col) {
        board_[1][col] = std::make_unique<Pawn>(PieceColor::BLACK);
        board_[6][col] = std::make_unique<Pawn>(PieceColor::WHITE);
    }
    
    // Place back pieces for black (row 0)
    board_[0][0] = std::make_unique<Rook>(PieceColor::BLACK);
    board_[0][1] = std::make_unique<Knight>(PieceColor::BLACK);
    board_[0][2] = std::make_unique<Bishop>(PieceColor::BLACK);
    board_[0][3] = std::make_unique<Queen>(PieceColor::BLACK);
    board_[0][4] = std::make_unique<King>(PieceColor::BLACK);
    board_[0][5] = std::make_unique<Bishop>(PieceColor::BLACK);
    board_[0][6] = std::make_unique<Knight>(PieceColor::BLACK);
    board_[0][7] = std::make_unique<Rook>(PieceColor::BLACK);
    
    // Place back pieces for white (row 7)
    board_[7][0] = std::make_unique<Rook>(PieceColor::WHITE);
    board_[7][1] = std::make_unique<Knight>(PieceColor::WHITE);
    board_[7][2] = std::make_unique<Bishop>(PieceColor::WHITE);
    board_[7][3] = std::make_unique<Queen>(PieceColor::WHITE);
    board_[7][4] = std::make_unique<King>(PieceColor::WHITE);
    board_[7][5] = std::make_unique<Bishop>(PieceColor::WHITE);
    board_[7][6] = std::make_unique<Knight>(PieceColor::WHITE);
    board_[7][7] = std::make_unique<Rook>(PieceColor::WHITE);
}

void Board::clearBoard() {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            board_[row][col] = nullptr;
        }
    }
    clearEnPassantTarget();
}

const Piece* Board::getPiece(const Position& pos) const {
    if (!pos.isValid()) return nullptr;
    return board_[pos.row][pos.col].get();
}

Piece* Board::getPiece(const Position& pos) {
    if (!pos.isValid()) return nullptr;
    return board_[pos.row][pos.col].get();
}

bool Board::placePiece(std::unique_ptr<Piece> piece, const Position& pos) {
    if (!pos.isValid()) return false;
    board_[pos.row][pos.col] = std::move(piece);
    return true;
}

std::unique_ptr<Piece> Board::removePiece(const Position& pos) {
    if (!pos.isValid()) return nullptr;
    return std::move(board_[pos.row][pos.col]);
}

bool Board::movePiece(const Position& from, const Position& to) {
    if (!from.isValid() || !to.isValid()) return false;
    
    auto piece = removePiece(from);
    if (!piece) return false;
    
    piece->setMoved(true);
    placePiece(std::move(piece), to);
    return true;
}

bool Board::isSquareEmpty(const Position& pos) const {
    return getPiece(pos) == nullptr;
}

bool Board::isSquareOccupiedBy(const Position& pos, PieceColor color) const {
    const Piece* piece = getPiece(pos);
    return piece && piece->getColor() == color;
}

Position Board::findKing(PieceColor color) const {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board_[row][col].get();
            if (piece && piece->getType() == PieceType::KING && piece->getColor() == color) {
                return Position(row, col);
            }
        }
    }
    return Position(-1, -1);
}

bool Board::isInCheck(PieceColor color) const {
    Position kingPos = findKing(color);
    if (!kingPos.isValid()) return false;
    
    PieceColor enemyColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    return isPositionAttacked(kingPos, enemyColor);
}

bool Board::isInCheckmate(PieceColor color) const {
    return isInCheck(color) && !hasValidMoves(color);
}

bool Board::isInStalemate(PieceColor color) const {
    return !isInCheck(color) && !hasValidMoves(color);
}

std::vector<Position> Board::getAllPiecesPositions(PieceColor color) const {
    std::vector<Position> positions;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board_[row][col].get();
            if (piece && piece->getColor() == color) {
                positions.emplace_back(row, col);
            }
        }
    }
    return positions;
}

std::vector<Position> Board::getAllValidMoves(PieceColor color) const {
    std::vector<Position> allMoves;
    auto piecePositions = getAllPiecesPositions(color);
    
    for (const auto& piecePos : piecePositions) {
        const Piece* piece = getPiece(piecePos);
        if (piece) {
            auto moves = piece->getPossibleMoves(piecePos, *this);
            for (const auto& move : moves) {
                if (!wouldBeInCheck(piecePos, move, color)) {
                    allMoves.push_back(move);
                }
            }
        }
    }
    return allMoves;
}

bool Board::wouldBeInCheck(const Position& from, const Position& to, PieceColor color) const {
    Board tempBoard(*this);
    
    auto piece = tempBoard.removePiece(from);
    if (!piece) return true;
    
    tempBoard.placePiece(std::move(piece), to);
    return tempBoard.isInCheck(color);
}

bool Board::canCastleKingside(PieceColor color) const {
    int row = (color == PieceColor::WHITE) ? 7 : 0;
    
    const Piece* king = getPiece(Position(row, 4));
    const Piece* rook = getPiece(Position(row, 7));
    
    if (!king || !rook || king->hasMoved() || rook->hasMoved()) return false;
    if (king->getType() != PieceType::KING || rook->getType() != PieceType::ROOK) return false;
    
    if (!isSquareEmpty(Position(row, 5)) || !isSquareEmpty(Position(row, 6))) return false;
    
    if (isInCheck(color)) return false;
    
    PieceColor enemyColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    return !isPositionAttacked(Position(row, 5), enemyColor) && 
           !isPositionAttacked(Position(row, 6), enemyColor);
}

bool Board::canCastleQueenside(PieceColor color) const {
    int row = (color == PieceColor::WHITE) ? 7 : 0;
    
    const Piece* king = getPiece(Position(row, 4));
    const Piece* rook = getPiece(Position(row, 0));
    
    if (!king || !rook || king->hasMoved() || rook->hasMoved()) return false;
    if (king->getType() != PieceType::KING || rook->getType() != PieceType::ROOK) return false;
    
    if (!isSquareEmpty(Position(row, 1)) || !isSquareEmpty(Position(row, 2)) || !isSquareEmpty(Position(row, 3))) return false;
    
    if (isInCheck(color)) return false;
    
    PieceColor enemyColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    return !isPositionAttacked(Position(row, 2), enemyColor) && 
           !isPositionAttacked(Position(row, 3), enemyColor);
}

void Board::print() const {
    std::cout << "  a b c d e f g h\n";
    for (int row = 0; row < 8; ++row) {
        std::cout << (8 - row) << " ";
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board_[row][col].get();
            if (piece) {
                std::cout << piece->getSymbol() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << (8 - row) << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}

void Board::copyBoard(const Board& other) {
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (other.board_[row][col]) {
                board_[row][col] = other.board_[row][col]->clone();
            } else {
                board_[row][col] = nullptr;
            }
        }
    }
}

bool Board::isPositionAttacked(const Position& pos, PieceColor attackingColor) const {
    auto attackerPositions = getAllPiecesPositions(attackingColor);
    
    for (const auto& attackerPos : attackerPositions) {
        const Piece* piece = getPiece(attackerPos);
        if (piece && piece->isValidMove(attackerPos, pos, *this)) {
            return true;
        }
    }
    return false;
}

bool Board::hasValidMoves(PieceColor color) const {
    auto validMoves = getAllValidMoves(color);
    return !validMoves.empty();
}