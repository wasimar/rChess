#include "core/Move.h"
#include "core/Board.h"
#include "utils/Utils.h"

Move::Move(const Position& from, const Position& to, MoveType type)
    : from_(from), to_(to), type_(type), promotion_piece_(PieceType::QUEEN) {}

Move::Move(const Position& from, const Position& to, PieceType promotionPiece)
    : from_(from), to_(to), type_(MoveType::PAWN_PROMOTION), promotion_piece_(promotionPiece) {}

std::string Move::toAlgebraicNotation(const Board& board) const {
    const Piece* piece = board.getPiece(from_);
    if (!piece) return "";
    
    std::string notation;
    
    // Special moves
    if (type_ == MoveType::CASTLING_KINGSIDE) {
        return "O-O";
    } else if (type_ == MoveType::CASTLING_QUEENSIDE) {
        return "O-O-O";
    }
    
    // Piece designation (except for pawns)
    if (piece->getType() != PieceType::PAWN) {
        notation += piece->getSymbol();
        // Make uppercase for white pieces
        if (piece->getColor() == PieceColor::WHITE) {
            notation = std::toupper(notation[0]);
        }
    }
    
    // Capture notation
    if (isCapture()) {
        if (piece->getType() == PieceType::PAWN) {
            notation += ('a' + from_.col);
        }
        notation += "x";
    }
    
    // Destination square
    notation += Utils::positionToSquare(to_);
    
    // Promotion
    if (type_ == MoveType::PAWN_PROMOTION) {
        notation += "=";
        notation += Utils::pieceToChar(promotion_piece_, piece->getColor());
    }
    
    // En passant
    if (type_ == MoveType::EN_PASSANT) {
        notation += " e.p.";
    }
    
    return notation;
}