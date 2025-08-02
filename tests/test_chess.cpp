#include "core/Game.h"
#include "ui/Display.h"
#include <iostream>

int main() {
    std::cout << "Testing Chess Game Implementation...\n\n";
    
    // Create a game instance
    Game game;
    Display display;
    
    std::cout << "1. Testing initial board setup:\n";
    display.showBoard(game.getBoard());
    
    std::cout << "2. Testing game status:\n";
    display.showGameStatus(game);
    
    std::cout << "3. Testing simple pawn move (e2e4):\n";
    Position from(6, 4);  // e2
    Position to(4, 4);    // e4
    
    if (game.makeMove(from, to)) {
        std::cout << "Move successful!\n";
        display.showBoard(game.getBoard());
        display.showGameStatus(game);
    } else {
        std::cout << "Move failed!\n";
    }
    
    std::cout << "4. Testing invalid move (e2e5 - pawn can't move 3 squares):\n";
    Position invalidFrom(6, 4);  // e2 (but pawn is now at e4)
    Position invalidTo(3, 4);    // e5
    
    if (game.makeMove(invalidFrom, invalidTo)) {
        std::cout << "Move successful (this shouldn't happen!)\n";
    } else {
        std::cout << "Move correctly rejected!\n";
    }
    
    std::cout << "5. Testing black pawn move (e7e5):\n";
    Position blackFrom(1, 4);  // e7
    Position blackTo(3, 4);    // e5
    
    if (game.makeMove(blackFrom, blackTo)) {
        std::cout << "Black move successful!\n";
        display.showBoard(game.getBoard());
        display.showGameStatus(game);
    } else {
        std::cout << "Black move failed!\n";
    }
    
    std::cout << "\n6. Testing move validation:\n";
    std::cout << "Current player: " << (game.getCurrentPlayer() == PieceColor::WHITE ? "White" : "Black") << "\n";
    
    auto validMoves = game.getValidMoves(Position(7, 1));  // White knight
    std::cout << "Valid moves for white knight at b1: " << validMoves.size() << " moves\n";
    
    std::cout << "\nTest completed successfully!\n";
    
    return 0;
}