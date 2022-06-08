/*
 * properties.h
 *
 *  Created on: 1 de jun de 2021
 *      Author: evellyn
 */

#ifndef BASE_PROPERTIES_H_
#define BASE_PROPERTIES_H_


#include <iostream>
enum algo_type { GREEDY, BRANCH_AND_CUT, MFLOW };

namespace ajns {
struct properties {
	std::size_t num_nodes;
	std::size_t num_arcs;
	std::size_t num_violators;
	std::size_t num_jumps;
	double run_time;
	algo_type algo_t;

	properties () {
		num_nodes = 0;
		num_arcs = 0;
		num_violators = 0;
		num_jumps = 0;
		run_time = 0;
		algo_t = GREEDY;
	}

	properties (std::size_t n_nodes, std::size_t n_arcs, std::size_t n_violators, std::size_t n_jumps, double run_time, algo_type algo) :
		num_nodes(n_nodes), num_arcs(n_arcs), num_violators(n_violators), num_jumps(n_jumps), run_time(run_time), algo_t(algo) {}
};
};


#endif /* BASE_PROPERTIES_H_ */
