/*
 * solution.cpp
 *
 *  Created on: 1 de mar de 2021
 *      Author: evellyn
 */

#include "solution.h"

Solution::Solution() {
	num_jumps = -1;
}

Solution::Solution(int _njumps, int _nvertices, std::vector<std::pair<int, int>>& _edges) {
	this->num_jumps = _njumps;
	this->arboreal_extension = construct_graph(_nvertices, _edges);
}

my_graph::digraph Solution::construct_from_edges(my_graph::digraph& input_graph, std::vector<bool>& selected_edges) {
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

my_graph::digraph Solution::construct_graph(int _nvertices, std::vector<std::pair<int, int>>& _edges) {
	my_graph::digraph solution_graph;
	my_graph::vertex_itr vit, vit_end;

	auto _vinfo = std::vector<my_graph::vertex_info>(_nvertices);
	auto _vertices = std::vector<my_graph::vertex>(_nvertices);

	for (int i = 0; i < _nvertices; i++) {
		_vinfo[i] = my_graph::vertex_info("0", i);
		if (i == _nvertices-1) {
			_vinfo[i].is_root = true;
		}
	}

	for (auto i : _vinfo) {
		auto v = boost::add_vertex(i, solution_graph);
		_vertices.push_back(v);
	}


	for (auto edge : _edges) {
		auto i = edge.first;
		auto j = edge.second;
		boost::add_edge(_vertices[i], _vertices[j], solution_graph);
	}

	return solution_graph;
}

void Solution::save_solution(std::string _filename, std::string _format) {
	_filename = _filename + "_solution.dot";
	auto _outfile = std::ofstream(_filename);
	boost::write_graphviz(_outfile, this->arboreal_extension,
		boost::make_label_writer(
			boost::get(&my_graph::vertex_info::id, arboreal_extension)),
		boost::make_label_writer(
			boost::get(&my_graph::edge_info::type, arboreal_extension)));
	_outfile.close();
}
