/*
 * solver.cpp
 *
 *  Created on: 22 de fev de 2021
 *      Author: evellyn
 */

#include "solver.h"

namespace solver {
solver::~solver() {
	cplex_solver.end();
}

solver::solver() {
	num_jumps = 0;
}

solver::solver(solver_params &solver_config,  Model* model) {
	this->config = solver_config;
	this->num_jumps = 0;
	cplex_solver = IloCplex(model->get_cplex_model());
}

void solver::setup_cplex() {
// https://www.ibm.com/docs/ru/icos/20.1.0?topic=parameters-mip-dynamic-search-switch
//	cplex_solver.setParam(IloCplex::Param::Preprocessing::Presolve, CPX_ON);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::HeuristicFreq, CPX_ON);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::RINSHeur, CPX_ON);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::FPHeur, CPX_ON);
//	cplex.setParam(IloCplex::Param::Preprocessing::Linear, 0);
//	cplex_solver.setParam(IloCplex::Param::MIP::Strategy::Search, CPX_MIPSEARCH_TRADITIONAL);
	cplex_solver.setParam(IloCplex::Param::Threads, 4);
	cplex_solver.setParam(IloCplex::Param::TimeLimit, config.time_limit);
	cplex_solver.setParam(IloCplex::Param::MIP::Limits::TreeMemory, config.tree_memory);
}
//
//

void solver::solve(ajns::properties &p) {
	std::cout << "solver.cpp\n";
	solve();
	p.num_jumps = num_jumps;
}

/* void solver::solve() {
	try {
		setup_cplex();
		ajnp_model->create();
		auto env = this->ajnp_model->get_cplex_env();
//		TODO: this can not be done by dynamic cast! FIX IT!
		auto x = dynamic_cast<ExponentialModel*>(this->ajnp_model)->get_variables_x();
		auto problem_instance = this->ajnp_model->get_ajnp_instance();



		if (this->ajnp_model->get_type() == RELAXED_CUTSET) {
//			cplex.use(separate_pi_inequalities(env, x, problem_instance));
//			cplex.use(separate_pi_sigma_inequalities(env, x, problem_instance));
//			cplex.use(separate_sigma_inequalities(env, x, problem_instance));
			cplex_solver.use(separate_precedence_inequalities(env, x, problem_instance));
			cplex_solver.use(find_constraints_for_integral_solution(env, x, problem_instance));
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
			IloNumArray x_values(ajnp_model->get_cplex_model().getEnv());
			cplex_solver.getValues(x_values, x);
			auto edges = std::vector<bool>();
			for (auto i = 0; i < x_values.getSize(); i++) {
				if (x_values[i] > 1e-6) edges.push_back(true);
				else edges.push_back(false);
			}
			auto final_solution = ajns::solution::construct_from_edges(problem_instance.input_graph, edges);
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

	} catch (IloException &ex) {
		std::cerr << "Error: " << ex << std::endl;
	} catch (...) {
		std::cerr << "Error" << std::endl;
	}

} */



}/* namespace ajns */
