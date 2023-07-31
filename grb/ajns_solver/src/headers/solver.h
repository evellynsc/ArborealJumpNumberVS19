#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "ortools/linear_solver/linear_solver.h"
#include "instance.h"


namespace operations_research {
struct DataModel {
  const std::vector<std::vector<double>> constraint_coeffs{
      {5, 7, 9, 2, 1},
      {18, 4, -9, 10, 12},
      {4, 7, 3, 8, 5},
      {5, 13, 16, 3, -7},
  };
  const std::vector<double> bounds{250, 285, 211, 315};
  const std::vector<double> obj_coeffs{7, 8, 2, 9, 6};
  const int num_vars = 5;
  const int num_constraints = 4;
};

void MipVarArray();
}  // namespace operations_research

// int main(int argc, char** argv) {
//   operations_research::MipVarArray();
//   return EXIT_SUCCESS;
// }



// =================== GUROBI ===============
// #include "gurobi_c++.h"
// #include <CXXGraph/CXXGraph.hpp>

// class Solver {
//     void InitModel() {
//         model_.set(GRB_IntParam_OutputFlag, 0);
//         model_.set(GRB_IntParam_LazyConstraints, 1);
//         model_.set(GRB_IntParam_Presolve, 0);
//         model_.set(GRB_IntParam_PreCrush, 1);
//         model_.set(GRB_IntParam_Threads, 1);
//         model_.set(GRB_IntParam_Cuts, 0);
//     }
// public:
//     // static const int kInf = (1 << 29);
//     // static const double kHeuristicOp;
//     // static const int kTimeLimitOp;
//     const std::string kFormulationName;

//     Solver(Instance _instance)
//         : instance(_instance), env(GRBEnv()), model(GRBModel(env_)) {
//         InitModel();
//     }
//     virtual ~Solver() {}

//     virtual void solve(int timeLimit, double heuristics) = 0;
//     virtual void add_constraints() = 0;
//     virtual void add_variables() = 0;
//     // virtual void SetInitialSolution() = 0;
//     // virtual void AddConstraintsAndObjective() = 0;

//     // static double GetUpper(GRBModel &model_);
//     // static double GetLower(GRBModel &model_);
//     // static double GetOptimalityGap(GRBModel &model_);

//     Instance instance;

// protected:
//     GRBEnv env;
//     GRBModel model;    
// };