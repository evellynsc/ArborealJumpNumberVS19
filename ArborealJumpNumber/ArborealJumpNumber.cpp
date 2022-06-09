//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <chrono>
#include <cstring>
#include "preprocessing/reader.h"
#include "preprocessing/instance_generator.h"
#include "base/instance.h"
#include "solver/solver.h"
#include "solver/BCSolver.h"
#include "solver/MFSolver.h"
#include "solver/exponential_model.h"
#include "solver/multi_flow_model.h"
#include "heuristic/minimal_extension.h"
#include "base/properties.h"


int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cout << "FALTANDO ARGUMENTOS\n" << std::endl;
		return 1;
	}

	auto input_file = ajns::reader(argv[1]);
	auto problem_data = input_file.read();
	auto generator = ajns::instance_generator();
	auto my_instance = generator.create_instance(problem_data);

	auto start = std::chrono::high_resolution_clock::now();
	auto prop = ajns::properties();

	prop.num_arcs = my_instance.num_edges;
	prop.num_nodes = my_instance.num_vertices;

	if (argv[2] == std::string("h")) {
		prop.algo_t = algo_type::HH;
		auto extension = ajns::minimal_extension(my_instance);
		extension.run(prop);
	}
	else {
		if (argv[2] == std::string("b")) {
			prop.algo_t = algo_type::BRANCH_AND_CUT;
			auto solver_config = solver::solver_params();
			auto exp_model = solver::ExponentialModel(my_instance);
			solver::solver* ajnp_solver = new solver::BCSolver(solver_config, exp_model);
			try {
				ajnp_solver->solve(prop);
			}
			catch (...) {
				std::cout << "algo de errado não está certo\n";
			}
		}
		else {
			if (argv[2] == std::string("f")) {
				prop.algo_t = algo_type::MFLOW;
				auto solver_config = solver::solver_params();
				auto exp_model = solver::MultiFlowModel(my_instance);
				solver::solver* ajnp_solver = new solver::MFSolver(solver_config, exp_model);
				try {
					ajnp_solver->solve(prop);
				}
				catch (...) {
					std::cout << "algo de errado não está certo\n";
				}
			}
		}

	}
	auto end = std::chrono::high_resolution_clock::now();
	double run_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); // @suppress("Invalid arguments") // @suppress("Symbol is not resolved") // @suppress("Method cannot be resolved")
	run_time *= 1e-9;

	prop.run_time = run_time;

	auto outFile = std::ofstream();
	auto name_file = "results" + std::to_string(prop.algo_t) + ".txt";
	outFile.open(name_file, std::ofstream::out | std::ofstream::app);

	outFile << my_instance.id << " " << prop.num_nodes << " " << prop.num_arcs << " " <<
		prop.num_violators << " " << prop.num_jumps << " " << fixed << prop.run_time << setprecision(9) << std::endl; // @suppress("Invalid overload")

	std::cout << "time === " << fixed << run_time << setprecision(9) << " ===\n";
	return 0;
}
