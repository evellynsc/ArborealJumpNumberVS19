/*
 * model.cpp
 *
 *  Created on: 19 de abr de 2021
 *      Author: evellyn
 */

#include "exponential_model.h"

namespace solver {

ExponentialModel::ExponentialModel() {
	this->type = CUTSET_EXP;
}

ExponentialModel::ExponentialModel(ajns::instance& problem_instance_, model_type type_) {
	this->type = type_;
	this->problem_instance = problem_instance_;
}

void ExponentialModel::add_variables() {
	this->x = IloBoolVarArray(this->env, this->problem_instance.num_edges);
}


void ExponentialModel::add_constraints() {
	if (type == CUTSET_EXP) {
		add_number_of_edges_constraints();
		add_limit_indegree_constraints();
		add_cutset_constraints();
	}
	if (type == RELAXED_CUTSET) {
		add_number_of_edges_constraints();
		add_limit_indegree_constraints();
	}
	add_out_edges_constraints();
}

void ExponentialModel::add_objective_function() {
	IloExpr minimize_jumps(env);
	std::cout << problem_instance.num_edges << std::endl;
	for (auto e : boost::make_iterator_range(
			boost::edges(problem_instance.input_graph))) {
		if (problem_instance.input_graph[e].type == my_graph::ARTIFICIAL) {
			minimize_jumps += x[problem_instance.input_graph[e].id];
		}
	}
	this->cplex_model.add(IloMinimize(env, minimize_jumps));
	minimize_jumps.end();
}

void ExponentialModel::add_limit_indegree_constraints() {
	IloExprArray indegree_limits_constraint(env, problem_instance.num_vertices);
	std::vector<bool> lhs_expr(problem_instance.num_vertices, false);
	for (auto i = 0u; i < problem_instance.num_vertices; i++)
		indegree_limits_constraint[i] = IloExpr(env);
	for (auto e : boost::make_iterator_range(
			boost::edges(problem_instance.input_graph))) {
		auto target_node = problem_instance.input_graph[e].target_id;
		indegree_limits_constraint[target_node] +=
				x[problem_instance.input_graph[e].id];
		lhs_expr[target_node] = true;
	}
	for (auto i = 0u; i < problem_instance.num_vertices; i++) {
		if (lhs_expr[i])
			cplex_model.add(indegree_limits_constraint[i] <= 1);
		indegree_limits_constraint[i].end();
	}
	indegree_limits_constraint.end();
}

void ExponentialModel::add_number_of_edges_constraints() {
	IloExpr number_of_arcs_selected(env);
	for (auto i = 0u; i < problem_instance.num_edges; i++) {
		number_of_arcs_selected += x[i];
	}
	cplex_model.add(
			number_of_arcs_selected == problem_instance.num_vertices - 1);
	number_of_arcs_selected.end();
}

void ExponentialModel::add_out_edges_constraints() {
	my_graph::out_edge_itr ei, ei_end;
	for (auto v : boost::make_iterator_range(boost::vertices(problem_instance.input_graph))) {
		if (boost::out_degree(v, problem_instance.covering_graph) > 0) {
			auto vertex_set = std::set<my_graph::vertex>();
			IloExpr out_edges_exp(env);
			for (boost::tie(ei, ei_end) = out_edges(v, problem_instance.input_graph); ei != ei_end; ++ei) {
				out_edges_exp += x[problem_instance.input_graph[*ei].id];
			}
			cplex_model.add(out_edges_exp >= 1);
		}
	}
}

void ExponentialModel::add_cutset_constraints() {
	for (auto powerset = 1;
			powerset < std::pow(2, problem_instance.num_vertices); powerset++) {
		IloExpr extension_constraint(env);
		auto add_expr = false;
		for (auto e : boost::make_iterator_range(
				boost::edges(problem_instance.input_graph))) {
			auto head = problem_instance.input_graph[e].source_id;
			auto tail = problem_instance.input_graph[e].target_id;

			if (in_the_set(powerset, head) && not in_the_set(powerset, tail)) {
				if (problem_instance.input_graph[e].type == my_graph::ORIGINAL) {
					add_expr = true;
				}
				extension_constraint += x[problem_instance.input_graph[e].id];
			}
		}
		if (add_expr)
			cplex_model.add(extension_constraint >= 1);
		extension_constraint.end();
	}
}


bool ExponentialModel::in_the_set(unsigned set_, std::size_t element_) {
	return (set_ & (1 << element_));
}

IloBoolVarArray ExponentialModel::get_variables_x() {
	return this->x;
}

auto ExponentialModel::get_variables(int which) {
	return this->x;
}


//ExponentialModel::~ExponentialModel() {
////	env.end();
////	cplex_model.end();
//}

} /* namespace ajns */
