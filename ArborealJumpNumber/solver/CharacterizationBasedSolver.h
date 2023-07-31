#pragma once

#include "solver.h"
#include "CharacterizationBasedFormulation.h"
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

namespace solver {
	class CharacterizationBasedSolver :
		public solver
	{
		CharacterizationBasedFormulation model;

	public:

		CharacterizationBasedSolver(solver_params& solver_config, CharacterizationBasedFormulation& ajnp_model) :
			solver(solver_config, &ajnp_model) {
			model = ajnp_model;
		}
		void solve();
		virtual ~CharacterizationBasedSolver() = default;
	};
}

