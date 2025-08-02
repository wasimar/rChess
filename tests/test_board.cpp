#include <gtest/gtest.h>
#include "core/Board.h"

class BoardTest : public ::testing::Test {
protected:
    void SetUp() override {
        board.initializeBoard();
    }
    
    Board board;
};

TEST_F(BoardTest, InitialBoardSetup) {
    // Test that pieces are in correct starting positions
    EXPECT_EQ(board.getPiece(Position(0, 0))->getType(), PieceType::ROOK);
    EXPECT_EQ(board.getPiece(Position(0, 0))->getColor(), PieceColor::BLACK);
    
    EXPECT_EQ(board.getPiece(Position(7, 4))->getType(), PieceType::KING);
    EXPECT_EQ(board.getPiece(Position(7, 4))->getColor(), PieceColor::WHITE);
    
    // Test empty squares
    EXPECT_TRUE(board.isSquareEmpty(Position(3, 3)));
    EXPECT_TRUE(board.isSquareEmpty(Position(4, 4)));
}

TEST_F(BoardTest, MovePiece) {
    // Move white pawn from e2 to e4
    Position from(6, 4);
    Position to(4, 4);
    
    EXPECT_TRUE(board.movePiece(from, to));
    EXPECT_TRUE(board.isSquareEmpty(from));
    EXPECT_FALSE(board.isSquareEmpty(to));
    EXPECT_EQ(board.getPiece(to)->getType(), PieceType::PAWN);
}

TEST_F(BoardTest, FindKing) {
    Position whiteKing = board.findKing(PieceColor::WHITE);
    Position blackKing = board.findKing(PieceColor::BLACK);
    
    EXPECT_EQ(whiteKing, Position(7, 4));
    EXPECT_EQ(blackKing, Position(0, 4));
}

TEST_F(BoardTest, CheckDetection) {
    // Initially no check
    EXPECT_FALSE(board.isInCheck(PieceColor::WHITE));
    EXPECT_FALSE(board.isInCheck(PieceColor::BLACK));
}