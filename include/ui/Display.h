#pragma once

#include "core/Board.h"
#include "core/Game.h"
#include <string>

class Display {
public:
    Display();
    ~Display() = default;
    
    void showBoard(const Board& board) const;
    void showGameStatus(const Game& game) const;
    void showMoveHistory(const Game& game) const;
    void showValidMoves(const std::vector<Position>& moves) const;
    
    void clearScreen() const;
    void showWelcomeMessage() const;
    void showHelpMessage() const;
    void showGameOverMessage(const Game& game) const;
    
    void displayMessage(const std::string& message) const;
    void displayError(const std::string& error) const;
    void displayPrompt(const std::string& prompt) const;
    
private:
    std::string positionToString(const Position& pos) const;
    std::string pieceToUnicode(const Piece* piece) const;
    std::string getSquareColor(int row, int col) const;
    void printBoardHeader() const;
    void printBoardFooter() const;
};