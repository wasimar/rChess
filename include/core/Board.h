#pragma once

#include "Position.h"
#include "Piece.h"
#include <array>
#include <memory>
#include <vector>

class Board {
public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    ~Board() = default;
    
    void initializeBoard();
    void clearBoard();
    
    const Piece* getPiece(const Position& pos) const;
    Piece* getPiece(const Position& pos);
    
    bool placePiece(std::unique_ptr<Piece> piece, const Position& pos);
    std::unique_ptr<Piece> removePiece(const Position& pos);
    bool movePiece(const Position& from, const Position& to);
    
    bool isSquareEmpty(const Position& pos) const;
    bool isSquareOccupiedBy(const Position& pos, PieceColor color) const;
    
    Position findKing(PieceColor color) const;
    bool isInCheck(PieceColor color) const;
    bool isInCheckmate(PieceColor color) const;
    bool isInStalemate(PieceColor color) const;
    
    std::vector<Position> getAllPiecesPositions(PieceColor color) const;
    std::vector<Position> getAllValidMoves(PieceColor color) const;
    
    bool wouldBeInCheck(const Position& from, const Position& to, PieceColor color) const;
    
    void setEnPassantTarget(const Position& pos) { en_passant_target_ = pos; }
    Position getEnPassantTarget() const { return en_passant_target_; }
    void clearEnPassantTarget() { en_passant_target_ = Position(-1, -1); }
    
    bool canCastleKingside(PieceColor color) const;
    bool canCastleQueenside(PieceColor color) const;
    
    void print() const;
    
private:
    std::array<std::array<std::unique_ptr<Piece>, 8>, 8> board_;
    Position en_passant_target_;
    
    void copyBoard(const Board& other);
    bool isPositionAttacked(const Position& pos, PieceColor attackingColor) const;
    bool hasValidMoves(PieceColor color) const;
};