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
	class instance {
	public:
		std::string id;
		my_graph::vertex root;
		int num_vertices;
		int num_edges;

		std::vector<std::pair<int, int>> artificial_arcs_pair;
		std::vector<my_graph::vertex_info> vertex_properties;
		std::vector<my_graph::edge_info> edge_properties;
		my_graph::digraph order_graph;
		my_graph::digraph t_order_graph;
		my_graph::digraph covering_graph;
		my_graph::digraph input_graph;

		map_vertex_set sucessors;
		map_vertex_set predecessors;


		map_vertex_set covering_sucessors;
		map_vertex_set covering_predecessors;


		//	TODO: make a vector to represent edge ids

		//		size_t num_edges;
		//		std::vector<size_t> inner_vertices;

		instance() {
			id = "";
			num_vertices = 0;
			num_edges = 0;
			input_graph = my_graph::digraph();
			artificial_arcs_pair = std::vector<std::pair<int, int>>();
			root = -1;
		}

		instance(std::string id, my_graph::vertex root,
			std::vector<std::pair<int, int>> artificial_arcs_pair,
			my_graph::digraph& order_graph, const my_graph::digraph& t_order_graph,
			const my_graph::digraph& covering_graph, const my_graph::digraph& input_graph,
			map_vertex_set predecessors, map_vertex_set sucessors) :
			id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
				boost::num_edges(input_graph)), artificial_arcs_pair(
					artificial_arcs_pair), order_graph(order_graph), t_order_graph(
						t_order_graph), covering_graph(covering_graph), input_graph(
							input_graph), predecessors(predecessors), sucessors(sucessors) {
			construct_adj_matrices();
		}

		instance(std::string id, my_graph::vertex root, my_graph::digraph& order_graph, const my_graph::digraph& t_order_graph,
			const my_graph::digraph& covering_graph, const my_graph::digraph& input_graph, map_vertex_set predecessors,
			map_vertex_set sucessors) :
			id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
				boost::num_edges(input_graph)), order_graph(order_graph), t_order_graph(
					t_order_graph), covering_graph(covering_graph), input_graph(
						input_graph), predecessors(predecessors), sucessors(sucessors) {
			construct_adj_matrices();
		}

		instance(std::string id, my_graph::vertex root, my_graph::digraph& order_graph, const my_graph::digraph& t_order_graph,
			const my_graph::digraph& covering_graph, const my_graph::digraph& input_graph, map_vertex_set predecessors,
			map_vertex_set sucessors, std::vector<std::pair<int, int>> artificial_arcs_pair) :
			id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
				boost::num_edges(input_graph)), order_graph(order_graph), t_order_graph(
					t_order_graph), covering_graph(covering_graph), input_graph(
						input_graph), predecessors(predecessors), sucessors(sucessors),
			artificial_arcs_pair(artificial_arcs_pair) {
			construct_adj_matrices();
		}

		instance(std::string id, my_graph::vertex root, my_graph::digraph& order_graph, const my_graph::digraph& t_order_graph,
			const my_graph::digraph& covering_graph, const my_graph::digraph& input_graph, map_vertex_set predecessors,
			map_vertex_set sucessors, map_vertex_set covering_predecessors,
			map_vertex_set covering_sucessors, std::vector<std::pair<int, int>> artificial_arcs_pair) :
			id(id), root(root), num_vertices(boost::num_vertices(input_graph)), num_edges(
				boost::num_edges(input_graph)), order_graph(order_graph), t_order_graph(
					t_order_graph), covering_graph(covering_graph), input_graph(
						input_graph), predecessors(predecessors), sucessors(sucessors),
							covering_predecessors(covering_predecessors), covering_sucessors(covering_sucessors),
								artificial_arcs_pair(artificial_arcs_pair) {
			construct_adj_matrices();
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

	private:
		std::vector<std::vector<bool>> adj_matrix_input_graph;
		std::vector<std::vector<bool>> adj_matrix_order_graph;
		std::vector<std::vector<bool>> adj_matrix_covering_graph;

		void construct_adj_matrices() {
			auto n = num_vertices;

			auto get_row_of_neighboors = [n](const my_graph::digraph& graph, const my_graph::vertex from) {
				auto row = std::vector<bool>(n, false);
				for (const auto &e : boost::make_iterator_range(boost::out_edges(from, graph))) {
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

}

#endif /* PROBLEM_INSTANCE_H_ */
