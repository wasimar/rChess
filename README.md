# C++ Chess Game

A console-based chess game implementation in C++ featuring a clean, modular architecture designed for future network multiplayer expansion.

## Features

- **Complete Chess Implementation**: All standard chess rules including castling, en passant, and pawn promotion
- **Beautiful Console Display**: Unicode chess pieces with colored squares for enhanced visual experience
- **Interactive Gameplay**: Intuitive command system with comprehensive help
- **Move Validation**: Full rule enforcement with check and checkmate detection
- **Move History**: Complete game recording with undo functionality
- **Modular Architecture**: Clean separation of concerns for easy extension

## Quick Start

### Building the Game

```bash
# Compile the chess game
g++ -std=c++17 -Iinclude -o chess.out src/main.cpp src/core/*.cpp src/ui/*.cpp src/utils/*.cpp

# Compile the test program
g++ -std=c++17 -Iinclude -o tests/test_chess.out tests/test_chess.cpp src/core/*.cpp src/ui/*.cpp src/utils/*.cpp
```

### Running the Game

```bash
# Start interactive chess game
./chess.out

# Run test program
./tests/test_chess.out
```

## How to Play

### Basic Commands

- **Make a Move**: `e2e4` (from square to square)
- **Get Help**: `help` or `h`
- **Undo Move**: `undo` or `u`  
- **Offer Draw**: `draw` or `d`
- **Resign**: `resign` or `r`
- **Quit Game**: `quit` or `q`

### Square Notation

Squares are named using algebraic notation (a1-h8):
- **Files** (columns): a, b, c, d, e, f, g, h (left to right)
- **Ranks** (rows): 1, 2, 3, 4, 5, 6, 7, 8 (bottom to top)

### Special Moves

- **Castling**: Move king two squares toward rook (e.g., `e1g1` for kingside)
- **En Passant**: Capture pawn diagonally to empty square after opponent's two-square pawn move
- **Pawn Promotion**: Move pawn to last rank and select promotion piece (Q/R/B/N)

## Architecture

### Core Components

```
src/
├── core/           # Game logic and rules
│   ├── Board.cpp   # Chess board representation
│   ├── Piece.cpp   # Piece types and movement rules
│   ├── Move.cpp    # Move representation and validation
│   ├── Game.cpp    # Game state and rule enforcement
│   └── Player.cpp  # Player management
├── ui/             # User interface
│   ├── Display.cpp # Console rendering
│   └── InputParser.cpp # Command parsing
└── utils/          # Utility functions
    └── Utils.cpp   # Helper functions
```

### Key Classes

- **`Board`**: Manages 8x8 chess board state and piece positions
- **`Piece`**: Abstract base class for all chess pieces with movement validation
- **`Game`**: Main game controller handling turns, rules, and game state
- **`Move`**: Represents chess moves with special move support
- **`Display`**: Console rendering with Unicode symbols and colored squares
- **`InputParser`**: Command line input processing and validation

## Chess Rules Implemented

✅ **Basic Movement**: All piece types with correct movement patterns  
✅ **Capture Rules**: Standard capture mechanics  
✅ **Check Detection**: Automatic check detection and enforcement  
✅ **Checkmate**: Game end condition detection  
✅ **Stalemate**: Draw condition recognition  
✅ **Castling**: Kingside and queenside castling with all restrictions  
✅ **En Passant**: Special pawn capture rule  
✅ **Pawn Promotion**: Promotion to Queen, Rook, Bishop, or Knight  
✅ **Move Validation**: Complete rule enforcement preventing illegal moves  
✅ **Turn Management**: Proper alternation between players  

## Future Enhancements

This architecture is designed to support future network multiplayer functionality using gRPC:

- **Network Protocol**: Clean interfaces for message serialization
- **Player Abstraction**: Easy extension for remote players
- **Game State Sync**: Designed for distributed game state management
- **Latency Handling**: Architecture ready for network communication patterns

## Testing

The project includes comprehensive tests covering:

- Board state management
- Piece movement validation  
- Game rule enforcement
- Move history and undo functionality
- Check and checkmate detection

## Dependencies

- **C++17** compatible compiler
- **Standard Library** (no external dependencies for core game)
- **GoogleTest** (optional, for running unit tests)

## Development

The codebase follows modern C++ best practices:

- RAII resource management
- Smart pointer usage
- Const correctness
- Exception safety
- Clean architecture principles

## License

This project is designed for educational and development purposes, demonstrating clean C++ architecture and chess game implementation.