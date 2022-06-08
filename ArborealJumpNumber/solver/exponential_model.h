/*
 * model.h
 *
 *  Created on: 19 de abr de 2021
 *      Author: evellyn
 */

#ifndef SOLVER_EXPONENTIAL_MODEL_H_
#define SOLVER_EXPONENTIAL_MODEL_H_

#include "model.h"
#include "../base/instance.h"
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN


namespace solver {
class ExponentialModel : public Model {
//	model_type type;
//	ajns::instance problem_instance;
//	IloEnv env;
//	IloModel cplex_model;
	IloBoolVarArray x;
	bool in_the_set(unsigned, std::size_t);
	void add_objective_function();
	void add_variables();
	void add_constraints();
	void add_number_of_edges_constraints();
	void add_limit_indegree_constraints();
	void add_cutset_constraints();

public:
	ExponentialModel();
	ExponentialModel(ajns::instance&);
	IloBoolVarArray get_variables_x();
	auto get_variables(int);
	virtual ~ExponentialModel() = default;
};

} /* namespace ajns */



#endif /* SOLVER_EXPONENTIALMODEL_H_ */
