/*
 * model.h
 *
 *  Created on: Feb 21, 2022
 *      Author: evellyn
 */

#ifndef SOLVER_MODEL_H_
#define SOLVER_MODEL_H_

#include "../base/instance.h"
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

namespace solver {
enum model_type { NOT_DEFINED, CUTSET_EXP, RELAXED_CUTSET, COMPACT, DDL};

class Model {

protected:
	bool linear_relaxation;
	model_type type;
	IloEnv env;
	IloModel cplex_model;
	ajns::instance problem_instance;

	virtual void add_variables() = 0;
	virtual void add_objective_function() = 0;
	virtual void add_constraints() = 0;


public:
	Model();
	virtual ~Model();

	void set_model_type(model_type);
	void set_env(IloEnv&);
	void set_cplex_model(IloModel&);
	void set_problem_instance(ajns::instance&);
	IloModel get_cplex_model();
	IloEnv get_cplex_env();
	ajns::instance get_ajnp_instance();
	model_type get_type();
	void create();
};
};



#endif /* SOLVER_MODEL_H_ */
