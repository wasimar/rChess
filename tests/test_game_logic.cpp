#include <gtest/gtest.h>
#include "core/Game.h"

class GameTest : public ::testing::Test {
protected:
    void SetUp() override {
        game.startNewGame();
    }
    
    Game game;
};

TEST_F(GameTest, InitialGameState) {
    EXPECT_EQ(game.getCurrentPlayer(), PieceColor::WHITE);
    EXPECT_EQ(game.getGameStatus(), GameStatus::ONGOING);
    EXPECT_TRUE(game.getMoveHistory().empty());
}

TEST_F(GameTest, ValidMoves) {
    // Test initial pawn move
    EXPECT_TRUE(game.isValidMove(Position(6, 4), Position(5, 4)));  // e2-e3
    EXPECT_TRUE(game.isValidMove(Position(6, 4), Position(4, 4)));  // e2-e4
    
    // Test invalid moves
    EXPECT_FALSE(game.isValidMove(Position(6, 4), Position(3, 4))); // e2-e5 (too far)
    EXPECT_FALSE(game.isValidMove(Position(1, 4), Position(2, 4))); // Black piece on white turn
}

TEST_F(GameTest, MakeMove) {
    // Make a valid move
    EXPECT_TRUE(game.makeMove(Position(6, 4), Position(4, 4)));  // e2-e4
    
    // Check that turn switched
    EXPECT_EQ(game.getCurrentPlayer(), PieceColor::BLACK);
    
    // Check move history
    EXPECT_EQ(game.getMoveHistory().size(), 1);
}

TEST_F(GameTest, TurnAlternation) {
    EXPECT_EQ(game.getCurrentPlayer(), PieceColor::WHITE);
    
    game.makeMove(Position(6, 4), Position(4, 4));  // White move
    EXPECT_EQ(game.getCurrentPlayer(), PieceColor::BLACK);
    
    game.makeMove(Position(1, 4), Position(3, 4));  // Black move
    EXPECT_EQ(game.getCurrentPlayer(), PieceColor::WHITE);
}

TEST_F(GameTest, UndoMove) {
    // Make a move
    game.makeMove(Position(6, 4), Position(4, 4));  // e2-e4
    EXPECT_EQ(game.getCurrentPlayer(), PieceColor::BLACK);
    EXPECT_EQ(game.getMoveHistory().size(), 1);
    
    // Undo the move
    game.undoLastMove();
    EXPECT_EQ(game.getCurrentPlayer(), PieceColor::WHITE);
    EXPECT_EQ(game.getMoveHistory().size(), 0);
    
    // Check that piece is back in original position
    EXPECT_EQ(game.getBoard().getPiece(Position(6, 4))->getType(), PieceType::PAWN);
    EXPECT_TRUE(game.getBoard().isSquareEmpty(Position(4, 4)));
}