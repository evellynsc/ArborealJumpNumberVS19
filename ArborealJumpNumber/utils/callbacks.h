/*
 * callbacks.h
 *
 *  Created on: 3 de mar de 2021
 *      Author: evellyn
 */

#ifndef UTILS_CALLBACKS_H_
#define UTILS_CALLBACKS_H_

#include <boost/config.hpp>
#include <boost/graph/copy.hpp>
#include <vector>
#include <map>
#include "../base/graph.h"
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

namespace solver {
	namespace callback {
		inline void strengthen_cut(std::list<my_graph::edge> cut, std::vector<bool> to_remove, const my_graph::digraph& graph);
		inline my_graph::digraph construct_graph_from_x_assigment(const my_graph::digraph &graph,
				IloNumArray &x_values);
		inline void remove_neighboors_of(my_graph::vertex v, my_graph::digraph &graph,
				const my_graph::digraph &imutable_graph);
		inline void reset_edges_of_vertice(my_graph::vertex v, my_graph::digraph &graph,
				const my_graph::digraph &imutable_graph);
		inline void construct_lhside_of_inequality(const my_graph::digraph &graph, std::list<my_graph::edge> &cut,
				IloBoolVarArray &x, IloExpr &lhside_ineq);
		inline void add_universal_sink(my_graph::digraph &graph, my_graph::vertex &sink);
		inline std::set<my_graph::vertex> get_neighboors_of(my_graph::vertex v, const my_graph::digraph &graph);
		inline my_graph::digraph construct_candidate_graph_from_x_assigment(const my_graph::digraph &graph,
				IloNumArray &x_values);
		inline std::set<my_graph::vertex> get_set_complement(std::pair<my_graph::vertex_itr, my_graph::vertex_itr> vertices, std::set<my_graph::vertex> vertex_set);

	} /* namespace callback */

	inline void callback::strengthen_cut(std::list<my_graph::edge> cut, std::vector<bool> to_remove, const my_graph::digraph& graph) {
		auto remove_edge = std::list<my_graph::edge>();
		for (auto e : cut){
			auto head = boost::source(e, graph);
			auto tail = boost::target(e, graph);

			if (to_remove[graph[head].id] or to_remove[graph[tail].id])
				remove_edge.push_back(e);
		}

		for (auto e : remove_edge) {
			cut.remove(e);
		}
	}

	inline std::set<my_graph::vertex> callback::get_set_complement(std::pair<my_graph::vertex_itr, my_graph::vertex_itr> vertices, std::set<my_graph::vertex> vertex_set) {
		auto complement = std::set<my_graph::vertex>();
		for (auto v : boost::make_iterator_range(vertices)) {
			if (vertex_set.find(v) == vertex_set.end())
				complement.insert(v);
		}
		return complement;
	}

	inline std::set<my_graph::vertex> callback::get_neighboors_of(my_graph::vertex v, const my_graph::digraph &graph) {
		auto neighboors = boost::adjacent_vertices(v, graph);
		auto vertices = std::set<my_graph::vertex>();
		for (auto n : boost::make_iterator_range(neighboors)) {
			vertices.insert(n);
		}
		return vertices;
	}

	inline void callback::construct_lhside_of_inequality(const my_graph::digraph &graph,
			std::list<my_graph::edge> &cut, IloBoolVarArray &x, IloExpr &lhside_ineq) {
		for (const auto& e : cut) {
			auto idx = graph[e].id;
			lhside_ineq += x[idx];
		}
	}

	inline void callback::add_universal_sink(my_graph::digraph &graph, my_graph::vertex &sink) {
		auto graph_copy = my_graph::digraph();
		boost::copy_graph(graph, graph_copy);
		sink = boost::add_vertex(graph);
		graph[sink].label = "sink";
		auto id_sink = graph[sink].id = boost::num_vertices(graph);
		auto ids_new_edges = 2*std::pow(boost::num_vertices(graph),2);

		auto added = false;
		auto new_edge = my_graph::edge();
		for (auto v : boost::make_iterator_range(boost::vertices(graph_copy))) {
			if (graph[v].is_maximum) {
				auto id_v = graph[v].id;
				boost::tie(new_edge, added) = boost::add_edge(v, sink,
						my_graph::edge_info(ids_new_edges++, id_v, id_sink, my_graph::USINK),
						graph);
				graph[new_edge].capacity = 1;
			}
		}
	}

	inline my_graph::digraph callback::construct_candidate_graph_from_x_assigment(const my_graph::digraph &graph,
			IloNumArray &x_values) {
		my_graph::digraph candidate_graph;
		boost::copy_graph(graph, candidate_graph);

		for (const auto& e : boost::make_iterator_range(boost::edges(graph))) {
			auto i = candidate_graph[e].id;
			auto h = boost::source(e, graph);
			auto t = boost::target(e, graph);
			auto to_be_removed = boost::edge(h,t, candidate_graph).first;
			if (x_values[i] <= 1e-6) {
//				std::cout << "removing edge\n";
				boost::remove_edge(to_be_removed, candidate_graph);
			}
			else {
				candidate_graph[e].capacity = x_values[i];
				if (candidate_graph[e].capacity <= 1e-6) {
					candidate_graph[e].capacity = 0;
				}
			}
		}

		return candidate_graph;
	}

	inline my_graph::digraph callback::construct_graph_from_x_assigment(const my_graph::digraph &graph,
			IloNumArray &x_values) {
		my_graph::digraph lr_graph;

		boost::copy_graph(graph, lr_graph);

		for (const auto& e : boost::make_iterator_range(boost::edges(lr_graph))) {
			auto i = lr_graph[e].id;
			auto x_value = x_values[i];
			if (x_values[i] <= 1e-6)
				x_value = 0;
			lr_graph[e].capacity = x_value;
//			std::cout << lr_graph[e].capacity << std::endl;
		}
//		std::cout << "construct graph from x assigment\n";
		return lr_graph;
	}

	inline void callback::remove_neighboors_of(my_graph::vertex v, my_graph::digraph &graph,
			const my_graph::digraph &imutable_graph) {
		auto neighboors = boost::adjacent_vertices(v, imutable_graph);
		for (auto n : boost::make_iterator_range(neighboors)) {
			boost::remove_vertex(n, graph);
		}
	}

	inline void callback::reset_edges_of_vertice(my_graph::vertex v, my_graph::digraph &graph,
			const my_graph::digraph &imutable_graph) {
		auto in_edges_v = boost::in_edges(v, imutable_graph);
		auto out_edges_v = boost::out_edges(v, imutable_graph);
		for (const auto& e : boost::make_iterator_range(in_edges_v)) {
			graph[e].capacity = 0;
		}
		for (const auto& e : boost::make_iterator_range(out_edges_v)) {
			graph[e].capacity = 0;
		}
//		std::cout << "reset_edges_of_vertice\n";
	}

//inline void callback::remove_predecessors_of(vertex v, digraph &graph,
//		const digraph &imutable_graph) {
//	remove_sucessors_of(v, graph, imutable_graph);
//}

//inline void callback::remove_sucessors_of(vertex v, digraph &graph,
//		const digraph &imutable_graph) {
//	digraph graph_copy;
//	boost::copy_graph(graph, graph_copy);
//	auto successors = boost::adjacent_vertices(v, imutable_graph);
//	vertex_itr vit, vit_end;
//
//	for (boost::tie(vit, vit_end) = boost::adjacent_vertices(v, imutable_graph);
//			vit != vit_end; ++vit) {
//		boost::remove_vertex(*vit, graph);
//	}
//}

}/* namespace ajns */

#endif /* UTILS_CALLBACKS_H_ */
