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

#include "graph.h"

using map_vertex_set = std::unordered_map<vertex, std::set<vertex>>;



class Instance {
public:
	std::string id;
	vertex root;
	int num_vertices;
	int num_edges;
	std::vector<std::pair<int, int>> artificial_arcs_pair;
	std::vector<vertex_info> vertex_properties;
	std::vector<edge_info> edge_properties;
	digraph order_graph;
	digraph t_order_graph;
	digraph covering_graph;
	digraph input_graph;

	map_vertex_set sucessors;
	map_vertex_set predecessors;

	map_vertex_set covering_sucessors;
	map_vertex_set covering_predecessors;

	int num_jumps;


	//	TODO: make a vector to represent edge ids

	//		size_t num_edges;
	//		std::vector<size_t> inner_vertices;

	Instance() {
		id = "";
		num_vertices = 0;
		num_edges = 0;
		num_jumps = 0;
		input_graph = digraph();
		artificial_arcs_pair = std::vector<std::pair<int, int>>();
		root = -1;
	}

	Instance(std::string id, vertex root,
		digraph& order_graph, const digraph& t_order_graph,
		const digraph& covering_graph, const digraph& input_graph,
		map_vertex_set predecessors, map_vertex_set sucessors, map_vertex_set c_predecessors,
		map_vertex_set c_sucessors, std::vector<std::pair<int, int>> artificial_arcs_pair) :
		id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
			boost::num_edges(input_graph)), order_graph(order_graph), t_order_graph(
				t_order_graph), covering_graph(covering_graph), input_graph(
					input_graph), predecessors(predecessors), sucessors(sucessors),
		covering_predecessors(c_predecessors), covering_sucessors(c_sucessors),
		artificial_arcs_pair(artificial_arcs_pair) {
		num_jumps = 0;
		construct_adj_matrices();
	}

	Instance(std::string id, vertex root,
		std::vector<std::pair<int, int>> artificial_arcs_pair,
		digraph& order_graph, const digraph& t_order_graph,
		const digraph& covering_graph, const digraph& input_graph,
		map_vertex_set predecessors, map_vertex_set sucessors) :
		id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
			boost::num_edges(input_graph)), artificial_arcs_pair(
				artificial_arcs_pair), order_graph(order_graph), t_order_graph(
					t_order_graph), covering_graph(covering_graph), input_graph(
						input_graph), predecessors(predecessors), sucessors(sucessors) {
		construct_adj_matrices();
		num_jumps = 0;
	}

	Instance(std::string id, vertex root, digraph& order_graph, const digraph& t_order_graph,
		const digraph& covering_graph, const digraph& input_graph, map_vertex_set predecessors,
		map_vertex_set sucessors) :
		id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
			boost::num_edges(input_graph)), order_graph(order_graph), t_order_graph(
				t_order_graph), covering_graph(covering_graph), input_graph(
					input_graph), predecessors(predecessors), sucessors(sucessors) {
		construct_adj_matrices();
		num_jumps = 0;
	}

	Instance(std::string id, vertex root, digraph& order_graph, const digraph& t_order_graph,
		const digraph& covering_graph, const digraph& input_graph, map_vertex_set predecessors,
		map_vertex_set sucessors, std::vector<std::pair<int, int>> artificial_arcs_pair) :
		id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
			boost::num_edges(input_graph)), order_graph(order_graph), t_order_graph(
				t_order_graph), covering_graph(covering_graph), input_graph(
					input_graph), predecessors(predecessors), sucessors(sucessors),
		artificial_arcs_pair(artificial_arcs_pair) {
		construct_adj_matrices();
		num_jumps = 0;
	}

	bool exist_edge_in_input_graph(size_t from, size_t to) {
		return adj_matrix_input_graph[from][to];
	}

	bool exist_edge_in_order_graph(size_t from, size_t to) {
		return adj_matrix_order_graph[from][to];
	}

	bool exist_edge_in_covering_graph(size_t from, size_t to) {
		return adj_matrix_covering_graph[from][to];
	}

	void set_num_jumps(int _num_jumps) {
		num_jumps = _num_jumps;
	}

private:
	std::vector<std::vector<bool>> adj_matrix_input_graph;
	std::vector<std::vector<bool>> adj_matrix_order_graph;
	std::vector<std::vector<bool>> adj_matrix_covering_graph;

	void construct_adj_matrices() {
		auto n = num_vertices;

		auto get_row_of_neighboors = [n](const digraph& graph, const vertex from) {
			auto row = std::vector<bool>(n, false);
			for (const auto& e : boost::make_iterator_range(boost::out_edges(from, graph))) {
				auto to = boost::target(e, graph);
				row[to] = true;
			}
			return row;
		};

		for (auto from : boost::make_iterator_range(boost::vertices(input_graph))) {
			auto row = get_row_of_neighboors(input_graph, from);
			adj_matrix_input_graph.push_back(row);
		}

		for (auto from : boost::make_iterator_range(boost::vertices(order_graph))) {
			auto row = get_row_of_neighboors(order_graph, from);
			adj_matrix_order_graph.push_back(row);
		}

		for (auto from : boost::make_iterator_range(boost::vertices(covering_graph))) {
			auto row = get_row_of_neighboors(covering_graph, from);
			adj_matrix_covering_graph.push_back(row);
		}
	}
};



#endif /* PROBLEM_INSTANCE_H_ */
