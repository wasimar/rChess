#pragma once

#include "Position.h"
#include "Move.h"
#include <string>

class Board;

class Player {
public:
    Player(const std::string& name, PieceColor color);
    virtual ~Player() = default;
    
    const std::string& getName() const { return name_; }
    PieceColor getColor() const { return color_; }
    
    virtual Move getMove(const Board& board) = 0;
    virtual bool isHuman() const = 0;
    
protected:
    std::string name_;
    PieceColor color_;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name, PieceColor color);
    
    Move getMove(const Board& board) override;
    bool isHuman() const override { return true; }
};