/*
 * solver.h
 *
 *  Created on: 22 de fev de 2021
 *      Author: evellyn
 */

#ifndef SOLVER_SOLVER_H_
#define SOLVER_SOLVER_H_


#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "model.h"
#include "solver_params.h"
#include "../base/properties.h"
#include "../base/solution.h"

namespace solver {
class solver {
protected:
	solver_params config;
	IloCplex cplex_solver;
	std::size_t num_jumps; // mudar aqui pra double
	Solution* solution;
	Model* model;
	int status;

public:
	solver();
	solver(std::string, ajns::instance&, bool, std::vector<bool>);
	solver(solver_params&, Model*);
/* 	solver(solver_params&, Model&); */
	void setup_cplex();
	void solve(ajns::properties &p);
	int get_status();
	virtual void solve() = 0;

	virtual void set_model(std::vector<bool>);
	virtual vector<double> get_values_main_variables();
	virtual ~solver() = 0;
};

} /* namespace solver */

#endif /* SOLVER_SOLVER_H_ */
