find_package(GTest)

if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50f33f9afd5f446491b3f8e5f9b5f6.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(chess_tests
    test_board.cpp
    test_pieces.cpp
    test_game_logic.cpp
    ../src/core/Board.cpp
    ../src/core/Piece.cpp
    ../src/core/Move.cpp
    ../src/core/Game.cpp
    ../src/core/Player.cpp
    ../src/utils/Utils.cpp
)

target_link_libraries(chess_tests gtest_main)
target_include_directories(chess_tests PRIVATE ../include)

include(GoogleTest)
gtest_discover_tests(chess_tests)