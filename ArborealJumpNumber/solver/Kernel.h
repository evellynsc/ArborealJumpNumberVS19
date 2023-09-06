#pragma once

#include "../base/instance.h"
#include "solver.h"
#include "MFSolver.h"

class Kernel
{
private:
	int nbuck;
	solver::solver* cpx;
	ajns::instance* ajnp;

public:
	Kernel();
	Kernel(ajns::instance& ajnp, std::string solver_name, int nbuck);
	~Kernel();
	void run();
};

