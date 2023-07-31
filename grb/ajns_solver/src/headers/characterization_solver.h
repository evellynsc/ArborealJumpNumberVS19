#pragma once

#include "gurobi_c++.h"
#include "solver.h"
#include "headers/instance.h"

class CharacterizationSolver : public Solver
{
public:
    CharacterizationSolver(Instance _instance) : Solver(_instance) {

    }

    ~CharacterizationSolver() {}
    void add_constraints();
    void add_variables();
    void solve(int timeLimit, double heuristics);

    // void SetInitialSolution();
    // void AddConstraintsAndObjective();

private:
    GRBVar *x;
    GRBVar *r;
    GRBVar *f;
    GRBVar *g;

    GRBVar **a;
    GRBVar **h;
    GRBVar **w;

    std::vector<std::vector<int>> index_parser_ns;
    std::vector<std::vector<int>> index_parser_m;
    std::vector<std::vector<int>> index_parser_ss;

    int s;
};