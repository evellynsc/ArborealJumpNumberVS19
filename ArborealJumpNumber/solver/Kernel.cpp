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
		cpx->solve();
	}
	catch (std::exception const& e)
	{
		cout << "[solveByKernel] Error: " << e.what() << endl;
	}

	
}