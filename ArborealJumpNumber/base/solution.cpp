/*
 * solution.cpp
 *
 *  Created on: 1 de mar de 2021
 *      Author: evellyn
 */

#include "solution.h"

//namespace ajns {

//solution::solution() {
//	// TODO Auto-generated constructor stub
//
//}
//
//solution::~solution() {
//	// TODO Auto-generated destructor stub
//}
//
//digraph solution::construct_graph(digraph &input_graph, std::vector<bool> &selected_edges) {
//	digraph solution_graph;
//
//	vertex_itr vit, vit_end;
//	for (boost::tie(vit, vit_end) = boost::vertices(input_graph); vit != vit_end; ++vit) {
//		boost::add_vertex(input_graph[*vit], solution_graph);
//	}
//
//	edge_itr eit, eit_end;
//	for (boost::tie(eit, eit_end) = boost::edges(input_graph); eit != eit_end; ++eit) {
//		auto index_var = input_graph[*eit].id;
//		if (selected_edges[index_var]) {
//			auto source_node = boost::source(*eit, input_graph);
//			auto target_node = boost::target(*eit, input_graph);
//			boost::add_edge(source_node, target_node, input_graph[*eit], solution_graph);
//		}
//	}
//
//	return solution_graph;
//}

namespace ajns::solution {
	my_graph::digraph construct_from_edges(my_graph::digraph &input_graph, std::vector<bool> &selected_edges) {
		my_graph::digraph solution_graph;

		my_graph::vertex_itr vit, vit_end;
	for (boost::tie(vit, vit_end) = boost::vertices(input_graph); vit != vit_end; ++vit) {
		boost::add_vertex(input_graph[*vit], solution_graph);
	}

	my_graph::edge_itr eit, eit_end;
	for (boost::tie(eit, eit_end) = boost::edges(input_graph); eit != eit_end; ++eit) {
		auto index_var = input_graph[*eit].id;
		if (selected_edges[index_var]) {
			auto source_node = boost::source(*eit, input_graph);
			auto target_node = boost::target(*eit, input_graph);
			boost::add_edge(source_node, target_node, input_graph[*eit], solution_graph);
		}
	}

	return solution_graph;
}
//}

} /* namespace ajns */
