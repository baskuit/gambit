cmake_minimum_required(VERSION 3.0)
project(gambit)

set(GAMBIT_SOURCES
    src/core/dvector.cc
    src/core/function.cc
    src/core/integer.cc
    src/core/matrix.cc
    src/core/pvector.cc
    src/core/rational.cc
    src/core/sqmatrix.cc
    src/core/tinyxml.cc
    src/core/tinyxmlerror.cc
    src/core/tinyxmlparser.cc
    src/core/vector.cc

    src/games/behav.cc
    src/games/behavitr.cc
    src/games/behavspt.cc
    src/games/file.cc
    src/games/game.cc
    src/games/gameagg.cc
    src/games/gamebagg.cc
    src/games/gametable.cc
    src/games/gametree.cc
    src/games/mixed.cc
    src/games/nash.cc
    src/games/stratitr.cc
    src/games/stratspt.cc
    # src/games/writer.cc

    src/games/agg/agg.cc
    src/games/agg/bagg.cc

    src/solvers/linalg/basis.cc
    src/solvers/linalg/btableau.cc
    src/solvers/linalg/lemketab.cc
    src/solvers/linalg/lhtab.cc
    src/solvers/linalg/lpsolve.cc
    src/solvers/linalg/lptab.cc
    src/solvers/linalg/ludecomp.cc
    src/solvers/linalg/tableau.cc

    src/solvers/enummixed/clique.cc
    src/solvers/enummixed/enummixed.cc
)

include_directories(
	src
	src/core
	src/games
	src/games/agg
	src/solvers
	src/lrs
)

add_library(gambit SHARED ${GAMBIT_SOURCES})

add_executable(enummixed src/tools/enummixed/enummixed.cc)
target_link_libraries(enummixed gambit)

add_executable(main main.cc)
target_link_libraries(main gambit)