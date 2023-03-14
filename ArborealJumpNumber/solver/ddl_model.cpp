#include "ddl_model.h"

namespace solver {

	DDLModel::DDLModel() {
		this->type = DDL;
	}


	DDLModel::DDLModel(ajns::instance& problem_instance_) {
		this->type = DDL;
		this->problem_instance = problem_instance_;
	}

	IloBoolVarArray DDLModel::get_y_variables() {
		return v;
	}

	IloBoolVarArray DDLModel::get_x_variables() {
		return x;
	}

	void DDLModel::add_variables() {
		auto r = problem_instance.input_graph[problem_instance.root].id;
		auto n = this->problem_instance.num_vertices;

		x = IloBoolVarArray(env, n * n);
		v = IloBoolVarArray(env, n * n);


		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < n; j++) {
				auto edge_in_input_graph = problem_instance.exist_edge_in_input_graph(i, j);
				auto edge_in_order_graph = problem_instance.exist_edge_in_order_graph(i, j);
				if (i == j or !edge_in_input_graph) {
					x[n * i + j] = IloNumVar(env, 0, 0, ILOBOOL);
				}
				else if (edge_in_order_graph) {
					v[n * i + j] = IloNumVar(env, 1, 1, ILOBOOL);
				}
				else {
					v[n * i + j] = IloNumVar(env, 0, 1, ILOBOOL);
					x[n * i + j] = IloNumVar(env, 0, 1, ILOBOOL);
				}
			}
		}
	}

	void DDLModel::add_constraints() {
		auto n = this->problem_instance.num_vertices;
		auto r = problem_instance.input_graph[problem_instance.root].id;

		// constraints 1
		for (const auto& v : boost::make_iterator_range(boost::vertices(problem_instance.input_graph))) {			
			if (not problem_instance.input_graph[v].is_root) {
				IloExpr in_edges(env);
				for (const auto& e : boost::make_iterator_range(boost::in_edges(v, problem_instance.input_graph))) {
					auto i = boost::source(e, problem_instance.input_graph);
					auto j = boost::target(e, problem_instance.input_graph);

					in_edges += x[n * i + j];
				}
				cplex_model.add(in_edges == 1);
			}
		}

		// constraints 2
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < n; j++) {
				if (i != j)
					cplex_model.add(x[n * i + j] <= v[n * i + j]);
			}
		}

		// constraints 3
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < n; j++) {
				for (size_t k = 0; k < n; k++) {
					if (i != j and j != k and k != i) {
						cplex_model.add(v[n * j + k] + v[n * i + j] + x[n * i + j] <= v[n * i + k] + 1);
					}
				}
			}
		}

		// constraints 4
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < n; j++) {
				if (i != j and i != r and j != r)
					cplex_model.add(v[n * j + i] + v[n * i + j] == 1);
			}
		}
	}

	void DDLModel::add_objective_function() {
		auto n = this->problem_instance.num_vertices;
		IloExpr minimize_jumps(env);
		for (const auto& e : boost::make_iterator_range(
			boost::edges(problem_instance.input_graph))) {
			if (problem_instance.input_graph[e].type == my_graph::ARTIFICIAL) {
				auto from = boost::source(e, problem_instance.input_graph);
				auto to = boost::target(e, problem_instance.input_graph);
				minimize_jumps += x[n * from + to];
			}
		}
		this->cplex_model.add(IloMinimize(env, minimize_jumps));
		minimize_jumps.end();
	}


	//void DDLModel::
};

