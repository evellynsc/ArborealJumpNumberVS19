#include "Kernel.h"

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

		for (int i = 0; i < v_size; i++) {
			if (var_values[i] > 0) {
				kernel[i] = true;
				kernel_out[i] = false;
			}
		}
	}
	catch (std::exception const& e)
	{
		cout << "[solveByKernel] Error: " << e.what() << endl;
	}	
}