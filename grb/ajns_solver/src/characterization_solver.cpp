#include "headers/characterization_solver.h"

#include "headers/instance.h"

void CharacterizationSolver::add_constraints()
{
    
}

void CharacterizationSolver::add_variables()
{
    int s = instance.s;
    int n = instance.n;
    int s1 = s + 1;
    int ns = n * s1;
    int ss = s1 * s1;

    index_parser_ns = std::vector<std::vector<int>>(n);
    index_parser_m = std::vector<std::vector<int>>(n);
    index_parser_ss = std::vector<std::vector<int>>(s1);

    x = model.addVars(ns, GRB_BINARY);
    r = model.addVars(ns, GRB_BINARY);
    f = model.addVars(ns, GRB_BINARY);
    g = model.addVars(ns, GRB_BINARY);
}