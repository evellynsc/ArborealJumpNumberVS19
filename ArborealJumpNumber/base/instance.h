/*
 * instance.h
 *
 *  Created on: 14 de fev de 2021
 *      Author: evellyn
 */

#ifndef PROBLEM_INSTANCE_H_
#define PROBLEM_INSTANCE_H_

#include <string>
#include <vector>

#include "../base/graph.h"

using map_vertex_set = std::unordered_map<my_graph::vertex, std::set<my_graph::vertex>>;


namespace ajns {
struct instance {
	std::string id;
	my_graph::vertex root;
	size_t num_vertices;
	size_t num_edges;
	std::vector<std::pair<my_graph::edge, my_graph::edge>> bidirected_edges;
	std::vector<my_graph::vertex_info> vertex_properties;
	std::vector<my_graph::edge_info> edge_properties;
	my_graph::digraph order_graph;
	my_graph::digraph t_order_graph;
	my_graph::digraph covering_graph;
	my_graph::digraph input_graph;

	map_vertex_set sucessors;
	map_vertex_set predecessors;


	//	TODO: make a vector to represent edge ids

	//		size_t num_edges;
	//		std::vector<size_t> inner_vertices;

	instance() {
		id = "";
		num_vertices = 0;
		num_edges = 0;
		input_graph = my_graph::digraph();
		bidirected_edges = std::vector<std::pair<my_graph::edge, my_graph::edge>>();
		root = -1;
	}

	instance(std::string id, my_graph::vertex root,
			std::vector<std::pair<my_graph::edge, my_graph::edge>> bidirected_edges,
			my_graph::digraph &order_graph, const my_graph::digraph &t_order_graph,
			const my_graph::digraph &covering_graph, const my_graph::digraph &input_graph,
			map_vertex_set predecessors, map_vertex_set sucessors) :
			id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
					boost::num_edges(input_graph)), bidirected_edges(
					bidirected_edges), order_graph(order_graph), t_order_graph(
					t_order_graph), covering_graph(covering_graph), input_graph(
					input_graph), predecessors(predecessors), sucessors(sucessors) {
	}

	instance(std::string id, my_graph::vertex root, my_graph::digraph &order_graph, const my_graph::digraph &t_order_graph,
			const my_graph::digraph &covering_graph, const my_graph::digraph &input_graph, map_vertex_set predecessors, 
			map_vertex_set sucessors) :
			id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
					boost::num_edges(input_graph)), order_graph(order_graph), t_order_graph(
					t_order_graph), covering_graph(covering_graph), input_graph(
					input_graph), predecessors(predecessors), sucessors(sucessors) {
	}
};

}

#endif /* PROBLEM_INSTANCE_H_ */
