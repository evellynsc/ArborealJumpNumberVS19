/*
 * flow.h
 *
 *  Created on: 14 de abr de 2021
 *      Author: evellyn
 */

#ifndef ALGORITHMS_FLOW_H_
#define ALGORITHMS_FLOW_H_

#include <map>
#include <list>
#include <map>
#include <set>

#include "../base/graph.h"
#include "boost/graph/push_relabel_max_flow.hpp"
#include <boost/graph/copy.hpp>
#include "boost/graph/boykov_kolmogorov_max_flow.hpp"
#include "boost/graph/edmonds_karp_max_flow.hpp"
#include "boost/property_map/transform_value_property_map.hpp"

namespace ajns {
enum algo_flow {BOYKOL, PUSREL};
class flow {
	my_graph::digraph network;
	double current_max_flow_value;
	std::list<my_graph::edge> current_min_cut;
	std::map<my_graph::edge,my_graph::edge> reversed_edge_of;
	my_graph::color_map vertex_coloring;
	std::map<my_graph::edge, double> res_capacity_map;
	std::set<my_graph::vertex> set_s;
	std::set<my_graph::vertex> set_bar_s;

	void add_missing_reversed_edges();
	void fill_aux_maps();
	bool is_edge_capacities_set();



public:
	flow(my_graph::digraph);
	void run(my_graph::vertex, my_graph::vertex, algo_flow algo);
	double get_max_flow_value();

	std::set<my_graph::vertex> get_set_s();
	std::set<my_graph::vertex> get_set_bar_s();
	std::list<my_graph::edge> get_min_cut();

	virtual ~flow();
};

} /* namespace ajns */

#endif /* ALGORITHMS_FLOW_H_ */
