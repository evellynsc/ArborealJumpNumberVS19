#include "DDLSolver.h"

void solver::DDLSolver::solve()
{
	try {
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
