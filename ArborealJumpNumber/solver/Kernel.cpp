#include "Kernel.h"
Kernel::Kernel(ajns::instance& ajnp, std::string solver_name, int nbuck) {
	this->ajnp = &ajnp;
	this->cpx = NULL;
	this->nbuck = nbuck;
	if (solver_name == "flow") {
		auto solver_config = solver::solver_params();
		auto model = solver::MultiFlowModel(*this->ajnp, true);
		cpx = new solver::MFSolver(solver_config, model);
	}
}


Kernel::~Kernel() {
	delete cpx;
	delete ajnp;
}


void Kernel::run() {
	try {
		cpx->solve();
		/*auto var_values = cpx->get_main_variable_values();
		int size = var_values.size();
		auto kernel = std::vector<bool>(size, false);
		auto out_kernel = std::vector<bool>(size, true);
		auto kernel_idx = std::vector<int>();
		for (auto v : var_values) {
			if (v > 0.5) {
				kernel[v] = true;
				out_kernel[v] = false;
				kernel_idx.push_back(v);
			}
		}

		auto solver_config = solver::solver_params();
		auto model = solver::MultiFlowModel(*this->ajnp, false);
		cpx = new solver::MFSolver(solver_config, model);

		cpx->solve();*/
	}
	catch (std::exception const& e) {
		cout << "[solveByKernel] Error: " << e.what() << endl;
	}
}