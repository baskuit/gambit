cmake_minimum_required(VERSION 3.0)
project(gambit)

set(GAMBIT_SOURCES
    src/core/matrix.cc
    src/core/vector.cc

    src/linalg/basis.cc
    src/linalg/btableau.cc
    src/linalg/lpsolve.cc
    src/linalg/lptab.cc
    src/linalg/ludecomp.cc
    src/linalg/tableau.cc

)

include_directories(
	src
)

add_library(gambit SHARED ${GAMBIT_SOURCES})

add_executable(main main.cc)
target_link_libraries(main gambit)