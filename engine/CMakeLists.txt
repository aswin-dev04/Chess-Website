cmake_minimum_required(VERSION 3.16)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SFML 3.x manual paths
include_directories(/usr/include)
link_directories(/usr/lib)

set(SOURCES
    gui/gui.cpp
    src/board.cpp
    src/move.cpp
    src/movegen.cpp
    src/utils.cpp
    src/tables.cpp    
    src/ai.cpp
    src/evaluation.cpp
    src/moveorder.cpp
    src/magic.cpp
    src/zobrist.cpp
    src/TT.cpp
)

add_executable(chess_gui ${SOURCES})
target_compile_options(chess_gui PRIVATE -O3)

target_link_libraries(chess_gui
    sfml-graphics
    sfml-window
    sfml-system
)

