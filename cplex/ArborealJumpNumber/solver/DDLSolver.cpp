#include "DDLSolver.h"
#include <boost/graph/graphviz.hpp>
#include "../base/solution.h"

namespace solver {
	void DDLSolver::solve()
	{
		try {
			std::cout << "mfsolver.cpp\n";
			setup_cplex();
			model.create();
			auto env = model.get_cplex_env();
			cplex_solver.exportModel("not_found.lp");
			cplex_solver.extract(model.get_cplex_model());
			if (cplex_solver.solve()) {
				auto x = model.get_x_variables();
				auto problem_instance = model.get_ajnp_instance();
				auto n = problem_instance.num_vertices;

				std::cout << std::endl << std::endl;
				std::cout << "================================================\n";
				std::cout << "================ SOLUTION FOUND ================\n";
				std::cout << "================================================\n";
				std::cout << "# Solution status: " << cplex_solver.getStatus() << std::endl;
				std::cout << "# Arboreal jump number = " << cplex_solver.getObjValue() << std::endl;
				std::cout << "================================================\n";
				std::cout << "================================================\n";
				std::cout << "================================================\n";

			

			

				//for (auto e : boost::make_iterator_range(boost::edges(problem_instance.input_graph))) {
				//	auto i = problem_instance.input_graph[e].source_id;
				//	auto j = problem_instance.input_graph[e].target_id;

				//	/*if (cplex_solver.getValue(model.x[n*i+j]) > 1e-6) {
				//		std::cout << i << ", " << j  << ", " << std::endl;
				//	}*/
				//}
				for (size_t i = 0; i < n; i++) {
					for (size_t j = 0; j < n; j++) {
						if (i != j) {
							if (cplex_solver.getValue(model.x[n * i + j]) > 1e-6) {
								std::cout << i  << ", " << j  << std::endl;
							}
						}
					}
				}


				/*auto edges = std::vector<bool>();
				for (auto i = 0; i < x_values.getSize(); i++) {
					if (x_values[i] > 1e-6) edges.push_back(true);
					else edges.push_back(false);
				}
				auto final_solution = ajns::solution::construct_from_edges(problem_instance.input_graph, edges);
				for (auto e : boost::make_iterator_range(boost::edges(final_solution))) {
					auto head = final_solution[e].source_id;
					auto tail = final_solution[e].target_id;
					std::cout << head << ',' << tail << ' ' << final_solution[e].type << std::endl;
				}*/
			

			}
			else {
				std::cout << "============================\n";
				std::cout << "!!!! SOLUTION NOT FOUND !!!!\n";
				std::cout << "============================\n";
			}
		}
		catch (IloException& ex) {
			std::cerr << "caiu aqui: " << ex << std::endl;
			std::cerr << "Error: " << ex << std::endl;
		}
		catch (...) {
			std::cerr << "Error" << std::endl;
		}
	}

} /* namespace solver */