cmake_minimum_required(VERSION 3.0)
project(gambit)

set(GAMBIT_SOURCES
    src/core/dvector.cc
    src/core/matrix.cc
    src/core/pvector.cc
    src/core/sqmatrix.cc
    src/core/vector.cc

    src/linalg/basis.cc
    src/linalg/btableau.cc
    src/linalg/lemketab.cc
    src/linalg/lhtab.cc
    src/linalg/lpsolve.cc
    src/linalg/lptab.cc
    src/linalg/ludecomp.cc
    src/linalg/tableau.cc

)

include_directories(
	src
	src/core
)

add_library(gambit SHARED ${GAMBIT_SOURCES})

# add_executable(enummixed src/tools/enummixed/enummixed.cc)
# target_link_libraries(enummixed gambit)

add_executable(main main.cc)
target_link_libraries(main gambit)