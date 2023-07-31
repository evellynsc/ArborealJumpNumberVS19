#include "CharacterizationBasedFormulation.h"

#include <cmath>

namespace solver {
	CharacterizationBasedFormulation::CharacterizationBasedFormulation() {
		this->type = CHARACTERIZATION;
		this->s = 0;
	}

	CharacterizationBasedFormulation::CharacterizationBasedFormulation(ajns::instance& problem_instance, int s) {
		this->type = CHARACTERIZATION;
		this->problem_instance = problem_instance;
		this->s = s;
	}

	CharacterizationBasedFormulation::CharacterizationBasedFormulation(ajns::instance& problem_instance, int s, bool _linear_relaxation) {
		this->type = CHARACTERIZATION;
		this->linear_relaxation = _linear_relaxation;
		this->problem_instance = problem_instance;
		this->s = s;
	}

	//IloNumVarArray CharacterizationBasedFormulation::get_v_variables() {
	//	return y;
	//}

	IloNumVarArray CharacterizationBasedFormulation::get_x_variables() {
		return x;
	}

	IloArray<IloNumVarArray> CharacterizationBasedFormulation::get_a_variables() {
		return a;
	}

	IloNumVarArray CharacterizationBasedFormulation::get_r_variables() {
		return r;
	}

	int CharacterizationBasedFormulation::get_s() {
		return s;
	}

	IloNumVarArray CharacterizationBasedFormulation::get_f_variables() {
		return f;
	}

	IloNumVarArray CharacterizationBasedFormulation::get_g_variables() {
		return g;
	}

	void CharacterizationBasedFormulation::add_variables() {
		auto m = problem_instance.num_vertices * problem_instance.num_vertices;

		auto s1 = s + 1u;
		auto ns = problem_instance.num_vertices * s1;
		auto s2 = s1 * s1;

		index_parser_ns = vector<vector<int>>(problem_instance.num_vertices);
		index_parser_m = vector<vector<int>>(problem_instance.num_vertices);
		index_parser_s2 = vector<vector<int>>(s1);


		for (int i = 0; i < problem_instance.num_vertices; i++) {
			index_parser_m[i] = vector<int>(problem_instance.num_vertices);
			for (int j = 0; j < problem_instance.num_vertices; j++) {
				index_parser_m[i][j] = i * problem_instance.num_vertices + j;
			}
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			index_parser_ns[i] = vector<int>(s1);
			for (int t = 0; t <= s; t++) {
				index_parser_ns[i][t] = t * problem_instance.num_vertices + i;
			}
		}

		for (int u = 0; u <= s; u++) {
			index_parser_s2[u] = vector<int>(s1);
			for (int t = 0; t <= s; t++) {
				index_parser_s2[u][t] = t * s1 + u;
			}
		}
		if (linear_relaxation) {
			x = IloNumVarArray(env, ns, 0.0, 1.0, ILOFLOAT);
			r = IloNumVarArray(env, ns, 0.0, 1.0, ILOFLOAT);
			f = IloNumVarArray(env, ns, 0.0, 1.0, ILOFLOAT);
			g = IloNumVarArray(env, ns, 0.0, 1.0, ILOFLOAT);

			a = IloArray<IloNumVarArray>(env, m);
			for (int i = 0; i < m; i++) {
				a[i] = IloNumVarArray(env, s1, 0.0, 1.0, ILOFLOAT);
			}

			h = IloArray<IloNumVarArray>(env, m);
			for (int i = 0; i < m; i++) {
				h[i] = IloNumVarArray(env, s2, 0.0, 1.0, ILOFLOAT);
			}

			w = IloArray<IloNumVarArray>(env, ns);
			for (int i = 0; i < ns; i++) {
				w[i] = IloNumVarArray(env, s1, 0.0, 1.0, ILOFLOAT);
			}
		}
		else {
			x = IloNumVarArray(env, ns, 0.0, 1.0, ILOBOOL);
			r = IloNumVarArray(env, ns, 0.0, 1.0, ILOBOOL);
			f = IloNumVarArray(env, ns, 0.0, 1.0, ILOBOOL);
			g = IloNumVarArray(env, ns, 0.0, 1.0, ILOBOOL);

			a = IloArray<IloNumVarArray>(env, m);
			for (int i = 0; i < m; i++) {
				a[i] = IloNumVarArray(env, s1, 0.0, 1.0, ILOBOOL);
			}

			h = IloArray<IloNumVarArray>(env, m);
			for (int i = 0; i < m; i++) {
				h[i] = IloNumVarArray(env, s2, 0.0, 1.0, ILOBOOL);
			}

			w = IloArray<IloNumVarArray>(env, ns);
			for (int i = 0; i < ns; i++) {
				w[i] = IloNumVarArray(env, s1, 0.0, 1.0, ILOBOOL);
			}
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			for (int j = 0; j < problem_instance.num_vertices; j++) {
				for (int t = 0; t < s1; t++) {
					std::string name = "a_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(t);
					auto idx_ij = index_parser_m[i][j];
					a[idx_ij][t].setName(name.c_str());
				}
			}
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			for (int j = 0; j < problem_instance.num_vertices; j++) {
				for (int t = 0; t < s1; t++) {
					for (int u = 0; u < s1; u++) {
						std::string name = "h_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(t) + "_" + std::to_string(u);
						auto idx_ij = index_parser_m[i][j];
						auto idx_tu = index_parser_s2[t][u];
						h[idx_ij][idx_tu].setName(name.c_str());
					}
				}
			}
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			for (int t = 0; t < s1; t++) {
				std::string name = "x_" + std::to_string(i) + "_" + std::to_string(t);
				auto idx = index_parser_ns[i][t];
				x[idx].setName(name.c_str());

				name = "r_" + std::to_string(i) + "_" + std::to_string(t);
				idx = index_parser_ns[i][t];
				r[idx].setName(name.c_str());

				name = "f_" + std::to_string(i) + "_" + std::to_string(t);
				idx = index_parser_ns[i][t];
				f[idx].setName(name.c_str());

				name = "g_" + std::to_string(i) + "_" + std::to_string(t);
				idx = index_parser_ns[i][t];
				g[idx].setName(name.c_str());

				for (int u = 0; u < s1; u++) {
					name = "w_" + std::to_string(i) + "_" + std::to_string(t) + "_" + std::to_string(u);
					idx = index_parser_ns[i][t];
					w[idx][u].setName(name.c_str());
				}
			}
		}
	}

	void CharacterizationBasedFormulation::add_constraints() {
		cplex_model.add(r[index_parser_ns[problem_instance.root][0]] == 1);
	
		for (int t = 0; t <= s; t++) {
			IloExpr sum_r(env);
			for (int i = 0; i < problem_instance.num_vertices; i++) {
				auto idx = index_parser_ns[i][t];
				sum_r += r[idx];
			}
			cplex_model.add(sum_r == 1);
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			IloExpr sum_x(env);
			for (int t = 0; t <= s; t++) {
				auto idx = index_parser_ns[i][t];
				sum_x += x[idx];
			}
			cplex_model.add(sum_x <= 1);
		}

		for (auto const e : boost::make_iterator_range(boost::edges(problem_instance.covering_graph))) {
			auto i = problem_instance.covering_graph[e].source_id;
			auto j = problem_instance.covering_graph[e].target_id;
			for (int t = 0; t <= s; t++) {
				auto idx_it = index_parser_ns[i][t];
				auto idx_jt = index_parser_ns[j][t];
				auto idx_m = index_parser_m[i][j];
				cplex_model.add(x[idx_it] >= a[idx_m][t]);
				cplex_model.add(x[idx_jt] >= a[idx_m][t]);
				cplex_model.add(a[idx_m][t] - x[idx_jt] - x[idx_it] >= - 1);
			}
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			for (int t = 0; t <= s; t++) {
				auto idx = index_parser_ns[i][t];
				cplex_model.add(x[idx] >= r[idx]);
			}
		}


		for (int j = 0; j < problem_instance.num_vertices; j++) {
			if (j != problem_instance.root) {
				IloExpr sum_a(env);
				IloExpr sum_r(env);
				for (int t = 0; t <= s; t++) {
					if (problem_instance.covering_predecessors.at(j).size() > 0) {
						for (auto i : problem_instance.covering_predecessors.at(j)) {
							auto idx_m = index_parser_m[i][j];
							sum_a += a[idx_m][t];
						}
					}
					auto idx_jt = index_parser_ns[j][t];
					sum_r += r[idx_jt];
				}
				cplex_model.add(sum_r + sum_a == 1);
			}
		}

		for (int j = 0; j < problem_instance.num_vertices; j++) {
			if (j != problem_instance.root) {
				IloExpr sum_a(env);
				for (int t = 0; t <= s; t++) {
					for (int i : problem_instance.covering_predecessors.at(j)) {
						auto idx_m = index_parser_m[i][j];
						sum_a += a[idx_m][t];
					}
				}
				cplex_model.add(sum_a <= 1);
			}
		}

// não tem na implementação com ortools
		for (int i = 0; i < problem_instance.num_vertices; i++) {
			auto idx_it = index_parser_ns[i][0];
			cplex_model.add(f[idx_it] == 0);
		}


		for (int t = 1; t <= s; t++) {
			IloExpr sum_f(env);
			for (int i = 0; i < problem_instance.num_vertices; i++) {
				auto idx_it = index_parser_ns[i][t];
				sum_f += f[idx_it];
			}
			cplex_model.add(sum_f == 1);
		}

		for (int u = 1; u <= s; u++) {
			for (int i = 0; i < problem_instance.num_vertices; i++) {
				IloExpr sum_x(env);
				for (int t = 0; t < u; t++) {
					auto idx_it = index_parser_ns[i][t];
					sum_x += x[idx_it];
				}
				auto idx_iu = index_parser_ns[i][u];
				cplex_model.add(f[idx_iu] <= sum_x);
			}
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			for (int j = 0; j < problem_instance.num_vertices; j++) {
				for (int t = 1; t <= s; t++) {
					auto idx_it = index_parser_ns[i][t];
					auto idx_jt = index_parser_ns[j][t];
					int d_ij = problem_instance.exist_edge_in_order_graph(i, j);

					cplex_model.add(f[idx_it] + r[idx_jt] + d_ij <= 2);
				}
			}
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			for (int j = 0; j < problem_instance.num_vertices; j++) {
				for (int t = 0; t <= s; t++) {
					for (int u = t + 1; u <= s; u++) {
						auto idx_ij = index_parser_m[i][j];
						auto idx_tu = index_parser_s2[t][u];
						auto idx_ju = index_parser_ns[j][u];
						auto idx_jt = index_parser_ns[j][t];
						auto idx_it = index_parser_ns[i][t];

						cplex_model.add(h[idx_ij][idx_tu] <= f[idx_ju]);
						cplex_model.add(h[idx_ij][idx_tu] <= x[idx_jt]);
						cplex_model.add(h[idx_ij][idx_tu] <= g[idx_it]);
						cplex_model.add(h[idx_ij][idx_tu] >= f[idx_ju] + x[idx_jt] + g[idx_it] - 2);
					}

				}
			}
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			//if (i != problem_instance.root) {
				for (int u = 1; u <= s; u++) {
					IloExpr sum_h(env);
					for (int j = 0; j < problem_instance.num_vertices; j++) {
						for (int t = 0; t < u; t++) {
							auto idx_ij = index_parser_m[i][j];
							auto idx_tu = index_parser_s2[t][u];
							sum_h += h[idx_ij][idx_tu];
						}
					}
					auto idx_iu = index_parser_ns[i][u];
					cplex_model.add(g[idx_iu] == f[idx_iu] + sum_h);
				}
			//}
		}


		for (int i = 0; i < problem_instance.num_vertices; i++) {
			for (int t = 0; t <= s; t++) {
				for (int u = 0; u <= s; u++) {
					if (t != u) {
						auto idx_it = index_parser_ns[i][t];
						auto idx_iu = index_parser_ns[i][u];
						cplex_model.add(w[idx_it][u] <= x[idx_it]);
						cplex_model.add(w[idx_it][u] <= g[idx_iu]);
						cplex_model.add(w[idx_it][u] >= x[idx_it] + g[idx_iu] - 1);
					}
				}
			}
		}

		for (auto const e : boost::make_iterator_range(boost::edges(problem_instance.covering_graph))) {
			auto i = problem_instance.covering_graph[e].source_id;
			auto j = problem_instance.covering_graph[e].target_id;
			for (int t = 0; t <= s; t++) {
				for (int u = 0; u <= s; u++) {
					if (t != u) {
						IloExpr sum_q(env);
						auto idx_kt = index_parser_ns[i][t];
						sum_q += w[idx_kt][u];
						for (int k : problem_instance.sucessors.at(i)) {
							idx_kt = index_parser_ns[k][t];
							sum_q += w[idx_kt][u];
						}
						auto idx_it = index_parser_ns[i][t];
						auto idx_ju = index_parser_ns[j][u];
						cplex_model.add(x[idx_it] + x[idx_ju] <= 1 + sum_q);
					}
				}
			}
		}

		for (int i = 0; i < problem_instance.num_vertices; i++) {
			auto idx_i0 = index_parser_ns[i][0];
			cplex_model.add(g[idx_i0] == 0);
		}
// as restrições abaixo não estão na implementação do ortools
		for (int i = 0; i < problem_instance.num_vertices; i++) {
			for (int t = 1; t <= s; t++) {
				auto idx_it = index_parser_ns[i][t];
				cplex_model.add(g[idx_it] <= 1);
			}
		}


		for (int t = 1; t <= s; t++) {
			IloExpr sum_x_l(env), sum_x_r(env);
			for (int i = 0; i < problem_instance.num_vertices; i++) {
				auto idx_it = index_parser_ns[i][t];
				sum_x_l += x[idx_it];
			}
			for (int i = 0; i < problem_instance.num_vertices; i++) {
				auto idx_it = index_parser_ns[i][t-1];
				sum_x_r += x[idx_it];
			}
			cplex_model.add(sum_x_l - sum_x_r <= 0);
		}
	}

	void CharacterizationBasedFormulation::add_objective_function() {
		//this->cplex_model.add(IloMinimize(env, 0));
	}

	int CharacterizationBasedFormulation::idx_ns(int i, int j) {
		return index_parser_ns[i][j];
	}

	CharacterizationBasedFormulation::~CharacterizationBasedFormulation() {}

	//void DDLModel::
} /* namespace solver */
