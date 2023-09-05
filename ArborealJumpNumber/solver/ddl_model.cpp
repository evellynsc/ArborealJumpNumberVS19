#include "ddl_model.h"
#include <cmath>

namespace solver {

	DDLModel::DDLModel() {
		this->type = DDL;
		this->t_max = 0;
	}

	DDLModel::DDLModel(ajns::instance& problem_instance) {
		this->type = DDL;
		this->problem_instance = problem_instance;
		this->t_max = std::floor(std::log2(problem_instance.num_vertices));
	}

	DDLModel::DDLModel(ajns::instance& problem_instance, bool _linear_relaxation) {
		this->type = DDL;
		this->linear_relaxation = _linear_relaxation;
		this->problem_instance = problem_instance;
		this->t_max = std::floor(std::log2(problem_instance.num_vertices));
	}

	IloNumVarArray DDLModel::get_v_variables() {
		return y;
	}

	IloNumVarArray DDLModel::get_x_variables() {
		return x;
	}

	void DDLModel::add_variables() {
		auto r = problem_instance.input_graph[problem_instance.root].id;
		auto n = this->problem_instance.num_vertices;
		std::cout << n << " " << t_max << std::endl;

		if (this->linear_relaxation) {
			x = IloNumVarArray(env, n * n, 0.0, 1.0, ILOFLOAT);
			y = IloNumVarArray(env, n * n * (t_max + 1u), 0.0, 1.0, ILOFLOAT);
			u = IloNumVarArray(env, n * n * n * (t_max + 1u), 0.0, 1.0, ILOFLOAT);
		}
		else {
			x = IloNumVarArray(env, n * n, 0.0, 1.0, ILOBOOL);
			y = IloNumVarArray(env, n * n * (t_max + 1u), 0.0, 1.0, ILOBOOL);
			u = IloNumVarArray(env, n * n * n * (t_max + 1u), 0.0, 1.0, ILOBOOL);
		}



		for (auto i = 0; i < n; i++) {
			for (auto j = 0; j < n; j++) {
				std::string name = "x_" + std::to_string(i) + "_" + std::to_string(j);
				x[j + i * n].setName(name.c_str());
			}
		}

		for (auto i = 0; i < n; i++) {
			for (auto j = 0; j < n; j++) {
				for (auto t = 0; t < (t_max + 1); t++) {
					std::string name = "y_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(t);
					y[j + i * n + t * n * n].setName(name.c_str());
				}				
			}
		}

		for (auto i = 0; i < n; i++) {
			for (auto j = 0; j < n; j++) {
				for (auto k = 0; k < n; k++) {
					for (auto t = 0; t < (t_max + 1); t++) {
						std::string name = "u_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(k) + "_" + std::to_string(t);
						u[j + i * n + k * n * n + t * n * n * n].setName(name.c_str());
					}
				}
			}
		}

		for (auto i = 0; i < n; i++) {
			for (auto j = 0; j < n; j++) {
				auto edge_in_input_graph = problem_instance.exist_edge_in_input_graph(i, j);
				if (!edge_in_input_graph or j == r) {
					cplex_model.add(x[n * i + j] == 0);
					std::cout << x[n * i + j].getName() << " == " << 0 << std::endl;
				}
			}
		}
		std::cout << "terminou x" << std::endl;

	}

	void DDLModel::add_constraints() {
		auto n = this->problem_instance.num_vertices;
		auto r = problem_instance.input_graph[problem_instance.root].id;

		for (auto j = 0u; j < n; j++) {
			if (j != r) {
				IloExpr sum_i(env);
				for (auto i = 0u; i < n; i++) {
					sum_i += x[n * i + j];
					std::cout << " + " << x[n * i + j].getName();
				}
				std::cout << " == 1 " << std::endl;
				cplex_model.add(sum_i == 1);
			}
		}

		std::cout << "terminou c1" << std::endl;

		for (auto i = 0u; i < n; i++) {
			for (auto j = 0u; j < n; j++) {
				std::cout << x[j + n * i].getName() << " == " << y[j + n * i].getName() << std::endl;
				cplex_model.add(x[j + n * i] == y[j + n * i]);
			}
		}
		std::cout << "terminou c3" << std::endl;


		for (auto i = 0u; i < n; i++) {
			for (auto j = 0u; j < n; j++) {
				//if (i != j) {
					for (auto t = 0u; t < t_max+1; t++) {					
						std::cout << y[j + n * i + t * n * n].getName() << " + " << y[i + n * j + t * n * n].getName() << " <= 1" << std::endl;
						cplex_model.add(y[j + n * i + t * n * n] + y[i + n * j + t * n * n] <= 1);
					}
				//}
			}
		}
		std::cout << "terminou c4" << std::endl;


		for (auto i = 0u; i < n; i++) {
			for (auto j = 0u; j < n; j++) {
				for (auto t = 1u; t < t_max+1; t++) {
					IloExpr sum_k(env);
					std::string sk = "";
					for (auto k = 0u; k < n; k++) {
						//if (i != k and j != k and i != j) {
							sk += " + ";
							sk += std::string(u[k + n * i + n * n * j + (t - 1) * n * n * n].getName());
							sum_k += u[k + n * i + n * n * j + (t - 1) * n * n * n];
						//}
					}
					if (i != j) {
						std::cout << y[j + n * i + t * n * n].getName() << " <= " << y[j + n * i + (t - 1) * n * n].getName() << sk << std::endl;
						cplex_model.add(y[j + n * i + t * n * n] <= y[j + n * i + (t - 1) * n * n] + sum_k);
					}
				}
			}
		}
		std::cout << "terminou c5" << std::endl;

		for (auto i = 0u; i < n; i++) {
			for (auto j = 0u; j < n; j++) {
				if (i != j) {
					for (auto t = 1u; t < t_max + 1; t++) {
						//std::cout << y[j + n * i + t * n * n].getName() << " >= " << y[j + n * i + (t - 1) * n * n].getName() << std::endl;
						cplex_model.add(y[j + n * i + t * n * n] >= y[j + n * i + (t - 1) * n * n]);
					}
				}
			}
		}
		std::cout << "terminou c5.1" << std::endl;


		for (auto i = 0u; i < n; i++) {
			for (auto k = 0u; k < n; k++) {
				for (auto j = 0u; j < n; j++) {	
					//if (i != k and k != j and i != j) {
						for (auto t = 1u; t < t_max + 1; t++) {
						/*	cout << i << " " << k << " " << j << " " << t << " " << std::endl;
							std::cout << y[j + n * i + t * n * n].getName() << " >= " << u[k + n * i + n * n * j + (t - 1) * n * n * n].getName() << std::endl;*/
							cplex_model.add(y[j + n * i + t * n * n] >= u[k + n * i + n * n * j + (t - 1) * n * n * n]);
						}
					//}
				}
			}
		}
		std::cout << "terminou c5.2" << std::endl;


		for (auto i = 0u; i < n; i++) {
			for (auto k = 0u; k < n; k++) {
				for (auto j = 0u; j < n; j++) {
					//if (i != k and k != j and i != j) {
						for (auto t = 0u; t < t_max + 1; t++) {
							//cout << i << " " << k << " " << j << " " << t << " " << std::endl;
							//std::cout << u[i + n * k + n * n * j + t * n * n * n].getName() << " <= " << y[i + n * k + t * n * n].getName() << std::endl;
							//std::cout << u[i + n * k + n * n * j + t * n * n * n].getName() << " <= " << y[j + n * i + t * n * n].getName() << std::endl;
							//std::cout << u[i + n * k + n * n * j + t * n * n * n].getName() << " >= " <<
							//	y[i + n * k + t * n * n].getName() << " + " << y[j + n * i + t * n * n].getName() << " - 1" << std::endl;

							cplex_model.add(u[i + n * k + n * n * j + t * n * n * n] <= y[i + n * k + t * n * n]);
							cplex_model.add(u[i + n * k + n * n * j + t * n * n * n] <= y[j + n * i + t * n * n]);
							cplex_model.add(u[i + n * k + n * n * j + t * n * n * n] >= y[i + n * k + t * n * n] + y[j + n * i + t * n * n] - 1);
						}
					//}
				}
			}
		}
		std::cout << "terminou c6, c7, c8" << std::endl;

		/*for (auto i = 0u; i < n; i++) {
			for (auto k = 0u; k < n; k++) {
				for (auto j = 0u; j < n; j++) {
					for (auto t = 0u; t < t_max + 1; t++) {
						cplex_model.add(u[i + n * k + n * n * j + t * n * n * n] <= y[k + n * j + t * n * n]);
					}
				}
			}
		}
		std::cout << "terminou c7" << std::endl;*/


		/*for (auto i = 0u; i < n; i++) {
			for (auto k = 0u; k < n; k++) {
				for (auto j = 0u; j < n; j++) {
					for (auto t = 1u; t < t_max + 1; t++) {
						cplex_model.add(u[i + n * k + n * n * j + t * n * n * n] >= y[i + n * k + t * n * n] + y[k + n * j + t * n * n] - 1);
					}
				}
			}
		}
		std::cout << "terminou c8" << std::endl;*/


		for (auto i = 0u; i < n; i++) {
			for (auto j = 0u; j < n; j++) {
				auto edge_in_order = problem_instance.exist_edge_in_order_graph(i, j);
				if (edge_in_order and i != j) {
					std::cout << y[j + n * i + t_max * n * n].getName() << " == 1" << std::endl;
					cplex_model.add(y[j + n * i + t_max * n * n] == 1);	
				}
			}
		}
		std::cout << "terminou c9" << std::endl;
	}

	void DDLModel::add_objective_function() {
		auto n = this->problem_instance.num_vertices;
		IloExpr minimize_jumps(env);
		for (const auto& e : boost::make_iterator_range(
			boost::edges(problem_instance.input_graph))) {
			if (problem_instance.input_graph[e].type == my_graph::ARTIFICIAL) {
				auto from = boost::source(e, problem_instance.input_graph);
				auto to = boost::target(e, problem_instance.input_graph);
				minimize_jumps += x[n * to + from];
			}
		}
		/*for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < n; j++) {
				minimize_jumps += (int)i * x[n * i + j];
			}
		}*/
		this->cplex_model.add(IloMinimize(env, minimize_jumps));
		//minimize_jumps.end();
	}


	DDLModel::~DDLModel() {}
};

