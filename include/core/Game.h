#pragma once

#include "Board.h"
#include "Move.h"
#include "Player.h"
#include <vector>
#include <memory>

class Game {
public:
    Game();
    ~Game() = default;
    
    void startNewGame();
    bool makeMove(const Position& from, const Position& to);
    bool makeMove(const Move& move);
    
    void undoLastMove();
    
    PieceColor getCurrentPlayer() const { return current_player_; }
    GameStatus getGameStatus() const { return game_status_; }
    const Board& getBoard() const { return board_; }
    
    const std::vector<Move>& getMoveHistory() const { return move_history_; }
    const Move& getLastMove() const;
    
    bool isValidMove(const Position& from, const Position& to) const;
    std::vector<Position> getValidMoves(const Position& from) const;
    
    bool isGameOver() const;
    std::string getGameStatusString() const;
    
    void setPlayer(PieceColor color, std::unique_ptr<Player> player);
    Player* getPlayer(PieceColor color) const;
    
    bool offerDraw();
    bool acceptDraw();
    void resignGame(PieceColor color);
    
private:
    Board board_;
    PieceColor current_player_;
    GameStatus game_status_;
    std::vector<Move> move_history_;
    std::unique_ptr<Player> white_player_;
    std::unique_ptr<Player> black_player_;
    
    int halfmove_clock_;
    int fullmove_number_;
    bool draw_offered_;
    
    void switchPlayer();
    void updateGameStatus();
    bool isThreefoldRepetition() const;
    bool isFiftyMoveRule() const;
    bool isInsufficientMaterial() const;
    
    Move createMove(const Position& from, const Position& to);
    bool executeCastling(const Move& move);
    bool executeEnPassant(const Move& move);
    bool executePromotion(const Move& move);
};