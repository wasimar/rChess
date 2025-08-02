#include "core/Piece.h"
#include "core/Board.h"
#include "utils/Utils.h"
#include <algorithm>
#include <cmath>

// Base Piece class implementation
Piece::Piece(PieceColor color, PieceType type) 
    : color_(color), type_(type), has_moved_(false) {}

bool Piece::isPathClear(const Position& from, const Position& to, const Board& board) const {
    int deltaRow = to.row - from.row;
    int deltaCol = to.col - from.col;
    
    int stepRow = (deltaRow == 0) ? 0 : (deltaRow > 0 ? 1 : -1);
    int stepCol = (deltaCol == 0) ? 0 : (deltaCol > 0 ? 1 : -1);
    
    Position current(from.row + stepRow, from.col + stepCol);
    
    while (current != to) {
        if (!board.isSquareEmpty(current)) {
            return false;
        }
        current.row += stepRow;
        current.col += stepCol;
    }
    
    return true;
}

bool Piece::isEnemyPiece(const Position& pos, const Board& board) const {
    const Piece* piece = board.getPiece(pos);
    return piece && piece->getColor() != color_;
}

bool Piece::isEmptySquare(const Position& pos, const Board& board) const {
    return board.isSquareEmpty(pos);
}

// Pawn implementation
Pawn::Pawn(PieceColor color) : Piece(color, PieceType::PAWN) {}

std::vector<Position> Pawn::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;
    int direction = (color_ == PieceColor::WHITE) ? -1 : 1;
    
    // Forward move
    Position oneForward(from.row + direction, from.col);
    if (oneForward.isValid() && board.isSquareEmpty(oneForward)) {
        moves.push_back(oneForward);
        
        // Two squares forward from starting position
        if (!has_moved_) {
            Position twoForward(from.row + 2 * direction, from.col);
            if (twoForward.isValid() && board.isSquareEmpty(twoForward)) {
                moves.push_back(twoForward);
            }
        }
    }
    
    // Diagonal captures
    Position leftCapture(from.row + direction, from.col - 1);
    Position rightCapture(from.row + direction, from.col + 1);
    
    if (leftCapture.isValid() && isEnemyPiece(leftCapture, board)) {
        moves.push_back(leftCapture);
    }
    
    if (rightCapture.isValid() && isEnemyPiece(rightCapture, board)) {
        moves.push_back(rightCapture);
    }
    
    // En passant
    Position enPassantTarget = board.getEnPassantTarget();
    if (enPassantTarget.isValid()) {
        if (leftCapture == enPassantTarget || rightCapture == enPassantTarget) {
            moves.push_back(enPassantTarget);
        }
    }
    
    return moves;
}

bool Pawn::isValidMove(const Position& from, const Position& to, const Board& board) const {
    auto possibleMoves = getPossibleMoves(from, board);
    return std::find(possibleMoves.begin(), possibleMoves.end(), to) != possibleMoves.end();
}

std::unique_ptr<Piece> Pawn::clone() const {
    auto newPawn = std::make_unique<Pawn>(color_);
    newPawn->has_moved_ = has_moved_;
    return newPawn;
}

char Pawn::getSymbol() const {
    return (color_ == PieceColor::WHITE) ? 'P' : 'p';
}

// Rook implementation
Rook::Rook(PieceColor color) : Piece(color, PieceType::ROOK) {}

std::vector<Position> Rook::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;
    
    // Horizontal and vertical directions
    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
    
    for (auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            Position to(from.row + i * dir[0], from.col + i * dir[1]);
            
            if (!to.isValid()) break;
            
            if (board.isSquareEmpty(to)) {
                moves.push_back(to);
            } else if (isEnemyPiece(to, board)) {
                moves.push_back(to);
                break;
            } else {
                break;
            }
        }
    }
    
    return moves;
}

bool Rook::isValidMove(const Position& from, const Position& to, const Board& board) const {
    if (from.row != to.row && from.col != to.col) return false;
    
    if (!board.isSquareEmpty(to) && !isEnemyPiece(to, board)) return false;
    
    return isPathClear(from, to, board);
}

std::unique_ptr<Piece> Rook::clone() const {
    auto newRook = std::make_unique<Rook>(color_);
    newRook->has_moved_ = has_moved_;
    return newRook;
}

char Rook::getSymbol() const {
    return (color_ == PieceColor::WHITE) ? 'R' : 'r';
}

// Knight implementation
Knight::Knight(PieceColor color) : Piece(color, PieceType::KNIGHT) {}

std::vector<Position> Knight::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;
    
    // Knight move patterns
    int knightMoves[8][2] = {
        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
    };
    
    for (auto& move : knightMoves) {
        Position to(from.row + move[0], from.col + move[1]);
        
        if (to.isValid() && (board.isSquareEmpty(to) || isEnemyPiece(to, board))) {
            moves.push_back(to);
        }
    }
    
    return moves;
}

bool Knight::isValidMove(const Position& from, const Position& to, const Board& board) const {
    int deltaRow = Utils::abs(to.row - from.row);
    int deltaCol = Utils::abs(to.col - from.col);
    
    if (!((deltaRow == 2 && deltaCol == 1) || (deltaRow == 1 && deltaCol == 2))) {
        return false;
    }
    
    return board.isSquareEmpty(to) || isEnemyPiece(to, board);
}

std::unique_ptr<Piece> Knight::clone() const {
    auto newKnight = std::make_unique<Knight>(color_);
    newKnight->has_moved_ = has_moved_;
    return newKnight;
}

char Knight::getSymbol() const {
    return (color_ == PieceColor::WHITE) ? 'N' : 'n';
}

// Bishop implementation
Bishop::Bishop(PieceColor color) : Piece(color, PieceType::BISHOP) {}

std::vector<Position> Bishop::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;
    
    // Diagonal directions
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    
    for (auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            Position to(from.row + i * dir[0], from.col + i * dir[1]);
            
            if (!to.isValid()) break;
            
            if (board.isSquareEmpty(to)) {
                moves.push_back(to);
            } else if (isEnemyPiece(to, board)) {
                moves.push_back(to);
                break;
            } else {
                break;
            }
        }
    }
    
    return moves;
}

bool Bishop::isValidMove(const Position& from, const Position& to, const Board& board) const {
    if (Utils::abs(to.row - from.row) != Utils::abs(to.col - from.col)) return false;
    
    if (!board.isSquareEmpty(to) && !isEnemyPiece(to, board)) return false;
    
    return isPathClear(from, to, board);
}

std::unique_ptr<Piece> Bishop::clone() const {
    auto newBishop = std::make_unique<Bishop>(color_);
    newBishop->has_moved_ = has_moved_;
    return newBishop;
}

char Bishop::getSymbol() const {
    return (color_ == PieceColor::WHITE) ? 'B' : 'b';
}

// Queen implementation
Queen::Queen(PieceColor color) : Piece(color, PieceType::QUEEN) {}

std::vector<Position> Queen::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;
    
    // All 8 directions (rook + bishop)
    int directions[8][2] = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    
    for (auto& dir : directions) {
        for (int i = 1; i < 8; ++i) {
            Position to(from.row + i * dir[0], from.col + i * dir[1]);
            
            if (!to.isValid()) break;
            
            if (board.isSquareEmpty(to)) {
                moves.push_back(to);
            } else if (isEnemyPiece(to, board)) {
                moves.push_back(to);
                break;
            } else {
                break;
            }
        }
    }
    
    return moves;
}

bool Queen::isValidMove(const Position& from, const Position& to, const Board& board) const {
    bool isRookMove = (from.row == to.row || from.col == to.col);
    bool isBishopMove = (Utils::abs(to.row - from.row) == Utils::abs(to.col - from.col));
    
    if (!isRookMove && !isBishopMove) return false;
    
    if (!board.isSquareEmpty(to) && !isEnemyPiece(to, board)) return false;
    
    return isPathClear(from, to, board);
}

std::unique_ptr<Piece> Queen::clone() const {
    auto newQueen = std::make_unique<Queen>(color_);
    newQueen->has_moved_ = has_moved_;
    return newQueen;
}

char Queen::getSymbol() const {
    return (color_ == PieceColor::WHITE) ? 'Q' : 'q';
}

// King implementation
King::King(PieceColor color) : Piece(color, PieceType::KING) {}

std::vector<Position> King::getPossibleMoves(const Position& from, const Board& board) const {
    std::vector<Position> moves;
    
    // All 8 directions, one step each
    int directions[8][2] = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0},
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    
    for (auto& dir : directions) {
        Position to(from.row + dir[0], from.col + dir[1]);
        
        if (to.isValid() && (board.isSquareEmpty(to) || isEnemyPiece(to, board))) {
            moves.push_back(to);
        }
    }
    
    // Castling moves
    if (!has_moved_) {
        if (board.canCastleKingside(color_)) {
            moves.emplace_back(from.row, from.col + 2);
        }
        if (board.canCastleQueenside(color_)) {
            moves.emplace_back(from.row, from.col - 2);
        }
    }
    
    return moves;
}

bool King::isValidMove(const Position& from, const Position& to, const Board& board) const {
    int deltaRow = Utils::abs(to.row - from.row);
    int deltaCol = Utils::abs(to.col - from.col);
    
    // Normal king move (one square in any direction)
    if (deltaRow <= 1 && deltaCol <= 1) {
        return board.isSquareEmpty(to) || isEnemyPiece(to, board);
    }
    
    // Castling
    if (!has_moved_ && deltaRow == 0 && deltaCol == 2) {
        if (to.col > from.col) {
            return board.canCastleKingside(color_);
        } else {
            return board.canCastleQueenside(color_);
        }
    }
    
    return false;
}

std::unique_ptr<Piece> King::clone() const {
    auto newKing = std::make_unique<King>(color_);
    newKing->has_moved_ = has_moved_;
    return newKing;
}

char King::getSymbol() const {
    return (color_ == PieceColor::WHITE) ? 'K' : 'k';
}