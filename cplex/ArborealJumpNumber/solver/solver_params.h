/*
 * solver_params.h
 *
 *  Created on: 22 de fev de 2021
 *      Author: evellyn
 */

#ifndef SOLVER_SOLVER_PARAMS_H_
#define SOLVER_SOLVER_PARAMS_H_

#include <set>

enum solver_type { PLAIN, EXPONENCIAL, BC, MULTIFLOW };
enum user_cut { SIGMA, PI, SIGMA_PI };

namespace solver {
	struct solver_params {
		double time_limit;
		double tree_memory;
		solver_type solver;
		std::set<user_cut> cuts;
		bool add_initial_solution;

		solver_params() {
			time_limit = 18000;
			tree_memory = 20000;
			solver = PLAIN;
			add_initial_solution = false;
		}

		solver_params(double time_limit, double memory_tree, solver_type solver) :
			time_limit(time_limit),
			tree_memory(memory_tree),
			solver(solver) {}

		solver_params(double time_limit, double memory_tree, solver_type solver,
				std::set<user_cut> cuts, bool add_initial_solution) :
			time_limit(time_limit),
			tree_memory(memory_tree),
			solver(solver),
			cuts(cuts),
			add_initial_solution(add_initial_solution) {}
	};
}


#endif /* SOLVER_SOLVER_PARAMS_H_ */
