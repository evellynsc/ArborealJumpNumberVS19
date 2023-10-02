#include "Kernel.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include "../heuristic/minimal_extension.h"

Kernel::Kernel() {
	this->nbuck = 0;
	this->ajnp = NULL;
	this->cpx = NULL;
}

Kernel::Kernel(ajns::instance& ajnp, std::string solver_name, int nbuck) {
	this->ajnp = &ajnp;
	this->nbuck = nbuck;
	this->cpx = NULL;
	if (solver_name == "flow") {
		auto solver_config = solver::solver_params();
		auto exp_model = solver::MultiFlowModel(ajnp, true);
		cpx = new solver::MFSolver(solver_config, exp_model);
	}
}
Kernel::~Kernel() {
	delete ajnp;
	delete cpx;
}

void Kernel::run() {
	try {
		//Init kernel
		cpx->solve();
		auto var_values = cpx->get_values_main_variables();
		auto v_size = var_values.size();
		auto kernel = std::vector<bool>(v_size, false);
		auto kernel_out = std::vector<bool>(v_size, true);

		auto extension = ajns::minimal_extension(*this->ajnp);
		extension.run();
		auto h_solution = extension.get_solution();
		int n_var_kernel = 0;
		for (auto e : boost::make_iterator_range(boost::edges(h_solution))) {
			auto i = h_solution[e].source_id;
			auto j = h_solution[e].target_id;
			int idx = this->ajnp->num_vertices * i + j;
			kernel[idx] = true;
			n_var_kernel++;
		}


		for (int i = 0; i < v_size; i++) {
			if (var_values[i] == 1 and not kernel[i]) {
				kernel[i] = true;
				n_var_kernel++;
			}
			/*else {
				boost::random::mt19937 gen;
				boost::random::uniform_int_distribution<> dist(1, 100);
				int a = dist(gen);
				if (a > 50) {
					kernel_out[i] = false;
				}
			}*/
		}
		std::cout << "\t>>>>>>> " << v_size - n_var_kernel << " <<<<<<<<\n";
		auto solver_config = solver::solver_params();
		auto exp_model = solver::MultiFlowModel(*this->ajnp, false);
		cpx = new solver::MFSolver(solver_config, exp_model);
		cpx->reset_model_by_kernel(kernel);
		cpx->solve();
	}
	catch (std::exception const& e)
	{
		cout << "[solveByKernel] Error: " << e.what() << endl;
	}	
}