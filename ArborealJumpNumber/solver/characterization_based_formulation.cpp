#include "../headers/characterization_based_formulation.h"



Answer CharacterizationBasedFormulation::Solve(int timeLimit, double heuristics) {
    std::cout << "AddConstraintsAndObjective" << std::endl;

	try {
		//TODO: place a parameter to receive s value

		cplex.exportModel("CharacterizationBasedFormulation.lp");
		cplex.extract(model);
		if (cplex.solve()) {
			std::cout << std::endl << std::endl;
			std::cout << "================================================\n";
			std::cout << "================ SOLUTION FOUND ================\n";
			std::cout << "================================================\n";
			std::cout << "# Solution status: " << cplex.getStatus() << std::endl;
			std::cout << "# Arboreal jump number = " << cplex.getObjValue() << std::endl;
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
		std::cerr << "caiu aqui: " << ex << std::endl;
		std::cerr << "Error: " << ex << std::endl;
	}
	catch (...) {
		std::cerr << "Error" << std::endl;
	}

	return Answer();
}

void CharacterizationBasedFormulation::AddConstraintsAndObjective() {
    std::cout << "AddConstraintsAndObjective" << std::endl;

	model.add(r[index_parser_m[0][0]] == 1);
	for (int t = 0; t <= s; t++) {
		IloExpr sum_r(env);
		for (int i = 0; t < instance.num_vertices; i++) {
			auto idx = index_parser_ns[i][t];
			sum_r += x[idx];
		}
		model.add(sum_r == 1);
	}

	for (int i = 0; i < instance.num_vertices; i++) {
		IloExpr sum_x(env);
		for (int t = 0; t <= s; t++) {
			auto idx = index_parser_ns[i][t];
			sum_x += x[idx];
		}
		model.add(sum_x == 1);
	}

	for (int t = 0; t <= s; t++) {
		for (int i = 0; i < instance.num_vertices; i++) {
			for (int j = 0; j < (s + 1); j++) {
				auto idx_it = index_parser_ns[i][t];
				auto idx_jt = index_parser_ns[j][t];
				auto idx_m = index_parser_m[i][j];
				model.add(x[idx_it] >= a[idx_m][t]);
				model.add(x[idx_jt] >= a[idx_m][t]);
				model.add(a[idx_m][t] >= x[idx_jt] + x[idx_it] - 1);
			}			
		}
	}

	for (int i = 1; i < instance.num_vertices; i++) {
		for (int t = 0; t <= s ; t++) {
			auto idx = index_parser_ns[i][t];
			model.add(x[idx] >= r[idx]);
		}
	}
	for (int t = 0; t <= s; t++) {
		IloExpr sum_a(env);
		for (int j = 0; j < instance.num_vertices; j++) {
			for (auto i : instance.covering_predecessors.at(j)) { //TODO: change this to predecessors at covering graph
				auto idx_m = index_parser_m[i][j];
				sum_a += a[idx_m][t];
			}
			auto idx_jt = index_parser_ns[j][t];
			model.add(r[idx_jt] + sum_a == 1);
		}
	}

	for (int j = 0; j < instance.num_vertices; j++) {
		IloExpr sum_a(env);
		for (int t = 0; t <= s; t++) {
			for (int i : instance.covering_predecessors.at(j)) { //TODO: change this to predecessors at covering graph
				auto idx_m = index_parser_m[i][j];
				sum_a += a[idx_m][t];
			}
		}
		model.add(sum_a <= 1);
	}

	for (int t = 1; t <= s; t++) {
		IloExpr sum_f(env);
		for (int i = 0; i < instance.num_vertices; i++) {
			auto idx_it = index_parser_ns[i][t];
			sum_f += f[idx_it];
		}
		model.add(sum_f == 1);
	}

	for (int u = 1; u <= s; u++) {
		for (int i = 0; i < instance.num_vertices; i++) {
			IloExpr sum_x(env);
			for (int t = 0; t < u; t++) {
				auto idx_it = index_parser_ns[i][t];
				sum_x += x[idx_it];
			}
			auto idx_iu = index_parser_ns[i][u];
			model.add(f[idx_iu] <= sum_x);
		}
	}

	for (int i = 0; i < instance.num_vertices; i++) {
		for (int j = 0; j < instance.num_vertices; j++) {
			for (int t = 1; t <= s; t++) {
				auto idx_it = index_parser_ns[i][t];
				auto idx_jt = index_parser_ns[j][t];
				int d_ij = instance.exist_edge_in_covering_graph(i, j);

				model.add(f[idx_it] + r[idx_jt] + d_ij <= 2);
			}
		}
	}

	for (int i = 0; i < instance.num_vertices; i++) {
		for (int t = 1; t <= s; t++) {
			for (int u = t + 1; u <= s; u++) {
				auto idx_iu = index_parser_ns[i][u];
				auto idx_it = index_parser_ns[i][t];

				model.add(w[idx_it][u] <= f[idx_iu]);
				model.add(w[idx_it][u] <= x[idx_it]);
				model.add(w[idx_it][u] >= f[idx_iu] + x[idx_it] - 1);

			}
		}
	}

	for (int t = 1; t <= s; t++) {
		for (int u = t + 1; u <= s; u++) {
			auto idx_tu = index_parser_s2[t][u];
			IloExpr sum_w(env);
			for (int i = 0; i < instance.num_vertices; i++) {
				auto idx_it = index_parser_ns[i][t];
				sum_w += w[idx_it][u];
			}
			model.add(y[idx_tu] == sum_w);
		}
	}

	for (int i = 0; i < instance.num_vertices; i++) {
		for (int t = 1; t <= s; t++) {
			for (int u = t + 1; u <= s; u++) {
				auto idx_tu = index_parser_s2[t][u];
				auto idx_it = index_parser_ns[i][t];
				auto idx_iu = index_parser_ns[i][u];

				model.add(z[idx_it][u] <= y[idx_tu]);
				model.add(z[idx_it][u] <= g[idx_it]);
				model.add(z[idx_it][u] >= y[idx_tu] + g[idx_it] - 1);
			}
		}
	}

	for (int i = 0; i < instance.num_vertices; i++) {
		for (int u = 1; u <= s; u++) {
			IloExpr sum_z(env);
			for (int t = 0; t < u; t++) {
				auto idx_it = index_parser_ns[i][t];
				sum_z += w[idx_it][u];
			}
			auto idx_iu = index_parser_ns[i][u];
			model.add(g[idx_iu] == f[idx_iu] + sum_z);
		}
	}

	for (int i = 0; i < instance.num_vertices; i++) {
		for (int t = 1; t <= s; t++) {
			for (int u = t + 1; u <= s; u++) {
				auto idx_tu = index_parser_s2[t][u];
				auto idx_it = index_parser_ns[i][t];
				auto idx_iu = index_parser_ns[i][u];

				model.add(q[idx_it][u] <= x[idx_iu]);
				model.add(q[idx_it][u] <= g[idx_it]);
				model.add(q[idx_it][u] >= x[idx_iu] + g[idx_it] - 1);
			}
		}
	}

	for (int i = 0; i < instance.num_vertices; i++) {
		for (int t = 1; t <= s; t++) {
			for (int u = t + 1; u <= s; u++) {
				auto idx_tu = index_parser_s2[t][u];
				auto idx_it = index_parser_ns[i][t];
				auto idx_iu = index_parser_ns[i][u];

				model.add(q[idx_it][u] <= x[idx_iu]);
				model.add(q[idx_it][u] <= g[idx_it]);
				model.add(q[idx_it][u] >= x[idx_iu] + g[idx_it] - 1);
			}
		}
	}

	for (auto const e : boost::make_iterator_range(boost::edges(instance.covering_graph))) {
		auto i = instance.covering_graph[e].source_id;
		auto j = instance.covering_graph[e].target_id;
		for (int t = 1; t <= s; t++) {
			for (int u = t + 1; u <= s; u++) {
				IloExpr sum_q(env);
				for (int k : instance.predecessors.at(i)) {
					auto idx_kt = index_parser_m[k][t];
					sum_q += q[idx_kt][u];
				}
				auto idx_iu = index_parser_ns[i][u];
				auto idx_jt = index_parser_ns[j][t];

				model.add(x[idx_iu] + x[idx_jt] <= 1 + sum_q);
			}
		}		
	}

}


void CharacterizationBasedFormulation::SetInitialSolution() {
    std::cout << "SetInitialSolution" << std::endl;
}
