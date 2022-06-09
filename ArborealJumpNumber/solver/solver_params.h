/*
 * solver_params.h
 *
 *  Created on: 22 de fev de 2021
 *      Author: evellyn
 */

#ifndef SOLVER_SOLVER_PARAMS_H_
#define SOLVER_SOLVER_PARAMS_H_

#include <set>

enum solver_type { PLAIN, BC, POLY };
enum user_cut { SIGMA, PI, SIGMA_PI };

namespace solver {
	struct solver_params {
		double time_limit;
		double tree_memory;
		solver_type solver;
		std::set<user_cut> cuts;

		solver_params() {
			time_limit = 600;
			tree_memory = 2000;
			solver = PLAIN;
		}

		solver_params(double time_limit, double memory_tree, solver_type solver,
				std::set<user_cut> cuts) :
			time_limit(time_limit),
			tree_memory(memory_tree),
			solver(solver),
			cuts(cuts) {}
	};
}


#endif /* SOLVER_SOLVER_PARAMS_H_ */
