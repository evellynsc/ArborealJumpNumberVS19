//============================================================================
// Name        : main.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define _HAS_STD_BYTE 0
#include <iostream>
#include <chrono>
#include <cstring>
#include <cstdlib>

#include "preprocessing/reader.h"
#include "preprocessing/instance_generator.h"
#include "base/properties.h"
#include "base/instance.h"
#include "heuristic/minimal_extension.h"
#include "solver/solver.h"
#include "solver/exponential_model.h"
#include "solver/BCSolver.h"
#include "solver/ddl_model.h"
#include "solver/DDLSolver.h"
#include "solver/multi_flow_model.h"
#include "solver/MFSolver.h"
#include "solver/CharacterizationBasedFormulation.h"
#include "solver/CharacterizationBasedSolver.h"

#include "solver/Kernel.h"

// IloCplex:: getNcols num of variables
// IloCplex:: getNrows num of constraints

int main(int argc, char* argv[]) {
	// ./ajns file algo_type relaxed [others]
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
	auto relaxed = false;
	if (std::atoi(argv[3]) == 1)
		relaxed = true;
	//int a;
	//std::cin >> a;

	//exit(1);
	int opt = std::atoi(argv[2]);
	switch (opt) {
	case 1:
	{
		prop.algo_t = algo_type::HH;
		auto extension = ajns::minimal_extension(my_instance);
		extension.run(prop);
		break;
	}
	case 2:
	{
		prop.algo_t = algo_type::BRANCH_AND_CUT;
		auto solver_config = solver::solver_params();
		auto exp_model = solver::ExponentialModel(my_instance, solver::RELAXED_CUTSET);
		solver::solver* ajnp_solver = new solver::BCSolver(solver_config, exp_model);
		try {
			std::cout << "entrando solver\n";
			ajnp_solver->solve(prop);
		}
		catch (...) {
			std::cout << "algo de errado não está certo\n";
		}
		break;
	}
	case 3:
	{
		prop.algo_t = algo_type::MFLOW;
		auto solver_config = solver::solver_params();
		auto exp_model = solver::MultiFlowModel(my_instance, relaxed);
		solver::solver* ajnp_solver = new solver::MFSolver(solver_config, exp_model);
		try {
			ajnp_solver->solve(prop);
		}
		catch (...) {
			std::cout << "algo de errado não está certo\n";
		}
		break;
	}
	case 4:
	{
		prop.algo_t = algo_type::DDL;
		auto solver_config = solver::solver_params();
		auto model = solver::DDLModel(my_instance, relaxed);
		solver::solver* ajnp_solver = new solver::DDLSolver(solver_config, model);
		try {
			ajnp_solver->solve(prop);
		}
		catch (...) {
			std::cout << "algo de errado não está certo\n";
		}
		break;
	}
	case 5:
	{
		prop.algo_t = algo_type::CHARACTERIZATION;
		auto solver_config = solver::solver_params();
		std::cout << "CHARACTERIZATION\n";
		auto ascending = true;
		if (std::atoi(argv[4]) == 0)
			ascending = false;
		//auto s = atoi(argv[3]);
		auto s = 1;
		auto run_next = true;
		if (ascending) {
			while (run_next and s != my_instance.num_vertices) {
				auto model = solver::CharacterizationBasedFormulation(my_instance, s, relaxed);
				solver::solver* ajnp_solver = new solver::CharacterizationBasedSolver(solver_config, model);
				try {
					ajnp_solver->solve(prop);
				}
				catch (...) {
					std::cout << "algo de errado não está certo\n";
				}
				std::cout << ajnp_solver->get_status() << std::endl;
				run_next = (ajnp_solver->get_status() == 1 || ajnp_solver->get_status() == 2) ? false : true;
				s++;
			}
		} else {
			s =  std::atoi(argv[5]);
			while (run_next and s != 0) {
				auto model = solver::CharacterizationBasedFormulation(my_instance, s, relaxed);
				solver::solver* ajnp_solver = new solver::CharacterizationBasedSolver(solver_config, model);
				try {
					ajnp_solver->solve(prop);
				}
				catch (...) {
					std::cout << "algo de errado não está certo\n";
				}
				std::cout << ajnp_solver->get_status() << std::endl;
				run_next = (ajnp_solver->get_status() == 1 || ajnp_solver->get_status() == 2) ? false : true;
				s--;
			}
		}
		break;
	}
	case 6:
	{
		Kernel* search = new Kernel(my_instance, "flow", 4);
		search->run();
		break;
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
