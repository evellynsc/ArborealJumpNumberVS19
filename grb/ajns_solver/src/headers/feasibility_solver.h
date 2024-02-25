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
    void InitSolver();

    std::vector<const MPVariable*> x;
    std::vector<const MPVariable*> r;
    std::vector<const MPVariable*> f;
    std::vector<const MPVariable*> g;
    std::vector<const MPVariable*> a;
    std::vector<const MPVariable*> h;
    std::vector<const MPVariable*> w;

    std::vector<MPConstraint*> constraints;

    // std::vector<std::vector<const MPVariable*>> a;
    // std::vector<std::vector<const MPVariable*>> h;
    // std::vector<std::vector<const MPVariable*>> w;

    // // vector that parses a integer to 
    // vector<vector<int>> idx_vct_nj;
    // vector<vector<int>> index_parser_m;
    // vector<vector<int>> index_parser_s2;



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
    void solve_model();

protected:
    std::unique_ptr<MPSolver> solver;
    void create_variables();
    void create_objective();
    void create_constraints();
    
};
} // namespace operations_research
