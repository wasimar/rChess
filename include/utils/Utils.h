#pragma once

#include <string>
#include "core/Position.h"

namespace Utils {
    std::string colorToString(PieceColor color);
    std::string pieceTypeToString(PieceType type);
    std::string gameStatusToString(GameStatus status);
    
    char pieceToChar(PieceType type, PieceColor color);
    std::string pieceToUnicode(PieceType type, PieceColor color);
    
    bool isValidSquareNotation(const std::string& square);
    std::string positionToSquare(const Position& pos);
    Position squareToPosition(const std::string& square);
    
    int abs(int value);
    int max(int a, int b);
    int min(int a, int b);
    
    void enableUnicodeOutput();
    void disableUnicodeOutput();
}