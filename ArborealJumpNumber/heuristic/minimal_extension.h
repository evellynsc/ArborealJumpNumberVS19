/*
 * minimal_extension.h
 *
 *  Created on: 2 de mai de 2021
 *      Author: evellyn
 */

#ifndef HEURISTIC_MINIMAL_EXTENSION_H_
#define HEURISTIC_MINIMAL_EXTENSION_H_

#include "../base/instance.h"
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/copy.hpp>
#include <list>
#include <unordered_map>
#include <algorithm>
#include "../base/properties.h"

namespace ajns {

struct violator_visitor: boost::default_dfs_visitor {
	boost::shared_ptr<std::list<my_graph::vertex>> targets;
	violator_visitor() :
			targets(new std::list<my_graph::vertex>()) { }

	template<typename Vertex, typename Graph> void discover_vertex(Vertex v,
			Graph const &g) {
		if (boost::in_degree(v, g) > 1) {
			targets->emplace_back(v);
		}
		boost::default_dfs_visitor::discover_vertex(v, g);
	}
};

class minimal_extension {
	instance problem_instance;
	std::unordered_map<my_graph::vertex,unsigned> vertex_levels;
	std::unordered_map<my_graph::vertex,unsigned> num_pred_violators;
	std::unordered_map<my_graph::vertex,unsigned> num_succ_violators;
	std::map<my_graph::vertex,std::list<my_graph::vertex>> open_violations;
	std::vector<std::pair<my_graph::vertex, my_graph::vertex>> added_jumps;
	std::pair<my_graph::vertex, my_graph::vertex> curr_added_jump;
	my_graph::digraph t_order_extension;
	my_graph::digraph order_extension;
	my_graph::digraph extension;
	my_graph::digraph solution;

	void find_violations();
	my_graph::vertex get_minimal_element(std::vector<my_graph::vertex> vertices, my_graph::digraph &graph);
	void find_vertex_levels();
	void find_num_pred_violators();
	void find_num_succ_violators();
	my_graph::vertex choose_violator();
	my_graph::vertex choose_vertex_x1(my_graph::vertex c);
	my_graph::vertex choose_vertex_x2(my_graph::vertex c, my_graph::vertex x1);
	std::map<my_graph::vertex, std::list<my_graph::vertex>> find_x2_z(my_graph::vertex c, my_graph::vertex x1);
	std::list<my_graph::vertex> difference_x1_x2(my_graph::vertex x1, my_graph::vertex x2);
	std::list<std::pair<my_graph::vertex, my_graph::vertex>> find_edges_to_remove(std::map<my_graph::vertex, std::list<my_graph::vertex>>&);
	void update_violations(std::list<std::pair<my_graph::vertex, my_graph::vertex>>&);
	void update_vertex_levels();
	void update_order_extension();
	void update_num_pred_violators(my_graph::vertex central);
	void update_num_pred_violators();
	void print_vertex_levels();
	void print_num_pred_violators();
	void print_open_violations();
	void update_extension(std::map<my_graph::vertex, std::list<my_graph::vertex>> &);
	std::list<my_graph::vertex> get_min_z(std::list<my_graph::vertex>&);


public:
	minimal_extension(instance &problem_instance);
	virtual ~minimal_extension();
	void run();
	void run(properties &p);
	my_graph::digraph get_solution();
};

} /* namespace ajns */

//https://stackoverflow.com/questions/50583130/how-does-distance-recorder-work-in-boost-graph-library
//dinamic properties boost

#endif /* HEURISTIC_MINIMAL_EXTENSION_H_ */
