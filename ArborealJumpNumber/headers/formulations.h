#pragma once

#include <iostream>
#include <string>
#include "exact_solver.h"
#include "instance.h"

class Formulations
{
public:
	static ExactSolver* GetFormulation(Instance internalS, std::string formulation);
	static const string characterizationBasedFormulation;

private:
	Formulations() {}
};


struct formulationNotFound : std::exception {
	const char* what() const noexcept { return "Formulation not found"; }
};
