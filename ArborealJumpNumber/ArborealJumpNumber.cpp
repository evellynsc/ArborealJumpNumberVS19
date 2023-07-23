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
#include "preprocessing/reader.h"
#include "preprocessing/instance_generator.h"
#include "headers/instance.h"

#include "headers/properties.h"

#include "headers/instance_solver.h"


#include "windows.h"
#include "psapi.h"






int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cout << "FALTANDO ARGUMENTOS\n" << std::endl;
		return 1;
	}


	auto input_file = reader(argv[1]);
	auto problem_data = input_file.read();
	auto generator = instance_generator();
	auto my_instance = generator.create_instance(problem_data);

	auto start = std::chrono::high_resolution_clock::now();
	auto prop = ajns::properties();

	prop.num_arcs = my_instance.num_edges;
	prop.num_nodes = my_instance.num_vertices;
	int a;
	std::cin >> a;

	//exit(1);

	if (argv[2] == std::string("c")) {
		InstanceSolver solver = InstanceSolver::GetInstance(argv[1], "characterizationbased", int(argv[3]));
		solver.Solve("teste.out", 2, 2);
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
