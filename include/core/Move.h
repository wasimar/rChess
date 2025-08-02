#pragma once

#include "Position.h"
#include "Piece.h"
#include <memory>

enum class MoveType {
    NORMAL,
    CAPTURE,
    CASTLING_KINGSIDE,
    CASTLING_QUEENSIDE,
    EN_PASSANT,
    PAWN_PROMOTION
};

class Move {
public:
    Move(const Position& from, const Position& to, MoveType type = MoveType::NORMAL);
    Move(const Position& from, const Position& to, PieceType promotionPiece);
    
    Move(const Move&) = delete;
    Move& operator=(const Move&) = delete;
    Move(Move&&) = default;
    Move& operator=(Move&&) = default;
    
    Position getFrom() const { return from_; }
    Position getTo() const { return to_; }
    MoveType getType() const { return type_; }
    PieceType getPromotionPiece() const { return promotion_piece_; }
    
    void setCapturedPiece(std::unique_ptr<Piece> piece) { captured_piece_ = std::move(piece); }
    const Piece* getCapturedPiece() const { return captured_piece_.get(); }
    std::unique_ptr<Piece> takeCapturedPiece() { return std::move(captured_piece_); }
    
    bool isCapture() const { return type_ == MoveType::CAPTURE || type_ == MoveType::EN_PASSANT; }
    bool isCastling() const { return type_ == MoveType::CASTLING_KINGSIDE || type_ == MoveType::CASTLING_QUEENSIDE; }
    bool isPromotion() const { return type_ == MoveType::PAWN_PROMOTION; }
    
    std::string toAlgebraicNotation(const class Board& board) const;
    
private:
    Position from_;
    Position to_;
    MoveType type_;
    PieceType promotion_piece_;
    std::unique_ptr<Piece> captured_piece_;
};