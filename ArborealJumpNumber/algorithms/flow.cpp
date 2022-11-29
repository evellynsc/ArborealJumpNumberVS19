/*
 * flow.cpp
 *
 *  Created on: 14 de abr de 2021
 *      Author: evellyn
 */

#include "flow.h"


namespace ajns {

void flow::add_missing_reversed_edges() {
	auto num_vertices = boost::num_vertices(network);
	auto rev_edge_ids = num_vertices*num_vertices;
	my_graph::edge_itr eit, eit_end;
	auto graph_copy = my_graph::digraph();
	boost::copy_graph(network, graph_copy);

//	TODO: does it work? iterating graph while modifying it?
	auto reverse = my_graph::edge();
	auto reverse_exists = false;
	for (const auto& e : boost::make_iterator_range(boost::edges(graph_copy))) {
		auto source_vertex = boost::source(e, network);
		auto target_vertex = boost::target(e, network);
		boost::tie(reverse, reverse_exists) = boost::edge(target_vertex, source_vertex, network);
		if (not reverse_exists) {
			my_graph::edge_info rev_info(rev_edge_ids++, target_vertex, source_vertex, my_graph::FLOW);
			boost::tie(reverse, reverse_exists) = boost::add_edge(target_vertex, source_vertex,
					rev_info, network);
			if (not reverse_exists) exit(1);
			network[reverse].capacity = 0.0;
		}
	}
}

void flow::fill_aux_maps() {
	auto reverse = my_graph::edge();
	auto reverse_exists = false;
	for (const auto& e : boost::make_iterator_range(boost::edges(network))) {
		auto source_vertex = boost::source(e, network);
		auto target_vertex = boost::target(e, network);
		//boost::tie(reverse, reverse_exists) = boost::edge(target_vertex, source_vertex, network);
		//if (not reverse_exists) exit(1);
		//reversed_edge_of[e] = reverse;
		res_capacity_map[e] = 0.0;
//		std::cout << network[e].capacity << " ";
	}
//	std::cout << std::endl;
}

flow::flow(my_graph::digraph network) {
	this->current_min_cut = std::list<my_graph::edge>();
	this->network = network;
	this->current_max_flow_value = -1.0;
	//add_missing_reversed_edges();
	//fill_aux_maps();
}

std::set<my_graph::vertex> flow::get_set_s() {
	auto set_s = std::set<my_graph::vertex>();

	for (const auto& e : current_min_cut) {
		auto head = boost::source(e, network);
		set_s.insert(head);
	}

	return set_s;
}

std::set<my_graph::vertex> flow::get_set_bar_s() {
	auto set_bar_s = std::set<my_graph::vertex>();

	for (const auto& e : current_min_cut) {
		auto tail = boost::target(e, network);
		set_bar_s.insert(tail);
	}

	return set_bar_s;
}

void flow::run(my_graph::vertex source, my_graph::vertex target, algo_flow algo) {
	if (algo == PUSREL) {
		auto props = capacity_map(get(&my_graph::edge_info::capacity,network))
				.residual_capacity_map(boost::make_assoc_property_map(res_capacity_map))
				.color_map(make_assoc_property_map(vertex_coloring))
				.reverse_edge_map(make_assoc_property_map(reversed_edge_of))
				.vertex_index_map(get(boost::vertex_index,network));
		current_max_flow_value = boost::push_relabel_max_flow(network, source, target, props);
	} else {
		current_max_flow_value = boost::boykov_kolmogorov_max_flow(network,
				boost::get(&my_graph::edge_info::capacity,network),
				boost::make_assoc_property_map(res_capacity_map),
				boost::make_assoc_property_map(reversed_edge_of),
				boost::make_assoc_property_map(vertex_coloring),
				boost::get(boost::vertex_index,network),
				source, target);
	}
}

double flow::get_max_flow_value() {
	return this->current_max_flow_value;
}

std::list<my_graph::edge> flow::get_min_cut() {
	/*for (const auto& v : vertex_coloring) {
		if (v.second == boost::default_color_type::black_color)
			std::cout << v.first << " :: " << " black" << std::endl;
		else 
			std::cout << v.first << " :: " << " not black" << std::endl;
	}*/
	if (not current_min_cut.empty())
		return current_min_cut;
	

	for (auto e : boost::make_iterator_range(boost::edges(network))) {
		/*if (network[e].type == my_graph::FLOW or network[e].type == my_graph::USINK) {
			current_min_cut.clear();
			break;
		}*/

		if (network[e].type == my_graph::ARTIFICIAL or network[e].type == my_graph::ORIGINAL) {
			auto head = boost::source(e, network);
			auto tail = boost::target(e, network);
			if (vertex_coloring[head] == boost::default_color_type::black_color and 
				vertex_coloring[tail] != boost::default_color_type::black_color) {
				this->current_min_cut.emplace_back(e);
			}
		}
	}
	//std::cout << std::endl;
	return current_min_cut;
}

flow::~flow() {
	// TODO Auto-generated destructor stub
}

} /* namespace ajns */
