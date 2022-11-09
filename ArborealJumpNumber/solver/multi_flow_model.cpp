/*
 * multi_flow_model.cpp
 *
 *  Created on: Feb 23, 2022
 *      Author: evellyn
 */

#include "multi_flow_model.h"

namespace solver {

MultiFlowModel::MultiFlowModel() {
	this->type = solver::COMPACT;
}

MultiFlowModel::MultiFlowModel(ajns::instance& problem_instance_) {
	this->type = solver::COMPACT;
	this->problem_instance = problem_instance_;
}

IloNumVarArray MultiFlowModel::get_y_variables() {
	return y;
}

void MultiFlowModel::add_variables() {
	auto r = problem_instance.input_graph[problem_instance.root].id;
	auto n = this->problem_instance.num_vertices;
	
	x = IloNumVarArray(env, n * n * n);;
	for (auto i = 0u; i < n; i++) {
		for (auto j = 0u; j < n; j++) {
			for (auto k = 0u; k < n; k++) {
				x[i + n*j + k*n*n] = IloNumVar(env, 0.0, 1.0, ILOFLOAT);
/*				if (k == r) {
					x[i][j][k] = 0;
				}*/
			}
		}
	 }

	y = IloNumVarArray(env, n*n);
	for (auto i = 0u; i < n; i++) {
		for (auto j = 0u; j < n; j++) {
			y[n*i+j] = IloNumVar(env, 0, 1, ILOBOOL);
		}
	}
}

void MultiFlowModel::add_objective_function() {
	auto n = this->problem_instance.num_vertices;
	IloExpr cost_sum(env);
	for (const auto e : boost::make_iterator_range(boost::edges(problem_instance.covering_graph))) {
		auto i = problem_instance.covering_graph[e].source_id;
		auto j = problem_instance.covering_graph[e].target_id;
		cost_sum += y[n*i + j];
	}
	this->cplex_model.add(IloMaximize(env, cost_sum, "cost_sum"));
}

void MultiFlowModel::add_constraints() {
	auto r = problem_instance.input_graph[problem_instance.root].id;
	auto n = this->problem_instance.num_vertices;
	
	//	constraints #1
	for (auto v : boost::make_iterator_range(boost::vertices(problem_instance.input_graph))) {
		if (not problem_instance.input_graph[v].is_root) {
			IloExpr in_flow_sum_j(env);
			auto j = problem_instance.input_graph[v].id;
			my_graph::digraph::in_edge_iterator in_begin, in_end;
			for (boost::tie(in_begin, in_end) = boost::in_edges(v, problem_instance.input_graph);
				in_begin != in_end; ++in_begin) {
				auto u = boost::source(*in_begin, problem_instance.input_graph);
				auto i = problem_instance.input_graph[u].id;
				in_flow_sum_j += x[i + n*j + j*n*n];
			}
			cplex_model.add(in_flow_sum_j == 1);
		}
	}
	

	//	constraints #2
	for (auto v : boost::make_iterator_range(boost::vertices(problem_instance.input_graph))) {
		auto j = problem_instance.input_graph[v].id;
		for (auto k = 0u; k < n; k++) {
			if (k != r and k != j and j != r) {
				IloExpr in_flow_sum_k(env);
				IloExpr out_flow_sum_k(env);
									
				my_graph::digraph::in_edge_iterator in_begin, in_end;
				for (boost::tie(in_begin, in_end) = boost::in_edges(v, problem_instance.input_graph);
					in_begin != in_end; ++in_begin) {
					auto u = boost::source(*in_begin, problem_instance.input_graph);
					auto i = problem_instance.input_graph[u].id;
					in_flow_sum_k += x[i + n*j + n*n*k];
				}

				my_graph::digraph::out_edge_iterator out_begin, out_end;
				for (boost::tie(out_begin, out_end) = boost::out_edges(v, problem_instance.input_graph);
					out_begin != out_end; ++out_begin) {
					auto u = boost::target(*out_begin, problem_instance.input_graph);
					auto i = problem_instance.input_graph[u].id;

					out_flow_sum_k += x[j + i*n + k*n*n];
				}
				cplex_model.add(in_flow_sum_k == out_flow_sum_k);
			}

		}	
	}
	

	//	constraints #3
	
	/*my_graph::digraph::out_edge_iterator out_begin, out_end;
	for (auto k = 0u; k < n; k++) {
		if (k != r) {
			iloexpr flow_out_root(env);
			for (boost::tie(out_begin, out_end) = boost::out_edges(problem_instance.root, problem_instance.input_graph);
				out_begin != out_end; ++out_begin) {
				auto u = boost::target(*out_begin, problem_instance.input_graph);
				auto j = problem_instance.input_graph[u].id;
				flow_out_root += x[r][j][k];
			}
			cplex_model.add(flow_out_root == 1);
		}
	}*/

	//	constraints #4
	for (auto e : boost::make_iterator_range(boost::edges(problem_instance.input_graph))) {
		auto i = problem_instance.input_graph[e].source_id;
		auto j = problem_instance.input_graph[e].target_id;
		for (auto k = 0u; k < n; k++) {
			if (k != r)
				cplex_model.add(x[i + n*j + n*n*k] <= y[n*i + j]);
		}
	}

	//	constraints #5
	for (auto v : boost::make_iterator_range(boost::vertices(problem_instance.input_graph))) {
		if (not problem_instance.input_graph[v].is_root) {
			IloExpr source_vertices_sum(env);
			auto j = problem_instance.input_graph[v].id;
			my_graph::digraph::in_edge_iterator in_begin, in_end;
			for (boost::tie(in_begin, in_end) = boost::in_edges(v, problem_instance.input_graph);
				in_begin != in_end; ++in_begin) {
				auto u = boost::source(*in_begin, problem_instance.input_graph);
				auto i = problem_instance.input_graph[u].id;
				source_vertices_sum += y[n*i + j];
			}
			cplex_model.add(source_vertices_sum == 1);
		}
	}

	//	constraints #6
	for (auto e : boost::make_iterator_range(boost::edges(problem_instance.covering_graph))) {
		auto v = boost::source(e, problem_instance.input_graph);
		auto j = problem_instance.input_graph[v].id;
		if (j != r) {
			IloExpr reaches_k(env);
			auto k = problem_instance.input_graph[e].target_id;
			my_graph::digraph::in_edge_iterator in_begin, in_end;
			for (boost::tie(in_begin, in_end) = boost::in_edges(v, problem_instance.input_graph);
				in_begin != in_end; ++in_begin) {
				auto u = boost::source(*in_begin, problem_instance.input_graph);
				auto i = problem_instance.input_graph[u].id;
				reaches_k += x[i + n*j + n*n*k];
			}
			cplex_model.add(reaches_k == 1);
		}
	}
}

} /* namespace solver */

