/*
 * solver.cpp
 *
 *  Created on: 22 de fev de 2021
 *      Author: evellyn
 */

#include "solver.h"

namespace solver {
solver::~solver() {
	cplex_solver.end();
}

solver::solver() {
	num_jumps = 0;
	this->status = -1;
}

solver::solver(solver_params &solver_config,  Model* model) {
	this->config = solver_config;
	this->num_jumps = 0;
	cplex_solver = IloCplex(model->get_cplex_model());
	this->status = -1;
}

void solver::setup_cplex() {
// https://www.ibm.com/docs/ru/icos/20.1.0?topic=parameters-mip-dynamic-search-switch
//	cplex_solver.setParam(IloCplex::Param::Preprocessing::Presolve, CPX_ON);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::HeuristicFreq, CPX_ON);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::RINSHeur, CPX_ON);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::FPHeur, CPX_ON);
//	cplex.setParam(IloCplex::Param::Preprocessing::Linear, 0);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::Search, CPX_MIPSEARCH_TRADITIONAL);
	cplex_solver.setParam(IloCplex::Param::Threads, 4);
	cplex_solver.setParam(IloCplex::Param::TimeLimit, config.time_limit);
	cplex_solver.setParam(IloCplex::Param::MIP::Limits::TreeMemory, config.tree_memory);
	cplex_solver.setParam(IloCplex::Param::Emphasis::MIP, CPX_MIPEMPHASIS_FEASIBILITY);
}
//
//

void solver::solve(ajns::properties &p) {
	std::cout << "solver.cpp\n";
	solve();
	p.num_jumps = num_jumps;
}

vector<double> solver::get_values_main_variables() {
	std::cout << "solver::get_values_main_variables\n";
	return vector<double>(0);
}

int solver::get_status() {
	return status;
}


}/* namespace ajns */
