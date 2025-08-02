#include "core/Player.h"
#include "core/Board.h"
#include "ui/InputParser.h"
#include "ui/Display.h"
#include "utils/Utils.h"
#include <iostream>

Player::Player(const std::string& name, PieceColor color)
    : name_(name), color_(color) {}

HumanPlayer::HumanPlayer(const std::string& name, PieceColor color)
    : Player(name, color) {}

Move HumanPlayer::getMove(const Board& board) {
    InputParser parser;
    Display display;
    
    while (true) {
        display.displayPrompt(name_ + "'s turn (" + Utils::colorToString(color_) + "): ");
        std::string input = parser.getUserInput();
        
        if (parser.isQuitCommand(input)) {
            return Move(Position(-1, -1), Position(-1, -1));
        }
        
        if (parser.isHelpCommand(input)) {
            display.showHelpMessage();
            continue;
        }
        
        auto movePositions = parser.parseMove(input);
        if (movePositions) {
            Position from = movePositions->first;
            Position to = movePositions->second;
            
            const Piece* piece = board.getPiece(from);
            if (!piece) {
                display.displayError("No piece at that position!");
                continue;
            }
            
            if (piece->getColor() != color_) {
                display.displayError("That's not your piece!");
                continue;
            }
            
            if (!piece->isValidMove(from, to, board)) {
                display.displayError("Invalid move!");
                continue;
            }
            
            // Check if it's a pawn promotion
            if (piece->getType() == PieceType::PAWN) {
                int promotionRow = (color_ == PieceColor::WHITE) ? 0 : 7;
                if (to.row == promotionRow) {
                    display.displayPrompt("Promote to (Q/R/B/N): ");
                    std::string promotionInput = parser.getUserInput();
                    auto promotionPiece = parser.parsePromotionPiece(promotionInput);
                    if (promotionPiece) {
                        return Move(from, to, *promotionPiece);
                    } else {
                        return Move(from, to, PieceType::QUEEN);  // Default to queen
                    }
                }
            }
            
            return Move(from, to);
        } else {
            display.displayError("Invalid input! Use format like 'e2e4' or type 'help'");
        }
    }
}