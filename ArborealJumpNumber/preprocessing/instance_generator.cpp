/*
 * instance_generator.cpp
 *
 *  Created on: 14 de fev de 2021
 *      Author: evellyn
 */

#include <map>
#include <numeric>

#include "instance_generator.h"
#include <boost/graph/transitive_closure.hpp>
#include <boost/graph/transitive_reduction.hpp>
#include <boost/graph/transpose_graph.hpp>
#include <boost/graph/graphviz.hpp>


Instance instance_generator::create_instance(problem_data &data) {
	current_edge_id = 0u;
	size_t root_id = 0u;
	add_root(data.adjacency_matrix, data.vertex_labels, root_id);
	vertex_properties = create_vertex_properties(data.vertex_labels);
	auto order_graph = create_order_graph(vertex_properties,
			data.adjacency_matrix);
	auto covering_graph = create_covering_graph(order_graph);
	
	//Remove vértices que formam um caminho o-o-o-o, onde o último vértice é uma folha
	set_vertices_to_remove(covering_graph);
	set_edges_to_remove(covering_graph);
	auto artificial_arcs_pair = std::vector<std::pair<int, int>>();
	auto input_graph = create_input_graph(order_graph, covering_graph, artificial_arcs_pair);

	fix_ids_order_graph(order_graph, covering_graph);
	auto t_order_graph = transpose_order_graph(order_graph);
	auto root = get_root(order_graph);

	map_vertex_set sucessors = get_successors(order_graph);
	map_vertex_set predecessors = get_predecessors(order_graph);

	map_vertex_set covering_sucessors = get_successors(covering_graph);
	map_vertex_set covering_predecessors = get_predecessors(covering_graph);


	auto ajnp = Instance(data.id, root, order_graph, t_order_graph,
			covering_graph, input_graph, predecessors, sucessors, covering_predecessors, covering_sucessors, artificial_arcs_pair);
	std::cout << "ROOT ======= " << order_graph[root].label << std::endl;
	auto dot = "dot -Tpdf ";
	auto command = std::string();
	std::ofstream outFile;
	auto name_file = data.id + "_order.dot";
	outFile.open(name_file);
	boost::write_graphviz(outFile, order_graph,
			boost::make_label_writer(
					boost::get(&vertex_info::id, order_graph)),
			boost::make_label_writer(
					boost::get(&edge_info::type, order_graph)));
	outFile.close();
	command = dot + name_file + " -o " + data.id + "_order.pdf";
//	std::system(command.c_str());
	name_file = data.id + "_covering.dot";
	outFile.open(name_file);
	boost::write_graphviz(outFile, covering_graph,
			boost::make_label_writer(
					boost::get(&vertex_info::id, covering_graph)),
			boost::make_label_writer(
					boost::get(&edge_info::type, covering_graph)));
	outFile.close();
	command = dot + name_file + " -o " + data.id + "_covering.pdf";
//	std::system(command.c_str());
	name_file = data.id + "_input.dot";
	outFile.open(name_file);
	boost::write_graphviz(outFile, input_graph,
			boost::make_label_writer(
					boost::get(&vertex_info::id, input_graph)),
			boost::make_label_writer(
					boost::get(&edge_info::type, input_graph)));
	outFile.close();

	

	std::cout << "finish creating instance" << std::endl;

	return ajnp;
}

void instance_generator::set_vertices_to_remove(digraph& graph) {
	std::unordered_map<vertex, size_t> out_dg;
	std::list<vertex> vertices_to_remove;

	in_edge_itr ei, ei_end;

	for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
		auto iv = boost::in_degree(v, graph);
		auto ov = boost::out_degree(v, graph);
		if (iv == 1 and ov == 0) {
			//incluir na lista
			out_dg.insert({v, 0});
			vertices_to_remove.push_back(v);
			graph[v].remove = true;
			std::cout << graph[v].id << std::endl;
		}
	}

	while (not vertices_to_remove.empty()) {
		vertex v = vertices_to_remove.front();
		vertices_to_remove.pop_front();
		for (boost::tie(ei, ei_end) = in_edges(v, graph); ei != ei_end; ++ei) {
			auto s = boost::source(*ei, graph);
			if (boost::in_degree(s, graph) == 1) {
				if (out_dg.find(s) != out_dg.end()) {
					out_dg[s] -= 1;
				}
				else {
					out_dg.insert({ s, boost::out_degree(s, graph) - 1 });
				}

				if (out_dg[s] == 0) {
					vertices_to_remove.push_back(s);
					std::cout << graph[s].id << std::endl;
					graph[v].remove = true;
				}
			}
		}
	}
}

void instance_generator::set_edges_to_remove(digraph& graph) {
	std::unordered_map<vertex, size_t> out_dg;
	std::list<vertex> vertices_to_remove;

	in_edge_itr ei, ei_end;

	for (const auto& e : boost::make_iterator_range(boost::edges(graph))) {
		auto s = boost::source(e, graph);
		auto t = boost::target(e, graph);

		if (graph[s].remove or graph[t].remove) {
			graph[e].value_set = true;
			graph[e].value = 1;
		}
	}
}


map_vertex_set instance_generator::get_successors(digraph& graph) {
	map_vertex_set successors;
	out_edge_itr ei, ei_end;

	for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
		auto vertex_set = std::set<vertex>();
		for (boost::tie(ei, ei_end) = out_edges(v, graph); ei != ei_end; ++ei) {
			auto target = boost::target(*ei, graph);
			vertex_set.insert(target);
		}

		successors.insert({ v, vertex_set });
	}

	return successors;
}
map_vertex_set instance_generator::get_predecessors(digraph& graph) {
	map_vertex_set predecessors;

	in_edge_itr ei, ei_end;


	for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
		std::cout << graph[v].id << ": ";
		auto vertex_set = std::set<vertex>();
		for (boost::tie(ei, ei_end) = in_edges(v, graph); ei != ei_end; ++ei) {
			auto source = boost::source(*ei, graph);
			vertex_set.insert(source);
			std::cout << graph[source].id << ", ";
		}

		predecessors.insert({ v, vertex_set });
		std::cout << std::endl;
	}

	return predecessors;
}


void instance_generator::fix_ids_order_graph(digraph &order_graph, digraph &covering_graph) {
	for (const auto& e : boost::make_iterator_range(boost::edges(order_graph))) {
		if (order_graph[e].id == -1)
			order_graph[e].id = current_edge_id++;
	}
}

/* GET ROOT */
vertex instance_generator::get_root(digraph &graph) {
	for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
		if (graph[v].is_root)
			return v;
	}
	return vertex();
}

/* ADD ROOT */
/* add a root to the input graph */
void instance_generator::add_root(
		std::vector<std::vector<bool>> &adjacency_matrix,
		std::vector<std::string> &vertex_labels, size_t &root_id) {
	auto n = adjacency_matrix.size();
	auto minimum = std::vector<bool>(n, true);
	for (auto i = 0u; i < n; i++) {
		for (auto j = 0u; j < n; j++) {
			if (adjacency_matrix[i][j] == true)
				minimum[j] = false;
		}
	}

	adjacency_matrix.push_back(std::vector<bool>(n + 1, false));
	for (auto i = 0u; i < n; i++) {
		adjacency_matrix[i].push_back(false);
		adjacency_matrix[n][i] = minimum[i];
	}
	vertex_labels.push_back("root");
	root_id = n;
}

/* CREATE VERTEX PROPERTIES */
std::vector<vertex_info> instance_generator::create_vertex_properties(
		std::vector<std::string> &vertex_labels) {
	auto vertices = std::vector<vertex_info>();
	auto n = vertex_labels.size();

	for (auto i = 0u; i < n; i++) {
		vertices.push_back(vertex_info(vertex_labels[i], i));
		if (vertex_labels[i].compare("root") == 0) {
			vertices[i].is_root = true;
		}
	}

	return vertices;
}

/* CREATE ORDER GRAPH */
digraph instance_generator::create_order_graph(std::vector<vertex_info> &v_info,
		std::vector<std::vector<bool>> &adjacency_matrix) {
	auto n = v_info.size();
	auto adj_matrix_graph = digraph();
	auto vertices = std::vector<vertex>();

//		add vertices
	for (auto i : v_info) {
		auto v = boost::add_vertex(i, adj_matrix_graph);
		vertices.push_back(v);
	}

	std::cout << std::endl;
//		add edges
	edge e;
	auto inserted = false;

	for (auto i = 0u; i < n; i++) {
		for (auto j = 0u; j < n; j++) {
			if (adjacency_matrix[i][j]) {
				boost::tie(e, inserted) = boost::add_edge(vertices[i],
						vertices[j], edge_info(i, j, ORIGINAL),
						adj_matrix_graph);
				if (inserted) {
					inserted = false;
				} else {
					std::cerr << "Error while creating order graph"
							<< std::endl;
					exit(0);
				}
			}
		}
	}

//		find transitive closure
	auto order_graph = digraph();
	std::map<vertex, vertex> g_to_tc;
	std::vector<size_t> id_map(boost::num_vertices(adj_matrix_graph));
	std::iota(id_map.begin(), id_map.end(), 0u);

	boost::transitive_closure(adj_matrix_graph, order_graph,
			boost::make_assoc_property_map(g_to_tc), id_map.data());

	for (auto &e : g_to_tc)
		order_graph[e.second] = adj_matrix_graph[e.first];

//	to fix id later
	for (const auto& e : boost::make_iterator_range(boost::edges(order_graph))) {
		auto head = boost::source(e, order_graph);
		auto tail = boost::target(e, order_graph);
		order_graph[e].source_id = order_graph[head].id;
		order_graph[e].target_id = order_graph[tail].id;
	}

	return order_graph;
}

// TODO: review
digraph instance_generator::create_covering_graph(digraph &order_graph) {
	auto aux_graph = digraph();

	auto g_to_tr = std::map<vertex, vertex>();

	boost::transitive_reduction(order_graph, aux_graph,
			boost::make_assoc_property_map(g_to_tr), boost::get(&vertex_info::index, order_graph));

	auto tr_to_g = std::map<vertex, vertex>();
	for (auto &e : g_to_tr) {
		aux_graph[e.second] = order_graph[e.first];
		tr_to_g[e.second] = e.first;
	}

	for (const auto& e : boost::make_iterator_range((boost::edges(aux_graph)))) {
		aux_graph[e].id = current_edge_id++;
		auto h = boost::source(e, aux_graph);
		auto t = boost::target(e, aux_graph);
		auto found = boost::edge(tr_to_g[h],tr_to_g[t], order_graph);
		if (found.second) {
			order_graph[found.first].id = aux_graph[e].id;
		}
	}

	auto covering_graph = digraph();
	boost::copy_graph(order_graph, covering_graph);

	for (const auto& e : boost::make_iterator_range(boost::edges(order_graph))) {
		auto h = boost::source(e, order_graph);
		auto t = boost::target(e, order_graph);
		auto found = boost::edge(g_to_tr[h],g_to_tr[t], aux_graph);
		if (not found.second) {
			boost::remove_edge(h, t, covering_graph);
		}
	}

	return covering_graph;
}

std::vector<std::pair<vertex, vertex>> instance_generator::find_incompatible_vertices(
		digraph &order_graph) {
	auto incompatible_vertices = std::vector<std::pair<vertex, vertex>>();
	auto inserted = std::set<std::pair<vertex, vertex>>();

	for (auto h : boost::make_iterator_range(boost::vertices(order_graph))) {
		for (auto t : boost::make_iterator_range(boost::vertices(order_graph))) {
			if (inserted.empty()
					or inserted.find( { t, h }) == inserted.end()) {
				auto not_root = not (order_graph[h].is_root
						or order_graph[t].is_root);
				auto is_different = (order_graph[h].id != order_graph[t].id);
				if (not_root and is_different) {
					auto found = bool();
					auto rev_found = bool();
					found = boost::edge(h, t, order_graph).second;
					rev_found = boost::edge(t, h, order_graph).second;

					if (not (found or rev_found)) {
						inserted.insert( { h, t });
						incompatible_vertices.push_back( { h, t });
					}
				}
			}
		}
	}

	return incompatible_vertices;
}

digraph instance_generator::create_input_graph(digraph &order_graph,
		digraph &covering_graph, std::vector<std::pair<int, int>>& art_arcs_pair) {
	auto incompatible_vertices = find_incompatible_vertices(order_graph);

	auto input_graph = digraph();
	boost::copy_graph(covering_graph, input_graph);

	assert(boost::num_edges(input_graph) == boost::num_edges(covering_graph));

	edge e;
	auto inserted = false;

	auto value_set = false;
	for (auto a : incompatible_vertices) {
		if (covering_graph[a.first].remove or covering_graph[a.second].remove)
			value_set = true;
		boost::tie(e, inserted) = boost::add_edge(a.first, a.second,
				edge_info(current_edge_id++, a.first, a.second, ARTIFICIAL, value_set, 0),
				input_graph);
		
		if (not inserted)
			exit(0);
		boost::tie(e, inserted) = boost::add_edge(a.second, a.first,
				edge_info(current_edge_id++, a.second, a.first, ARTIFICIAL, value_set, 0),
				input_graph);
		value_set = false;

		art_arcs_pair.push_back({ input_graph[e].id - 1, input_graph[e].id });
	}

	assert(boost::num_edges(input_graph) == boost::num_edges(covering_graph) + 2 * incompatible_vertices.size());
	assert(boost::num_edges(input_graph) == current_edge_id);
	assert(boost::num_edges(input_graph) == art_arcs_pair.size());

	return input_graph;
}

std::vector<bool> instance_generator::find_ids_maximum_vertices(
		std::vector<std::vector<bool>> &adjacency_matrix) {
	auto n = adjacency_matrix.size();
	std::vector<bool> maximum_vertices(n, false);
	for (auto i = 0u; i < n; i++) {
		auto is_maximum = true;
		for (auto j = 0u; j < n; j++) {
			if (adjacency_matrix[i][j] == 1) {
				is_maximum = false;
				break;
			}
			if (is_maximum)
				maximum_vertices[i] = true;
		}
	}
	return maximum_vertices;
}

void instance_generator::update_maximum_vertices(digraph &graph,
		std::vector<bool> &ids) {
	for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
		if (ids[graph[v].id])
			graph[v].is_maximum = true;
	}
}

digraph instance_generator::transpose_order_graph(digraph &order_graph) {
	auto t_order_graph = digraph();
	boost::transpose_graph(order_graph, t_order_graph);
	return t_order_graph;
}


