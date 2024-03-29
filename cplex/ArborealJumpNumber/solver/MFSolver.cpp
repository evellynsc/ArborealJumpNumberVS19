/*
 * solver.cpp
 *
 *  Created on: 22 de fev de 2021
 *      Author: evellyn
 */

#include "MFSolver.h"

#include "../base/solution.h"
#include <numeric>


#include <boost/graph/graphviz.hpp>
#include <boost/graph/transitive_closure.hpp>

namespace solver {
//MFSolver::MFSolver() {
//
//}

//MFSolver::MFSolver(solver_params& solver_config, MultiFlowModel& ajnp_model) {
//	model = ajnp_model;
//}

//MFSolver::MFSolver(solver_params& solver_config, MultiFlowModel& ajnp_model) :
//		solver(solver_config, &ajnp_model) {model = ajnp_model;}

//REFACTORE ME, PLX
std::vector<double> MFSolver::get_values_main_variables() {
	auto problem_instance = model.get_ajnp_instance();
	int n = problem_instance.num_vertices;
	std::vector<double> values = std::vector<double>(n * n, 0);
	
	for (auto e : boost::make_iterator_range(boost::edges(problem_instance.input_graph))) {
		auto i = problem_instance.input_graph[e].source_id;
		auto j = problem_instance.input_graph[e].target_id;
		int idx = n * i + j;
		if (cplex_solver.getValue(model.y[idx]) > 1e-6) {			
			values[idx] = cplex_solver.getValue(model.y[idx]);
		}
	}

	return values;
}

//REFACTORE ME, PLX
void MFSolver::reset_model_by_kernel(std::vector<bool> kernel) {
	std::cout << "setting kernel\n";
	auto problem_instance = model.get_ajnp_instance();
	int n = problem_instance.num_vertices;
	int ii = 0;

	for (auto e : boost::make_iterator_range(boost::edges(problem_instance.input_graph))) {
		auto i = problem_instance.input_graph[e].source_id;
		auto j = problem_instance.input_graph[e].target_id;
		int idx = n * i + j;
		if (not kernel[idx]) {
			model.y[idx].setUB(0);
			ii++;
		}
	}
	std::cout << ii << " vari�veis foram fixadas" << std::endl;
}

void MFSolver::solve() {
	try {
		//auto env = model.get_cplex_env();

		if (cplex_solver.solve()) {
			//auto y = model.get_y_variables();
			auto problem_instance = model.get_ajnp_instance();
			auto n = problem_instance.num_vertices;

			num_jumps = n - 1 - cplex_solver.getObjValue();

			double lr = n - 1 - cplex_solver.getObjValue();

			std::cout << std::endl << std::endl;
			std::cout << "================================================\n";
			std::cout << "================ SOLUTION FOUND ================\n";
			std::cout << "================================================\n";
			std::cout << "# Solution status: " << cplex_solver.getStatus() << std::endl;
			std::cout << "# Arboreal jump number = " << lr << std::endl;
			std::cout << "================================================\n";
			std::cout << "================================================\n";
			std::cout << "================================================\n";

			/*std::cout << "covering graph\n";

			for (auto e : boost::make_iterator_range(boost::edges(problem_instance.covering_graph))) {
				auto i = problem_instance.covering_graph[e].source_id;
				auto j = problem_instance.covering_graph[e].target_id;
				std::cout << i+1 << ", " << j+1 << std::endl;
			}

			std::cout << "================================================\n";
			std::cout << "================================================\n";
			std::cout << "================================================\n";*/

			std::cout << "valor de y\n";

			auto solution = my_graph::digraph();

			
			for (auto v : boost::make_iterator_range(boost::vertices(problem_instance.input_graph))) {
				boost::add_vertex(problem_instance.input_graph[v], solution);
			}
			//		add edges
		
			for (auto const& e : boost::make_iterator_range(boost::edges(problem_instance.input_graph))) {
				auto i = problem_instance.input_graph[e].source_id;
				auto j = problem_instance.input_graph[e].target_id;
				if (cplex_solver.getValue(model.y[n*i + j]) > 1e-6) {
					boost::add_edge(i,j,solution);
					std::cout << i << "->" << j << " [label=" << problem_instance.input_graph[e].type << ",value=" << cplex_solver.getValue(model.y[n * i + j]) << "];\n";
					//std::cout << n * i + j << ',' << i + 1 << ", " << j + 1 << " " << cplex_solver.getValue(model.y[n * i + j]) << " " << problem_instance.input_graph[e].type << std::endl;
				}
			}


			auto tc_solution = my_graph::digraph();

			std::map<my_graph::vertex, my_graph::vertex> g_to_tc;
			std::vector<std::size_t> id_map(boost::num_vertices(solution));
			std::iota(id_map.begin(), id_map.end(), 0u);

			boost::transitive_closure(solution, tc_solution, boost::make_assoc_property_map(g_to_tc), id_map.data());

			for (auto& e : g_to_tc)
				tc_solution[e.second] = solution[e.first];

			for (auto const& e : boost::make_iterator_range(boost::edges(problem_instance.order_graph))) {
				auto i = problem_instance.input_graph[e].source_id;
				auto j = problem_instance.input_graph[e].target_id;
				if (not boost::edge(i, j, tc_solution).second) {
					std::cout << "\n === T� ERRADO ===" << std::endl;
				}
			}
			
			/*std::cout << "valor de f\n";
			std::cout << "================================================\n";
			std::cout << "================================================\n";
			std::cout << "================================================\n";

			for (auto k = 0; k < n - 1; k++) {
				for (auto e : boost::make_iterator_range(boost::edges(problem_instance.input_graph))) {
					auto i = problem_instance.input_graph[e].source_id;
					auto j = problem_instance.input_graph[e].target_id;
				
					if (cplex_solver.getValue(model.x[i + j*n + n*n*k]) > 1e-6) {
						std::cout << i + 1 << ", " << j + 1 << ", " << k + 1 << " " << cplex_solver.getValue(model.x[i + j * n + n * n * k]) << std::endl;
					}
				}
			}*/

			/*for (IloInt i = 0; i < n; i++) {
				for (IloInt j = 0; j < n; ++j) {
				std:cout << "HOLA" << endl;
					if (cplex_solver.getValue(y[i][j]) > 1e-6) {
						std::cout << i << "," << j << std::endl;
					}
				}
			}*/
					

			//		dot -Tps filename.dot -o outfile.ps
			/*
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
			outFile.close();*/
			//			auto command = "dot -Tps " + name_file + " -o " + problem_instance.id + "_output.ps";
			//			std::system(command.c_str());
		}
		else {
			std::cout << "============================\n";
			std::cout << "!!!! SOLUTION NOT FOUND !!!!\n";
			std::cout << "============================\n";
		}

	}
	catch (IloException& ex) {
		std::cerr << "Error: " << ex << std::endl;
	}
	catch (...) {
		std::cerr << "Error" << std::endl;
	}

}



}/* namespace ajns */
