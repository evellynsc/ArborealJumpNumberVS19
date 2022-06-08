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

#include "../base/instance.h"
#include "../base/graph.h"
#include "problem_data.h"

namespace ajns {
	class instance_generator {
		size_t current_edge_id;
		std::vector<my_graph::vertex_info> vertex_properties;

		void fix_ids_order_graph(my_graph::digraph &order_graph, my_graph::digraph &covering_graph);
		std::vector<my_graph::vertex_info> create_vertex_properties(std::vector<std::string> &vertex_labels);
		void add_root(std::vector<std::vector<bool>> &adjacency_matrix, std::vector<std::string> &vertex_labels, std::size_t &root_id);
		my_graph::digraph create_order_graph(std::vector<my_graph::vertex_info> &vertex_info,
				std::vector<std::vector<bool>> &adjacency_matrix);
		my_graph::digraph create_covering_graph(my_graph::digraph &order_graph);
		std::vector<std::pair<my_graph::vertex, my_graph::vertex>> find_incompatible_vertices(my_graph::digraph &order_graph);
		void add_artificial_edges();
		my_graph::digraph create_input_graph(my_graph::digraph &order_graph, my_graph::digraph &covering_graph);
		my_graph::digraph transpose_order_graph(my_graph::digraph &order_graph);
		std::vector<bool> find_ids_maximum_vertices(std::vector<std::vector<bool>> &adjacency_matrix);
		void update_maximum_vertices(my_graph::digraph& graph, std::vector<bool> &ids);
		my_graph::vertex get_root(my_graph::digraph& graph);
	public:
		instance create_instance(problem_data &data);

	};
}


#endif /* PREPROCESSING_INSTANCE_GENERATOR_H_ */
