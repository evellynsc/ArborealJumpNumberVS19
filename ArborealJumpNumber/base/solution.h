/*
 * solution.h
 *
 *  Created on: 1 de mar de 2021
 *      Author: evellyn
 */

#ifndef BASE_SOLUTION_H_
#define BASE_SOLUTION_H_


#include "graph.h"
namespace ajns::solution {
	my_graph::digraph construct_from_edges(my_graph::digraph &input_graph, std::vector<bool> &selected_edges);

//class solution {
//
//public:
//	solution();
//	digraph construct_graph(digraph &input_graph, std::vector<bool> &selected_edges);
//	virtual ~solution();
//};


//namespace solution {
//} /* namespace solution */

} /* namespace ajns */

#endif /* BASE_SOLUTION_H_ */
