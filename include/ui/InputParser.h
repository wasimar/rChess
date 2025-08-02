#pragma once

#include "core/Position.h"
#include "core/Move.h"
#include <string>
#include <optional>

class InputParser {
public:
    InputParser() = default;
    ~InputParser() = default;
    
    std::optional<std::pair<Position, Position>> parseMove(const std::string& input) const;
    std::optional<Position> parsePosition(const std::string& input) const;
    std::optional<PieceType> parsePromotionPiece(const std::string& input) const;
    
    bool isQuitCommand(const std::string& input) const;
    bool isHelpCommand(const std::string& input) const;
    bool isUndoCommand(const std::string& input) const;
    bool isDrawCommand(const std::string& input) const;
    bool isResignCommand(const std::string& input) const;
    
    std::string getUserInput() const;
    std::string getPlayerName(PieceColor color) const;
    
private:
    bool isValidSquare(const std::string& square) const;
    Position squareToPosition(const std::string& square) const;
    std::string trim(const std::string& str) const;
    std::string toLower(const std::string& str) const;
};