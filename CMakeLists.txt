cmake_minimum_required(VERSION 3.16)
project(ChessGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

if(MSVC)
    add_compile_options(/W4 /WX)
endif()

include_directories(include)

set(CORE_SOURCES
    src/core/Board.cpp
    src/core/Piece.cpp
    src/core/Move.cpp
    src/core/Game.cpp
    src/core/Player.cpp
)

set(UI_SOURCES
    src/ui/Display.cpp
    src/ui/InputParser.cpp
)

set(UTIL_SOURCES
    src/utils/Utils.cpp
)

set(NETWORK_SOURCES
    src/network/NetworkManager.cpp
)

add_executable(chess
    src/main.cpp
    ${CORE_SOURCES}
    ${UI_SOURCES}
    ${UTIL_SOURCES}
    ${NETWORK_SOURCES}
)

find_package(Threads REQUIRED)
target_link_libraries(chess Threads::Threads)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(chess PRIVATE DEBUG_MODE)
endif()

enable_testing()

add_subdirectory(tests)