#pragma once
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cassert>

enum EdgeType { INPUT, ARTIFICIAL };
enum FileExtension { DOT, TXT, MTX, PR };
enum SolverEnum { GUROBI, CPLEX, HIGHS };

// long unsigned get_index_d2(std::pair<const long unsigned, const long unsigned>, std::pair<const long unsigned, const long unsigned>);
// long unsigned get_index_d3(std::pair<const long unsigned, const long unsigned>, std::pair<const long unsigned, const long unsigned>, std::pair<const long unsigned, const long unsigned>);
// long unsigned get_index_d4(std::pair<const long unsigned, const long unsigned>, std::pair<const long unsigned, const long unsigned>, std::pair<const long unsigned, const long unsigned>, std::pair<const long unsigned, const long unsigned>);

inline long unsigned get_index_d2(std::pair<const long unsigned, const long unsigned> x, std::pair<const long unsigned, const long unsigned> y)
{
    assert(x.first < x.second);
    assert(y.first < y.second);
    
    return (x.first*y.second + y.first);
}

inline long unsigned get_index_d3(std::pair<const long unsigned, const long unsigned> x, std::pair<const long unsigned, const long unsigned> y, std::pair<const long unsigned, const long unsigned> z)
{
    assert(x.first < x.second);
    assert(y.first < y.second);
    assert(z.first < z.second);
    
    return (x.first*y.second*z.second + y.first*z.second + z.first);
}

inline long unsigned get_index_d4(std::pair<const long unsigned, const long unsigned> x, std::pair<const long unsigned, const long unsigned> y, std::pair<const long unsigned, const long unsigned> z, std::pair<const long unsigned, const long unsigned> w)
{
    assert(x.first < x.second);
    assert(y.first < y.second);
    assert(z.first < z.second);
    assert(w.first < w.second);

    return (x.first*y.second*z.second*w.second + y.first*z.second*w.second + z.first*w.second + w.first);
}