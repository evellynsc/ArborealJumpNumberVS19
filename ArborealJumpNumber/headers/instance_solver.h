#pragma once

#include <iostream>
#include <fstream>
#include "answer.h"
#include "instance.h"
#include "exact_solver.h"

class InstanceSolver {
public:
    static InstanceSolver GetInstance(std::string path_file_name, string formulation, int s);

    Answer Solve(string file_output_name, int time_limit, double heuristics) const;
    void PrintBest();

private:
    InstanceSolver(ExactSolver* solver)
        : instance(&solver->instance), solver(solver) {}

    const Instance* instance;
    ExactSolver* solver;

    /*vector<int> best_;
    int generated_;
    int active_;*/
};