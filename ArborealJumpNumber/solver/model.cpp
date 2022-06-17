/*
 * model.cpp
 *
 *  Created on: Feb 22, 2022
 *      Author: evellyn
 */


/*
 * model.cpp
 *
 *  Created on: 19 de abr de 2021
 *      Author: evellyn
 */

#include "model.h"

namespace solver {

Model::Model() {
	type = NOT_DEFINED;
	env = IloEnv();
	cplex_model = IloModel(env);
}

IloEnv Model::get_cplex_env() {
	return this->env;
}

ajns::instance Model::get_ajnp_instance() {
	return this->problem_instance;
}

model_type Model::get_type() {
	return this->type;
}

IloModel Model::get_cplex_model() {
	return (cplex_model);
}

void Model::create() {
	add_variables();
	add_objective_function();
	add_constraints();
}

Model::~Model() {
	std::cout << "desalocando o modelo\n";
	cplex_model.end();
	env.end();
	std::cout << "acabou desalocando o modelo\n";
}

} /* namespace solver */


