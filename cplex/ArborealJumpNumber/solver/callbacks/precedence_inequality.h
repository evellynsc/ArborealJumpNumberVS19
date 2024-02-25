/*
 * precedence_inequality.h
 *
 *  Created on: 11 de mai de 2021
 *      Author: evellyn
 */

#ifndef SOLVER_CALLBACKS_PRECEDENCE_INEQUALITY_H_
#define SOLVER_CALLBACKS_PRECEDENCE_INEQUALITY_H_

#include <string>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "../../base/instance.h"
#include "../../utils/callbacks.h"
#include "../../algorithms/flow.h"

#include "boost/graph/transpose_graph.hpp"

using namespace solver;

ILOUSERCUTCALLBACK2(separate_precedence_inequalities, IloBoolVarArray, x, ajns::instance&, problem_instance) {
	if (!isAfterCutLoop())
		return;

	//TODO: make a map<vertex, vector<bool>> to get successor and predecessor
	auto get_adjacent_vertices_bool = [](std::set<my_graph::vertex> set_s,
			my_graph::digraph &graph) {
		auto num_vertices = boost::num_vertices(graph);
		auto adj = std::vector<bool>(num_vertices, false);
		for (auto u : set_s) {
			auto neighboors = boost::adjacent_vertices(u, graph);
			for (auto n : boost::make_iterator_range(neighboors)) {
				adj[graph[n].id] = true;
			}
		}
		return adj;
	};

//	std::cout << "separate_precedence_inequalities" << std::endl;
	IloEnv env = getEnv();
	IloNumArray x_values(env);
	getValues(x_values, x);

	auto graph_x = callback::construct_graph_from_x_assigment(
			problem_instance.input_graph, x_values);

	auto graph_pi = my_graph::digraph();
	boost::copy_graph(graph_x, graph_pi);
	auto sink = my_graph::vertex();
	callback::add_universal_sink(graph_pi, sink);

	auto graph_sigma = my_graph::digraph();
	boost::copy_graph(graph_x, graph_sigma);

	auto graph_pi_sigma = my_graph::digraph();
	boost::copy_graph(graph_x, graph_pi_sigma);

	for (auto v : boost::make_iterator_range(
			boost::vertices(problem_instance.input_graph))) {
		if (not problem_instance.input_graph[v].is_maximum) {
			auto copy_graph_pi = my_graph::digraph();
			boost::copy_graph(graph_pi, copy_graph_pi);
			callback::reset_edges_of_vertice(v, copy_graph_pi,
					problem_instance.t_order_graph);
			auto graph_pi_flow = ajns::flow(copy_graph_pi);
			graph_pi_flow.run(v, sink, ajns::PUSREL);
			if (graph_pi_flow.get_max_flow_value() < 1) {
				auto min_cut = graph_pi_flow.get_min_cut();

				auto set_s = graph_pi_flow.get_set_s();
				auto pred_set_s = get_adjacent_vertices_bool(set_s,
						problem_instance.t_order_graph);
				callback::strengthen_cut(min_cut, pred_set_s, graph_pi);

				if (min_cut.size()) {
					auto lhside_ineq = IloExpr(env);
					callback::construct_lhside_of_inequality(
							problem_instance.input_graph, min_cut, x,
							lhside_ineq);
					add(lhside_ineq >= 1);
					lhside_ineq.end();
				}
			}
		}

		if (not problem_instance.input_graph[v].is_root) {
			my_graph::digraph copy_graph_sigma;
			boost::copy_graph(graph_sigma, copy_graph_sigma);
			callback::reset_edges_of_vertice(v, copy_graph_sigma,
					problem_instance.order_graph);
			auto graph_sigma_flow = ajns::flow(copy_graph_sigma);
			graph_sigma_flow.run(problem_instance.root, v, ajns::PUSREL);
			if (graph_sigma_flow.get_max_flow_value() < 1) {
				auto min_cut = graph_sigma_flow.get_min_cut();
				auto set_bar_s = graph_sigma_flow.get_set_bar_s();
				auto succ_bar_s = get_adjacent_vertices_bool(set_bar_s,
						problem_instance.order_graph);
				callback::strengthen_cut(min_cut, succ_bar_s, graph_sigma);
				if (min_cut.size()) {
					auto lhside_ineq = IloExpr(env);
					callback::construct_lhside_of_inequality(
							problem_instance.input_graph, min_cut, x,
							lhside_ineq);
					add(lhside_ineq >= 1);
					lhside_ineq.end();
				}
			}
		}
	}

	for (auto e : boost::make_iterator_range(
			boost::edges(problem_instance.covering_graph))) {
		auto head = boost::source(e, problem_instance.covering_graph);
		auto tail = boost::target(e, problem_instance.covering_graph);
		my_graph::digraph copy_graph_x;
		boost::copy_graph(graph_x, copy_graph_x);
		if (not graph_x[head].is_root) {
			callback::reset_edges_of_vertice(head, copy_graph_x,
					problem_instance.order_graph);
			callback::reset_edges_of_vertice(tail, copy_graph_x,
					problem_instance.t_order_graph);
			auto graph_flow = ajns::flow(copy_graph_x);
			graph_flow.run(head, tail, ajns::PUSREL);
			//				callback::strengthen_cut(min_cut, succ_bar_s, graph_sigma);
			if (graph_flow.get_max_flow_value() < 1) {
				auto min_cut = graph_flow.get_min_cut();
				if (min_cut.size()) {
					auto lhside_ineq = IloExpr(env);
					callback::construct_lhside_of_inequality(
							problem_instance.input_graph, min_cut, x,
							lhside_ineq);
					add(lhside_ineq >= 1);
					lhside_ineq.end();
				}
			}
		}
	}

}

#endif /* SOLVER_CALLBACKS_PRECEDENCE_INEQUALITY_H_ */
