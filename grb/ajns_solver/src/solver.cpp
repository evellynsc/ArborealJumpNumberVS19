#include "headers/solver.h"

void FeasibilitySolver::create_variables()
{
  const double infinity = solver->infinity();
  // x[j] is an array of non-negative, integer variables.
  std::vector<const MPVariable *> x(data.num_vars);
  for (int j = 0; j < data.num_vars; ++j)
  {
    x[j] = solver->MakeIntVar(0.0, infinity, "");
  }
  LOG(INFO) << "Number of variables = " << solver->NumVariables();

  auto m = data.m;
  auto j1 = jumps++;
  auto n_j1 = data.n * j1;
  auto j1_j1 = j1 * j1;

  index_parser_ns = vector<vector<int>>(problem_instance.num_vertices);
  index_parser_m = vector<vector<int>>(problem_instance.num_vertices);
  index_parser_s2 = vector<vector<int>>(s1);
}
// https://ideone.com/ATyAyp
void operations_research::MipVarArray()
{
  DataModel data;

  // Create the mip solver with the SCIP backend.
  std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));
  if (!solver)
  {
    LOG(WARNING) << "SCIP solver unavailable.";
    return;
  }

  const double infinity = solver->infinity();
  // x[j] is an array of non-negative, integer variables.
  std::vector<const MPVariable *> x(data.num_vars);
  for (int j = 0; j < data.num_vars; ++j)
  {
    x[j] = solver->MakeIntVar(0.0, infinity, "");
  }
  LOG(INFO) << "Number of variables = " << solver->NumVariables();

  // Create the constraints.
  for (int i = 0; i < data.num_constraints; ++i)
  {
    MPConstraint *constraint = solver->MakeRowConstraint(0, data.bounds[i], "");
    for (int j = 0; j < data.num_vars; ++j)
    {
      constraint->SetCoefficient(x[j], data.constraint_coeffs[i][j]);
    }
  }
  LOG(INFO) << "Number of constraints = " << solver->NumConstraints();

  // Create the objective function.
  MPObjective *const objective = solver->MutableObjective();
  for (int j = 0; j < data.num_vars; ++j)
  {
    objective->SetCoefficient(x[j], data.obj_coeffs[j]);
  }
  objective->SetMaximization();

  const MPSolver::ResultStatus result_status = solver->Solve();

  // Check that the problem has an optimal solution.
  if (result_status != MPSolver::OPTIMAL)
  {
    LOG(FATAL) << "The problem does not have an optimal solution.";
  }
  LOG(INFO) << "Solution:";
  LOG(INFO) << "Optimal objective value = " << objective->Value();

  for (int j = 0; j < data.num_vars; ++j)
  {
    LOG(INFO) << "x[" << j << "] = " << x[j]->solution_value();
  }
}