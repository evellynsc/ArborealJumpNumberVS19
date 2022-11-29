/*
 * solver.cpp
 *
 *  Created on: 22 de fev de 2021
 *      Author: evellyn
 */

#include "BCSolver.h"



#include <boost/graph/graphviz.hpp>

#include "../base/solution.h"
#include "callbacks/pi_inequality.h"
#include "callbacks/sigma_inequality.h"
#include "callbacks/pi_sigma_inequality.h"
#include "callbacks/precedence_inequality.h"
#include "callbacks/reachability.h"
#include "callbacks/add_min_cuts_lc.h"
#include "callbacks/add_min_cuts_uc.h"

namespace solver {
//BCSolver::BCSolver(solver_params& config_, ExponentialModel& model_) {
//	solver(config_, model_);
//	model = model_;
//}

//void BCSolver::setup_cplex() {
//	cplex_solver.setParam(IloCplex::Param::Preprocessing::Presolve, CPX_ON);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::HeuristicFreq, CPX_ON);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::RINSHeur, CPX_ON);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::FPHeur, CPX_ON);
////	cplex.setParam(IloCplex::Param::Preprocessing::Linear, 0);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::Search, CPX_MIPSEARCH_TRADITIONAL);
//	cplex_solver.setParam(IloCplex::Param::Threads, 4);
//	cplex_solver.setParam(IloCplex::Param::TimeLimit, config.time_limit);
//	cplex_solver.setParam(IloCplex::Param::MIP::Limits::TreeMemory, config.tree_memory);
//}
//
//

/* void BCSolver::solve(ajns::properties &p) {
	solve();
	p.num_jumps = num_jumps;
} */

void BCSolver::solve() {
	try {
		setup_cplex();
		model.create();
		auto env = model.get_cplex_env();
//		TODO: this can not be done by dynamic cast! FIX IT!
		auto x = model.get_variables_x();
		auto problem_instance = model.get_ajnp_instance();



		if (model.get_type() == RELAXED_CUTSET) {
/*APAGAR ACHO//			cplex.use(separate_pi_inequalities(env, x, problem_instance));
//			cplex.use(separate_pi_sigma_inequalities(env, x, problem_instance));
//			cplex.use(separate_sigma_inequalities(env, x, problem_instance));*/


//			cplex_solver.use(separate_precedence_inequalities(env, x, problem_instance));
//			cplex_solver.use(find_constraints_for_integral_solution(env, x, problem_instance));
			cplex_solver.use(add_min_cuts_lc(env, x, problem_instance));
			cplex_solver.use(add_min_cuts_uc(env, x, problem_instance));
		}


		if (cplex_solver.solve()) {
			num_jumps = cplex_solver.getObjValue();
			std::cout << std::endl << std::endl;
			std::cout << "================================================\n";
			std::cout << "================ SOLUTION FOUND ================\n";
			std::cout << "================================================\n";
			std::cout << "# Solution status: " << cplex_solver.getStatus() << std::endl;
			std::cout << "# Arboreal jump number = " << cplex_solver.getObjValue() << std::endl;
			std::cout << "================================================\n";
			std::cout << "================================================\n";
			std::cout << "================================================\n";
			//		dot -Tps filename.dot -o outfile.ps

			std::cout << "estou aqui?\n";

			IloNumArray x_values(model.get_cplex_model().getEnv());
			cplex_solver.getValues(x_values, x);
			auto edges = std::vector<bool>();
			for (auto i = 0; i < x_values.getSize(); i++) {
				if (x_values[i] > 1e-6) edges.push_back(true);
				else edges.push_back(false);
			}
			auto final_solution = ajns::solution::construct_from_edges(problem_instance.input_graph, edges);
			for (auto e : boost::make_iterator_range(boost::edges(final_solution))) {
				auto head = final_solution[e].source_id;
				auto tail = final_solution[e].target_id;
				std::cout << head << ',' << tail << ' ' << final_solution[e].type << std::endl;
			}


			boost::print_graph(problem_instance.covering_graph);
			std::cout << "TESTE" << std::endl;
			std::ofstream outFile;
			auto name_file = problem_instance.id + "_output.dot";
			outFile.open(name_file);
			boost::write_graphviz(outFile, final_solution,
							boost::make_label_writer(boost::get(&my_graph::vertex_info::id, final_solution)),
							boost::make_label_writer(boost::get(&my_graph::edge_info::type, final_solution)));
			outFile.close();
//			auto command = "dot -Tps " + name_file + " -o " + problem_instance.id + "_output.ps";
//			std::system(command.c_str());
		} else {
			std::cout << "============================\n";
			std::cout << "!!!! SOLUTION NOT FOUND !!!!\n";
			std::cout << "============================\n";
		}

		cplex_solver.exportModel("my_model.lp");

	} catch (IloException &ex) {
		std::cerr << "Error: " << ex << std::endl;
	} catch (...) {
		std::cerr << "Error" << std::endl;
	}

}



}/* namespace ajns */
