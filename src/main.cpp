#include "core/Game.h"
#include "core/Player.h"
#include "ui/Display.h"
#include "ui/InputParser.h"
#include "utils/Utils.h"
#include <iostream>
#include <memory>

int main() {
    Display display;
    InputParser parser;
    Game game;
    
    display.showWelcomeMessage();
    
    // Get player names
    std::string whiteName = parser.getPlayerName(PieceColor::WHITE);
    std::string blackName = parser.getPlayerName(PieceColor::BLACK);
    
    // Create players
    auto whitePlayer = std::make_unique<HumanPlayer>(whiteName, PieceColor::WHITE);
    auto blackPlayer = std::make_unique<HumanPlayer>(blackName, PieceColor::BLACK);
    
    game.setPlayer(PieceColor::WHITE, std::move(whitePlayer));
    game.setPlayer(PieceColor::BLACK, std::move(blackPlayer));
    
    // Main game loop
    while (!game.isGameOver()) {
        display.showBoard(game.getBoard());
        display.showGameStatus(game);
        
        Player* currentPlayer = game.getPlayer(game.getCurrentPlayer());
        if (!currentPlayer) {
            display.displayError("No player found for current turn!");
            break;
        }
        
        // Get player input
        display.displayPrompt(currentPlayer->getName() + "'s turn (" + 
                            Utils::colorToString(game.getCurrentPlayer()) + "): ");
        std::string input = parser.getUserInput();
        
        // Handle commands
        if (parser.isQuitCommand(input)) {
            display.displayMessage("Thanks for playing!");
            break;
        } else if (parser.isHelpCommand(input)) {
            display.showHelpMessage();
            continue;
        } else if (parser.isUndoCommand(input)) {
            game.undoLastMove();
            display.displayMessage("Move undone.");
            continue;
        } else if (parser.isDrawCommand(input)) {
            if (game.offerDraw()) {
                display.displayMessage("Draw offered. Opponent, type 'draw' to accept.");
            }
            continue;
        } else if (parser.isResignCommand(input)) {
            game.resignGame(game.getCurrentPlayer());
            display.displayMessage(currentPlayer->getName() + " resigned.");
            break;
        }
        
        // Parse and execute move
        auto movePositions = parser.parseMove(input);
        if (movePositions) {
            Position from = movePositions->first;
            Position to = movePositions->second;
            
            if (game.makeMove(from, to)) {
                display.displayMessage("Move executed: " + 
                                     Utils::positionToSquare(from) + " to " + 
                                     Utils::positionToSquare(to));
            } else {
                display.displayError("Invalid move! Try again.");
            }
        } else {
            display.displayError("Invalid input format! Use 'e2e4' or type 'help'");
        }
    }
    
    // Show final game state
    if (game.isGameOver()) {
        display.showBoard(game.getBoard());
        display.showGameOverMessage(game);
        display.showMoveHistory(game);
    }
    
    return 0;
}