/*
 * graph.h
 *
 *  Created on: 14 de fev de 2021
 *      Author: evellyn
 */

#ifndef BASE_GRAPH_H_
#define BASE_GRAPH_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include "elementary.h"
namespace my_graph {
using matrix = boost::adjacency_matrix<boost::directedS, vertex_info, edge_info>;
using digraph = boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, vertex_info, edge_info>;

using vertex = digraph::vertex_descriptor; // Define Vertex
using vertex_itr = digraph::vertex_iterator; // Define Vertex iterator

using edge = digraph::edge_descriptor;
using edge_itr = digraph::edge_iterator;

using adj_itr = digraph::adjacency_iterator;

using out_edge_itr = digraph::out_edge_iterator;
using in_edge_itr = digraph::in_edge_iterator;

using color_map = std::map<vertex, boost::default_color_type>;

using edge_map = std::map<edge, double>;
}

//edge_itr eit, eit_end;
//vertex_itr vit, vit_end;

#endif /* BASE_GRAPH_H_ */
