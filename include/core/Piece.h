#pragma once

#include "Position.h"
#include <vector>
#include <memory>

class Board;

class Piece {
public:
    Piece(PieceColor color, PieceType type);
    virtual ~Piece() = default;
    
    PieceColor getColor() const { return color_; }
    PieceType getType() const { return type_; }
    bool hasMoved() const { return has_moved_; }
    void setMoved(bool moved) { has_moved_ = moved; }
    
    virtual std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const = 0;
    virtual bool isValidMove(const Position& from, const Position& to, const Board& board) const = 0;
    virtual std::unique_ptr<Piece> clone() const = 0;
    virtual char getSymbol() const = 0;
    
protected:
    PieceColor color_;
    PieceType type_;
    bool has_moved_;
    
    bool isPathClear(const Position& from, const Position& to, const Board& board) const;
    bool isEnemyPiece(const Position& pos, const Board& board) const;
    bool isEmptySquare(const Position& pos, const Board& board) const;
};

class Pawn : public Piece {
public:
    explicit Pawn(PieceColor color);
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
    char getSymbol() const override;
};

class Rook : public Piece {
public:
    explicit Rook(PieceColor color);
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
    char getSymbol() const override;
};

class Knight : public Piece {
public:
    explicit Knight(PieceColor color);
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
    char getSymbol() const override;
};

class Bishop : public Piece {
public:
    explicit Bishop(PieceColor color);
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
    char getSymbol() const override;
};

class Queen : public Piece {
public:
    explicit Queen(PieceColor color);
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
    char getSymbol() const override;
};

class King : public Piece {
public:
    explicit King(PieceColor color);
    std::vector<Position> getPossibleMoves(const Position& from, const Board& board) const override;
    bool isValidMove(const Position& from, const Position& to, const Board& board) const override;
    std::unique_ptr<Piece> clone() const override;
    char getSymbol() const override;
};