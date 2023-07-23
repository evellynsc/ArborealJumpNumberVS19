/*
 * instance_generator.h
 *
 *  Created on: 14 de fev de 2021
 *      Author: evellyn
 */

#ifndef PREPROCESSING_INSTANCE_GENERATOR_H_
#define PREPROCESSING_INSTANCE_GENERATOR_H_

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "../headers/instance.h"
#include "../headers/graph.h"
#include "problem_data.h"

class instance_generator {
	size_t current_edge_id;
	std::vector<vertex_info> vertex_properties;
	map_vertex_set get_successors(digraph& input_graph);
	map_vertex_set get_predecessors(digraph& input_graph);

	void set_vertices_to_remove(digraph&);
	void set_edges_to_remove(digraph&);


	void fix_ids_order_graph(digraph &order_graph, digraph &covering_graph);
	std::vector<vertex_info> create_vertex_properties(std::vector<std::string> &vertex_labels);
	void add_root(std::vector<std::vector<bool>> &adjacency_matrix, std::vector<std::string> &vertex_labels, std::size_t &root_id);
	digraph create_order_graph(std::vector<vertex_info> &vertex_info,
			std::vector<std::vector<bool>> &adjacency_matrix);
	digraph create_covering_graph(digraph &order_graph);
	std::vector<std::pair<vertex, vertex>> find_incompatible_vertices(digraph &order_graph);
	void add_artificial_edges();
	digraph create_input_graph(digraph &order_graph, digraph &covering_graph, std::vector<std::pair<int, int>>&);
	digraph transpose_order_graph(digraph &order_graph);
	std::vector<bool> find_ids_maximum_vertices(std::vector<std::vector<bool>> &adjacency_matrix);
	void update_maximum_vertices(digraph& graph, std::vector<bool> &ids);
	vertex get_root(digraph& graph);
public:
	Instance create_instance(problem_data &data);

};



#endif /* PREPROCESSING_INSTANCE_GENERATOR_H_ */
