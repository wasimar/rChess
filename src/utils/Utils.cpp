#include "utils/Utils.h"
#include "core/Position.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace Utils {

std::string colorToString(PieceColor color) {
    return (color == PieceColor::WHITE) ? "White" : "Black";
}

std::string pieceTypeToString(PieceType type) {
    switch (type) {
        case PieceType::PAWN: return "Pawn";
        case PieceType::ROOK: return "Rook";
        case PieceType::KNIGHT: return "Knight";
        case PieceType::BISHOP: return "Bishop";
        case PieceType::QUEEN: return "Queen";
        case PieceType::KING: return "King";
        default: return "Unknown";
    }
}

std::string gameStatusToString(GameStatus status) {
    switch (status) {
        case GameStatus::ONGOING: return "Ongoing";
        case GameStatus::CHECK: return "Check";
        case GameStatus::CHECKMATE: return "Checkmate";
        case GameStatus::STALEMATE: return "Stalemate";
        case GameStatus::DRAW: return "Draw";
        default: return "Unknown";
    }
}

char pieceToChar(PieceType type, PieceColor color) {
    char piece;
    switch (type) {
        case PieceType::PAWN: piece = 'P'; break;
        case PieceType::ROOK: piece = 'R'; break;
        case PieceType::KNIGHT: piece = 'N'; break;
        case PieceType::BISHOP: piece = 'B'; break;
        case PieceType::QUEEN: piece = 'Q'; break;
        case PieceType::KING: piece = 'K'; break;
        default: piece = '?'; break;
    }
    return (color == PieceColor::WHITE) ? piece : (piece + ('a' - 'A'));
}

std::string pieceToUnicode(PieceType type, PieceColor color) {
    if (color == PieceColor::WHITE) {
        switch (type) {
            case PieceType::KING: return "♔";
            case PieceType::QUEEN: return "♕";
            case PieceType::ROOK: return "♖";
            case PieceType::BISHOP: return "♗";
            case PieceType::KNIGHT: return "♘";
            case PieceType::PAWN: return "♙";
            default: return "?";
        }
    } else {
        switch (type) {
            case PieceType::KING: return "♚";
            case PieceType::QUEEN: return "♛";
            case PieceType::ROOK: return "♜";
            case PieceType::BISHOP: return "♝";
            case PieceType::KNIGHT: return "♞";
            case PieceType::PAWN: return "♟";
            default: return "?";
        }
    }
}

bool isValidSquareNotation(const std::string& square) {
    if (square.length() != 2) return false;
    
    char file = square[0];
    char rank = square[1];
    
    return (file >= 'a' && file <= 'h') && (rank >= '1' && rank <= '8');
}

std::string positionToSquare(const Position& pos) {
    if (!pos.isValid()) return "";
    
    char file = 'a' + pos.col;
    char rank = '8' - pos.row;
    
    return std::string(1, file) + std::string(1, rank);
}

Position squareToPosition(const std::string& square) {
    if (!isValidSquareNotation(square)) return Position(-1, -1);
    
    int col = square[0] - 'a';
    int row = '8' - square[1];
    
    return Position(row, col);
}

int abs(int value) {
    return (value < 0) ? -value : value;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

void enableUnicodeOutput() {
#ifdef _WIN32
    std::cout << "\033[?1049h";
#endif
}

void disableUnicodeOutput() {
#ifdef _WIN32
    std::cout << "\033[?1049l";
#endif
}

}