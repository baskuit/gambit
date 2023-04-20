//
// This file is part of Gambit
// Copyright (c) 1994-2022, The Gambit Project (http://www.gambit-project.org)
//
// FILE: src/tools/enummixed/enummixed.cc
// Compute Nash equilibria via Mangasarian's algorithm
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <fstream>

#include "gambit.h"
#include "solvers/enummixed/enummixed.h"

using namespace Gambit;
using namespace Gambit::Nash;

template <class T>
void PrintCliques(const List<List<MixedStrategyProfile<T>>> &p_cliques,
                  std::shared_ptr<StrategyProfileRenderer<T>> p_renderer)
{
  for (int cl = 1; cl <= p_cliques.size(); cl++)
  {
    for (int i = 1; i <= p_cliques[cl].size(); i++)
    {
      p_renderer->Render(p_cliques[cl][i],
                         "convex-" + lexical_cast<std::string>(cl));
    }
  }
}
#include <chrono>
int main(int argc, char *argv[])
{

  int numDecimals = 6;

  std::istream *input_stream = &std::cin;
  std::ifstream file_stream;

  file_stream.open("contrib/games/8x8.nfg");
  input_stream = &file_stream;

  Game game = ReadGame(*input_stream);

  std::shared_ptr<StrategyProfileRenderer<double>> renderer(
      new MixedStrategyCSVRenderer<double>(std::cout, numDecimals));
  EnumMixedStrategySolver<double> solver(renderer);

  auto start = std::chrono::high_resolution_clock::now();
  std::shared_ptr<EnumMixedStrategySolution<double>> solution =
      solver.SolveDetailed(game);
  auto end = std::chrono::high_resolution_clock::now();
  double elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1e6;
  std::cout << "Elapsed time: " << elapsed << " seconds\n";

  List<List<MixedStrategyProfile<double>>> cliques =
      solution->GetCliques();
  PrintCliques(cliques, renderer);

  return 0;
}
