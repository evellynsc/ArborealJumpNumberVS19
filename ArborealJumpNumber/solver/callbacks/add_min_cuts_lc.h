/*
 * add_min_cuts_lc.h
 *
 *  Created on: 20 de abr de 2021
 *      Author: evellyn
 */

#ifndef SOLVER_CALLBACKS_ADD_MIN_CUTS_LC_H_
#define SOLVER_CALLBACKS_ADD_MIN_CUTS_LC_H_

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "../../base/instance.h"
#include "../../utils/callbacks.h"
#include "../../algorithms/flow.h"
#include <numeric>

#include <boost/graph/transitive_closure.hpp>

#include <map>

using namespace solver;

ILOLAZYCONSTRAINTCALLBACK2(add_min_cuts_lc, IloBoolVarArray, x, ajns::instance&, problem_instance) {
	//std::cout << "find_constraints_for_integral_solution" << std::endl;
	IloEnv env = getEnv();
	IloNumArray x_values(env);
	getValues(x_values, x);

	auto graph_x = callback::construct_candidate_graph_from_x_assigment(
		problem_instance.input_graph, x_values);
	auto tc_graph_x = my_graph::digraph();

	std::map<my_graph::vertex, my_graph::vertex> g_to_tc;
	std::vector<std::size_t> id_map(boost::num_vertices(graph_x));
	std::iota(id_map.begin(), id_map.end(), 0u);

	boost::transitive_closure(graph_x, tc_graph_x, boost::make_assoc_property_map(g_to_tc), id_map.data());

	for (auto& e : g_to_tc)
		tc_graph_x[e.second] = graph_x[e.first];
	
	my_graph::digraph complete_net_flow;
	boost::copy_graph(problem_instance.input_graph, complete_net_flow);
	for (const auto& e : boost::make_iterator_range(boost::edges(complete_net_flow))) {
		complete_net_flow[e].capacity = x_values[complete_net_flow[e].id];
		if (complete_net_flow[e].capacity <= 1e-6) {
			complete_net_flow[e].capacity = 0;
		}
	}

	for (const auto& e : boost::make_iterator_range(boost::edges(problem_instance.covering_graph))) {
		auto head = boost::source(e, problem_instance.covering_graph);
		auto tail = boost::target(e, problem_instance.covering_graph);
		if (not boost::edge(head, tail, tc_graph_x).second) {
			//std::cout << "(" << head << "," << tail << ") violated\n";

			auto set_q = std::set<my_graph::vertex>();
			set_q.insert(problem_instance.root);
			set_q.insert(problem_instance.predecessors.at(head).begin(), problem_instance.predecessors.at(head).end());
			set_q.insert(problem_instance.sucessors.at(tail).begin(), problem_instance.sucessors.at(tail).end());

			my_graph::digraph net_flow;
			boost::copy_graph(complete_net_flow, net_flow);

			for (const auto& v : boost::make_iterator_range(boost::vertices(net_flow))) {
				// remove v (actually, we delete adjacent arcs)
				if (set_q.find(v) != set_q.end()) {
					boost::clear_vertex(v, net_flow);
				}
			}

			auto f = ajns::flow(net_flow);
			f.run(head, tail, ajns::BOYKOL);

			if (f.get_max_flow_value() < 1) {
				//std::cout << "FLOW LESS THAN ONE\n" << f.get_max_flow_value() << std::endl;
				auto min_cut = f.get_min_cut();
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

#endif /* SOLVER_CALLBACKS_ADD_MIN_CUTS_LC_H_ */
#pragma once
