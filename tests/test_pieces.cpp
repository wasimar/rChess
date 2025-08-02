#include <gtest/gtest.h>
#include "core/Piece.h"
#include "core/Board.h"

class PieceTest : public ::testing::Test {
protected:
    void SetUp() override {
        board.clearBoard();
    }
    
    Board board;
};

TEST_F(PieceTest, PawnMoves) {
    auto pawn = std::make_unique<Pawn>(PieceColor::WHITE);
    Position pos(6, 4);  // e2
    board.placePiece(std::move(pawn), pos);
    
    const Piece* piece = board.getPiece(pos);
    auto moves = piece->getPossibleMoves(pos, board);
    
    // Pawn should be able to move 1 or 2 squares forward from starting position
    EXPECT_GE(moves.size(), 1);
    EXPECT_LE(moves.size(), 2);
}

TEST_F(PieceTest, KnightMoves) {
    auto knight = std::make_unique<Knight>(PieceColor::WHITE);
    Position pos(4, 4);  // e4 - center of board
    board.placePiece(std::move(knight), pos);
    
    const Piece* piece = board.getPiece(pos);
    auto moves = piece->getPossibleMoves(pos, board);
    
    // Knight in center should have up to 8 moves
    EXPECT_EQ(moves.size(), 8);
}

TEST_F(PieceTest, RookMoves) {
    auto rook = std::make_unique<Rook>(PieceColor::WHITE);
    Position pos(4, 4);  // e4 - center of board
    board.placePiece(std::move(rook), pos);
    
    const Piece* piece = board.getPiece(pos);
    auto moves = piece->getPossibleMoves(pos, board);
    
    // Rook in center should have 14 moves (7 horizontal + 7 vertical)
    EXPECT_EQ(moves.size(), 14);
}

TEST_F(PieceTest, BishopMoves) {
    auto bishop = std::make_unique<Bishop>(PieceColor::WHITE);
    Position pos(4, 4);  // e4 - center of board
    board.placePiece(std::move(bishop), pos);
    
    const Piece* piece = board.getPiece(pos);
    auto moves = piece->getPossibleMoves(pos, board);
    
    // Bishop in center should have 13 moves (diagonal lines)
    EXPECT_EQ(moves.size(), 13);
}

TEST_F(PieceTest, QueenMoves) {
    auto queen = std::make_unique<Queen>(PieceColor::WHITE);
    Position pos(4, 4);  // e4 - center of board
    board.placePiece(std::move(queen), pos);
    
    const Piece* piece = board.getPiece(pos);
    auto moves = piece->getPossibleMoves(pos, board);
    
    // Queen in center should have 27 moves (14 from rook + 13 from bishop)
    EXPECT_EQ(moves.size(), 27);
}

TEST_F(PieceTest, KingMoves) {
    auto king = std::make_unique<King>(PieceColor::WHITE);
    Position pos(4, 4);  // e4 - center of board
    board.placePiece(std::move(king), pos);
    
    const Piece* piece = board.getPiece(pos);
    auto moves = piece->getPossibleMoves(pos, board);
    
    // King in center should have 8 moves (one in each direction)
    EXPECT_EQ(moves.size(), 8);
}