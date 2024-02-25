#include "CharacterizationBasedSolver.h"


#include <boost/graph/graphviz.hpp>
namespace solver {


CharacterizationBasedSolver::CharacterizationBasedSolver(){}
void CharacterizationBasedSolver::solve()
{
	try {

		setup_cplex();
		std::cout << "creating characterizatoin based formulation...\n";
		model.create();

		auto env = model.get_cplex_env();
		cplex_solver.exportModel("characterization_model.lp");
		cplex_solver.extract(model.get_cplex_model());
		auto a = model.get_a_variables();
		auto x = model.get_x_variables();
		auto r = model.get_r_variables();
		auto f = model.get_f_variables();
		auto g = model.get_g_variables();
		if (cplex_solver.solve()) {

			auto problem_instance = model.get_ajnp_instance();
			auto n = problem_instance.num_vertices;
			auto s = model.get_s();

			std::cout << std::endl << std::endl;
			std::cout << "================================================\n";
			std::cout << "================ SOLUTION FOUND ================\n";
			std::cout << "================================================\n";
			std::cout << "# Solution status: " << cplex_solver.getStatus() << std::endl;
			std::cout << "# Arboreal jump number = " << s << std::endl;
			std::cout << "================================================\n";
			std::cout << "================================================\n";
			std::cout << "================================================\n";

			IloNumArray values(env);
			cplex_solver.getValues(values, f);
			env.out() << "Values = " << values << std::endl;

			for (int i = 0; i < n; i++) {
				for (int t = 0; t <= s; t++) {
					if (cplex_solver.getValue(x[model.idx_ns(i, t)]) > 1e-6) {
						std::cout << "x " << i << ", " << t << " = " << cplex_solver.getValue(x[model.idx_ns(i, t)]);
						if (cplex_solver.getValue(x[model.idx_ns(i, t)]) > 0.5) {
							std::cout << " *";
						}
						std::cout << std::endl;
					}
				}
			}

			auto _roots = std::map<int, int>();
			for (int i = 0; i < n; i++) {
				for (int t = 0; t <= s; t++) {
					if (cplex_solver.getValue(r[model.idx_ns(i, t)]) > 1e-6) {
						std::cout << "r " << i << ", " << t << " = " << cplex_solver.getValue(r[model.idx_ns(i, t)]) << std::endl;
						_roots.insert({ t,i });
					}
				}
			}

			std::vector<pair<int, int>> _selected_edges;

			for (int i = 0; i < n; i++) {
				for (int t = 0; t <= s; t++) {
					if (cplex_solver.getValue(f[model.idx_ns(i, t)]) > 1e-6) {
						std::cout << "f " << i << ", " << t << " = " << cplex_solver.getValue(f[model.idx_ns(i, t)]) << std::endl;
						_selected_edges.push_back({i, _roots[t]});
					}
				}
			}

			for (int i = 0; i < n; i++) {
				for (int t = 0; t <= s; t++) {
					if ((t > 0) and (cplex_solver.getValue(g[model.idx_ns(i, t)]) > 1e-6)) {
						std::cout << "g " << i << ", " << t << " = " << cplex_solver.getValue(g[model.idx_ns(i, t)]) << std::endl;
					}
				}
			}



			for (auto e : boost::make_iterator_range(boost::edges(problem_instance.covering_graph))) {
				auto i = problem_instance.input_graph[e].source_id;
				auto j = problem_instance.input_graph[e].target_id;
				for (int t = 0; t <= s; t++) {
					if (cplex_solver.getValue(a[n * i + j][t]) > 1e-6) {
						std::cout << "a, " << i << ", " << j << ", " << t << " = " << cplex_solver.getValue(a[n * i + j][t]) << std::endl;
						_selected_edges.push_back({i,j});
					}
				}
			}

			solution = new Solution(s, problem_instance.num_vertices, _selected_edges);
			solution->save_solution("teste", "dot");
			status = 1;


			solution = new Solution(s, problem_instance.num_vertices, _selected_edges);
			solution->save_solution("teste", "dot");
			status = 1;


			/*for (size_t i = 0; i < n; i++) {
				for (size_t j = 0; j < n; j++) {
					if (i != j) {
						if (cplex_solver.getValue(model.x[n * i + j]) > 1e-6) {
							std::cout << i << ", " << j << std::endl;
						}
					}
				}
			}*/


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

			if (cplex_solver.getStatus() == IloAlgorithm::InfeasibleOrUnbounded || cplex_solver.getStatus() == IloAlgorithm::Infeasible)
				status = 0;
			else
				status = 2;

			//if ((cplex_solver.getStatus() == IloAlgorithm::Infeasible) ||
			//	(cplex_solver.getStatus() == IloAlgorithm::InfeasibleOrUnbounded)) {
			//	cout << endl << "No solution - starting Conflict refinement" << endl;

			//	IloConstraintArray infeas(env);
			//	IloNumArray preferences(env);

			//	for (IloInt i = 0; i < infeas.getSize(); i++) {
			//		preferences.add(1.0);  // user may wish to assign unique preferences
			//	}

			//	if (cplex_solver.refineConflict(infeas, preferences)) {
			//		IloCplex::ConflictStatusArray conflict = cplex_solver.getConflict(infeas);
			//		env.getImpl()->useDetailedDisplay(IloTrue);
			//		std::cout << "Conflict :" << endl;
			//		for (IloInt i = 0; i < infeas.getSize(); i++) {
			//			if (conflict[i] == IloCplex::ConflictMember)
			//				std::cout << "Proved  : " << infeas[i] << endl;
			//			if (conflict[i] == IloCplex::ConflictPossibleMember)
			//				std::cout << "Possible: " << infeas[i] << endl;
			//		}
			//	}
			//	else
			//		std::cout << "Conflict could not be refined" << endl;
			//	std::cout << endl;

			//	solved = false;
			//}
		}
	}
	catch (IloException& ex) {
		std::cerr << "caiu aqui: " << ex << std::endl;
		std::cerr << "Error: " << ex << std::endl;
	}
	/*catch (...) {
		std::cerr << "Error" << std::endl;
	}*/
}

CharacterizationBasedSolver::~CharacterizationBasedSolver() {}
} /* namespace solver */
