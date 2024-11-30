#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "instance.h"
#include "global.h"
// #include "absl/flags/flag.h"
// #include "absl/log/flags.h"
#include "ortools/base/init_google.h"
#include "ortools/base/logging.h"
#include "ortools/linear_solver/linear_solver.h"

namespace operations_research {

class FeasibilitySolver
{
    const std::string name;
    const std::unique_ptr<Instance> data; 
    long unsigned nparts;
    static std::unordered_map<SolverEnum, const std::string> solver_map;
    // void InitSolver();

    // x_{it} binary variable which value is one if vertex i ∈ V belongs to part t, zero, otherwise.
    std::vector<const MPVariable*> x;
    // r_{it} binary variable which value is one if vertex i ∈ V is the root of part t, zero, otherwise.
    std::vector<const MPVariable*> r;
    // f_{it} binary variable which value is one if f (πt) = i, zero, otherwise.
    std::vector<const MPVariable*> f;
    // g_{it} binary variable which value is one if i ∈ g(πt), zero, otherwise.
    std::vector<const MPVariable*> g;
    // a_{ijt} binary variable which value is one if arc (i, j) ∈ R− belongs to part πt, zero, otherwise.
    std::vector<const MPVariable*> a;
    // h_{ijtu} binary variable which value is one if fju, xjt and git, t < u, are also one, zero, otherwise. h_{ijtu} = f_{ju}x_{jt}g_{it}.
    std::vector<const MPVariable*> h;
    // w_{itu} binary variable which value is one if g_{iu} and x_{iu}, t < u, are also one, zero, otherwise. w_{itu} = x_{it}g_{iu}.
    std::vector<const MPVariable*> w;

    std::vector<MPConstraint*> constraints;

public:
    
    FeasibilitySolver(std::string _name, std::unique_ptr<Instance> _data, long unsigned _nparts, SolverEnum _solver)
    : name(_name), data(std::move(_data)), nparts(_nparts)
    {
        // TODO: fix this turnaround!!
        std::unique_ptr<MPSolver> teste(MPSolver::CreateSolver(solver_map[_solver]));
        solver = std::move(teste);
        // solver = MPSolver::CreateSolver(solver_map[_solver]);
        solver->EnableOutput();
        if (!solver) {
            LOG(WARNING) << solver_map[_solver] << " solver unavailable." << std::endl;
            return;
        }
    }
    ~FeasibilitySolver()=default;
    void create_model();
    bool solve_model();

protected:
    std::unique_ptr<MPSolver> solver;
    void create_variables();
    void create_objective();
    void create_constraints();
    
};
} // namespace operations_research
