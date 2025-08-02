#include "ui/InputParser.h"
#include "utils/Utils.h"
#include <iostream>
#include <algorithm>
#include <cctype>

std::optional<std::pair<Position, Position>> InputParser::parseMove(const std::string& input) const {
    std::string trimmedInput = trim(input);
    
    if (trimmedInput.length() == 4) {
        std::string fromSquare = trimmedInput.substr(0, 2);
        std::string toSquare = trimmedInput.substr(2, 2);
        
        if (isValidSquare(fromSquare) && isValidSquare(toSquare)) {
            Position from = squareToPosition(fromSquare);
            Position to = squareToPosition(toSquare);
            return std::make_pair(from, to);
        }
    } else if (trimmedInput.length() == 5 && trimmedInput[2] == '-') {
        std::string fromSquare = trimmedInput.substr(0, 2);
        std::string toSquare = trimmedInput.substr(3, 2);
        
        if (isValidSquare(fromSquare) && isValidSquare(toSquare)) {
            Position from = squareToPosition(fromSquare);
            Position to = squareToPosition(toSquare);
            return std::make_pair(from, to);
        }
    }
    
    return std::nullopt;
}

std::optional<Position> InputParser::parsePosition(const std::string& input) const {
    std::string trimmedInput = trim(input);
    
    if (isValidSquare(trimmedInput)) {
        return squareToPosition(trimmedInput);
    }
    
    return std::nullopt;
}

std::optional<PieceType> InputParser::parsePromotionPiece(const std::string& input) const {
    std::string lowerInput = toLower(trim(input));
    
    if (lowerInput == "q" || lowerInput == "queen") {
        return PieceType::QUEEN;
    } else if (lowerInput == "r" || lowerInput == "rook") {
        return PieceType::ROOK;
    } else if (lowerInput == "b" || lowerInput == "bishop") {
        return PieceType::BISHOP;
    } else if (lowerInput == "n" || lowerInput == "knight") {
        return PieceType::KNIGHT;
    }
    
    return std::nullopt;
}

bool InputParser::isQuitCommand(const std::string& input) const {
    std::string lowerInput = toLower(trim(input));
    return lowerInput == "quit" || lowerInput == "q" || lowerInput == "exit";
}

bool InputParser::isHelpCommand(const std::string& input) const {
    std::string lowerInput = toLower(trim(input));
    return lowerInput == "help" || lowerInput == "h" || lowerInput == "?";
}

bool InputParser::isUndoCommand(const std::string& input) const {
    std::string lowerInput = toLower(trim(input));
    return lowerInput == "undo" || lowerInput == "u";
}

bool InputParser::isDrawCommand(const std::string& input) const {
    std::string lowerInput = toLower(trim(input));
    return lowerInput == "draw" || lowerInput == "d";
}

bool InputParser::isResignCommand(const std::string& input) const {
    std::string lowerInput = toLower(trim(input));
    return lowerInput == "resign" || lowerInput == "r";
}

std::string InputParser::getUserInput() const {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

std::string InputParser::getPlayerName(PieceColor color) const {
    std::string colorName = Utils::colorToString(color);
    std::cout << "Enter name for " << colorName << " player: ";
    
    std::string name;
    std::getline(std::cin, name);
    
    if (name.empty()) {
        name = colorName + " Player";
    }
    
    return name;
}

bool InputParser::isValidSquare(const std::string& square) const {
    if (square.length() != 2) return false;
    
    char file = square[0];
    char rank = square[1];
    
    return (file >= 'a' && file <= 'h') && (rank >= '1' && rank <= '8');
}

Position InputParser::squareToPosition(const std::string& square) const {
    if (!isValidSquare(square)) return Position(-1, -1);
    
    int col = square[0] - 'a';
    int row = '8' - square[1];
    
    return Position(row, col);
}

std::string InputParser::trim(const std::string& str) const {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

std::string InputParser::toLower(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}