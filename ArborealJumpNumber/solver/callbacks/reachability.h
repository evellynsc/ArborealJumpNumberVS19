/*
 * reachability.h
 *
 *  Created on: 20 de abr de 2021
 *      Author: evellyn
 */

#ifndef SOLVER_CALLBACKS_REACHABILITY_H_
#define SOLVER_CALLBACKS_REACHABILITY_H_

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "../../base/instance.h"
#include "../../utils/callbacks.h"
#include "../../algorithms/flow.h"
#include <numeric>

#include <boost/graph/transitive_closure.hpp>

#include <map>

using namespace solver;

ILOLAZYCONSTRAINTCALLBACK2(find_constraints_for_integral_solution, IloBoolVarArray, x, ajns::instance&, problem_instance) {
//	std::cout << "find_constraints_for_integral_solution" << std::endl;
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
//
//	boost::print_graph(graph_x);
//	std::cout << "===================" << std::endl;
//	boost::print_graph(tc_graph_x);
//	std::cout << "===================" << std::endl;
//	boost::print_graph(problem_instance.covering_graph);
	auto cuts = std::map<my_graph::edge, std::pair<std::vector<my_graph::vertex>, std::vector<my_graph::vertex>>>();
//	std::cout << "=======================\n";
//	boost::print_graph(problem_instance.order_graph);
	auto cuts_to_add = std::vector<std::pair<std::set<my_graph::vertex>, std::set<my_graph::vertex>>>();
	for (auto e : boost::make_iterator_range(boost::edges(problem_instance.covering_graph))) {
		auto head = boost::source(e, problem_instance.covering_graph);
		auto tail = boost::target(e, problem_instance.covering_graph);
		if (not boost::edge(head, tail, tc_graph_x).second) {
//			std::cout << "dont obey precedence\n";
//			std::cout << problem_instance.order_graph[head].id << ","
//					<< problem_instance.order_graph[tail].id << std::endl;
			auto set_q = std::set<my_graph::vertex>();
			
			set_q.insert(problem_instance.root);
			set_q.merge(problem_instance.predecessors.at(head));
			set_q.merge(problem_instance.sucessors.at(tail));


			auto set_s = callback::get_neighboors_of(head, tc_graph_x);

			auto head_cut = std::set<my_graph::vertex>();
			head_cut.insert(head);
			for (auto& v : set_s)
			{
				if (set_q.find(v) != set_q.end()) {
					head_cut.insert(v);
				}
			}

			auto set_complement = callback::get_set_complement(boost::vertices(problem_instance.input_graph),set_s);
			auto tail_cut = std::set<my_graph::vertex>();
			tail_cut.insert(tail);
			for (auto& v : set_complement)
			{
				if (set_q.find(v) != set_q.end()) {
					tail_cut.insert(v);
				}
			}
			//cuts_to_add.push_back({head_successors,set_complement});

			/*auto transpose_tc_graph_x = my_graph::digraph();

			boost::transpose_graph(tc_graph_x, transpose_tc_graph_x);
			

			auto tail_predecessors = callback::get_neighboors_of(tail, problem_instance.input_graph);
			tail_predecessors.insert(tail);
			set_complement = callback::get_set_complement(boost::vertices(problem_instance.input_graph),
								tail_predecessors);*/
			cuts_to_add.push_back({head_cut,tail_cut});
		}
	}

	for (auto c : cuts_to_add) {
		auto heads = c.first;
		auto tails = c.second;

		auto edges_of_cut = std::list<my_graph::edge>();
		auto exist = false;
		auto e = my_graph::edge();
		for (auto h : heads) {
			for (auto t : tails) {
				boost::tie(e, exist) = boost::edge(h,t, problem_instance.input_graph);
				if (exist) {
					edges_of_cut.push_back(e);
				}
			}
		}

		auto lhside_ineq = IloExpr(env);
		callback::construct_lhside_of_inequality(problem_instance.input_graph,
									edges_of_cut, x, lhside_ineq);
		add(lhside_ineq >= 1);
		lhside_ineq.end();
	}

//	for (auto c : cuts) {
//		auto heads = c.second.first;
//		auto tails = c.second.second;
////		heads.push_back(boost::source(c.first, problem_instance.order_graph));
////		tails.push_back(boost::target(c.first, problem_instance.order_graph));
//		auto edges_of_cut = std::vector<my_graph::edge>();
//		for (auto h : heads) {
//			for (auto t : tails) {
//				if (boost::edge(h,t, problem_instance.input_graph).second) {
//					edges_of_cut.push_back(boost::edge(h,t, problem_instance.input_graph).first);
//				}
//			}
//		}
//		auto lhside_ineq = IloExpr(env);
//		callback::construct_lhside_of_inequality(problem_instance.input_graph,
//									edges_of_cut, x, lhside_ineq);
//		add(lhside_ineq >= 1);
//		lhside_ineq.end();
//	}

//	std::cout << "NAO ACHOU NADA AQUI!!" << std::endl;
}

#endif /* SOLVER_CALLBACKS_REACHABILITY_H_ */
