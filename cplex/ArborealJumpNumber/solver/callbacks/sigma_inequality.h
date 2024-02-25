/*
 * pi_inequality.h
 *
 *  Created on: 3 de mar de 2021
 *      Author: evellyn
 */

#ifndef SOLVER_CALLBACKS_SIGMA_INEQUALITY_H_
#define SOLVER_CALLBACKS_SIGMA_INEQUALITY_H_

#include <string>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "../../base/instance.h"
#include "../../utils/callbacks.h"
#include "../../algorithms/flow.h"

#include "boost/graph/transpose_graph.hpp"

using namespace solver;

ILOUSERCUTCALLBACK2(separate_sigma_inequalities, IloBoolVarArray, x, ajns::instance&, problem_instance) {
	if (!isAfterCutLoop())
		return;
	std::cout << "separate_sigma_inequalities" << std::endl;
	IloEnv env = getEnv();
	IloNumArray x_values(env);
	getValues(x_values, x);

	auto graph_x = callback::construct_graph_from_x_assigment(
			problem_instance.input_graph, x_values);

	for (auto v : boost::make_iterator_range(boost::vertices(problem_instance.input_graph))) {
		if (not problem_instance.input_graph[v].is_root) {
			my_graph::digraph copy_graph_x;
			boost::copy_graph(graph_x, copy_graph_x);
			callback::reset_edges_of_vertice(v, copy_graph_x,
					problem_instance.order_graph);
			auto graph_flow = ajns::flow(copy_graph_x);
			graph_flow.run(problem_instance.root, v, ajns::PUSREL);
			if (graph_flow.get_max_flow_value() < 1) {
				auto min_cut = graph_flow.get_min_cut();
				if (min_cut.size()) {
					auto lhside_ineq = IloExpr(env);
					callback::construct_lhside_of_inequality(
							problem_instance.input_graph, min_cut, x, lhside_ineq);
					add(lhside_ineq >= 1);
					lhside_ineq.end();
				}
			}
		}
	}
}

#endif /* SOLVER_CALLBACKS_SIGMA_INEQUALITY_H_ */
