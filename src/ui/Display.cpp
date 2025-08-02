#include "ui/Display.h"
#include "utils/Utils.h"
#include <iostream>
#include <iomanip>

Display::Display() {
    Utils::enableUnicodeOutput();
}

void Display::showBoard(const Board& board) const {
    clearScreen();
    
    std::cout << "\n";
    printBoardHeader();
    
    for (int row = 0; row < 8; ++row) {
        std::cout << "  " << (8 - row) << " ";
        
        for (int col = 0; col < 8; ++col) {
            const Piece* piece = board.getPiece(Position(row, col));
            
            std::cout << getSquareColor(row, col);
            
            if (piece) {
                std::cout << pieceToUnicode(piece);
            } else {
                std::cout << " ";
            }
            
            std::cout << " ";
        }
        
        std::cout << "\033[0m " << (8 - row) << "\n";
    }
    
    printBoardFooter();
    std::cout << "\n";
}

void Display::showGameStatus(const Game& game) const {
    std::cout << "Game Status: " << game.getGameStatusString() << "\n";
    
    if (!game.getMoveHistory().empty()) {
        const Move& lastMove = game.getLastMove();
        std::cout << "Last move: " << positionToString(lastMove.getFrom()) 
                  << " to " << positionToString(lastMove.getTo()) << "\n";
    }
    
    std::cout << "\n";
}

void Display::showMoveHistory(const Game& game) const {
    const auto& history = game.getMoveHistory();
    if (history.empty()) {
        std::cout << "No moves played yet.\n";
        return;
    }
    
    std::cout << "Move History:\n";
    for (size_t i = 0; i < history.size(); i += 2) {
        std::cout << std::setw(3) << (i / 2 + 1) << ". ";
        
        // White move
        std::cout << std::setw(8) << history[i].toAlgebraicNotation(game.getBoard());
        
        // Black move (if exists)
        if (i + 1 < history.size()) {
            std::cout << std::setw(8) << history[i + 1].toAlgebraicNotation(game.getBoard());
        }
        
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Display::showValidMoves(const std::vector<Position>& moves) const {
    if (moves.empty()) {
        std::cout << "No valid moves available.\n";
        return;
    }
    
    std::cout << "Valid moves: ";
    for (size_t i = 0; i < moves.size(); ++i) {
        std::cout << positionToString(moves[i]);
        if (i < moves.size() - 1) std::cout << ", ";
    }
    std::cout << "\n";
}

void Display::clearScreen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Display::showWelcomeMessage() const {
    clearScreen();
    std::cout << "===============================================\n";
    std::cout << "           Welcome to Chess Game!\n";
    std::cout << "===============================================\n";
    std::cout << "A console-based chess implementation in C++\n";
    std::cout << "\nCommands:\n";
    std::cout << "  Move: e2e4 (from square to square)\n";
    std::cout << "  Help: help or h\n";
    std::cout << "  Undo: undo or u\n";
    std::cout << "  Quit: quit or q\n";
    std::cout << "  Draw: draw or d\n";
    std::cout << "  Resign: resign or r\n";
    std::cout << "\nPress Enter to start...\n";
    std::cin.get();
}

void Display::showHelpMessage() const {
    std::cout << "\n=== CHESS GAME HELP ===\n";
    std::cout << "Move format: [from][to] (e.g., e2e4, a7a8)\n";
    std::cout << "Squares are named a1-h8 (files a-h, ranks 1-8)\n";
    std::cout << "\nSpecial moves:\n";
    std::cout << "  Castling: Move king two squares (e1g1 for kingside)\n";
    std::cout << "  En passant: Capture diagonally to empty square\n";
    std::cout << "  Promotion: Move pawn to last rank, select piece\n";
    std::cout << "\nCommands:\n";
    std::cout << "  help/h     - Show this help\n";
    std::cout << "  undo/u     - Undo last move\n";
    std::cout << "  quit/q     - Quit game\n";
    std::cout << "  draw/d     - Offer draw\n";
    std::cout << "  resign/r   - Resign game\n";
    std::cout << "\nPiece symbols:\n";
    std::cout << "  ♔♕♖♗♘♙ (White: King Queen Rook Bishop Knight Pawn)\n";
    std::cout << "  ♚♛♜♝♞♟ (Black: King Queen Rook Bishop Knight Pawn)\n";
    std::cout << "\n";
}

void Display::showGameOverMessage(const Game& game) const {
    std::cout << "\n===============================================\n";
    std::cout << "                GAME OVER\n";
    std::cout << "===============================================\n";
    std::cout << game.getGameStatusString() << "\n";
    std::cout << "Thank you for playing!\n";
    std::cout << "===============================================\n\n";
}

void Display::displayMessage(const std::string& message) const {
    std::cout << message << "\n";
}

void Display::displayError(const std::string& error) const {
    std::cout << "\033[31mError: " << error << "\033[0m\n";
}

void Display::displayPrompt(const std::string& prompt) const {
    std::cout << prompt;
}

std::string Display::positionToString(const Position& pos) const {
    return Utils::positionToSquare(pos);
}

std::string Display::pieceToUnicode(const Piece* piece) const {
    if (!piece) return " ";
    return Utils::pieceToUnicode(piece->getType(), piece->getColor());
}

std::string Display::getSquareColor(int row, int col) const {
    bool isLightSquare = (row + col) % 2 == 0;
    if (isLightSquare) {
        return "\033[47m\033[30m";  // Light background, dark text
    } else {
        return "\033[40m\033[37m";  // Dark background, light text
    }
}

void Display::printBoardHeader() const {
    std::cout << "     a b c d e f g h\n";
    std::cout << "   ┌─────────────────┐\n";
}

void Display::printBoardFooter() const {
    std::cout << "   └─────────────────┘\n";
    std::cout << "     a b c d e f g h\n";
}