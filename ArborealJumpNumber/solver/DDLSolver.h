#ifndef SOLVER_DDLSOLVER_H_
#define SOLVER_DDLSOLVER_H_

#pragma once

#include "solver.h"
#include "ddl_model.h"
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

namespace solver {
	class DDLSolver : public solver
	{
		DDLModel model;


	public:
		//	MFSolver();
		DDLSolver(solver_params& solver_config, DDLModel& ajnp_model) :
			solver(solver_config, &ajnp_model) {
			model = ajnp_model;
		}
		void solve();
		virtual ~DDLSolver() = default;
	};
} /* namespace solver */

#endif /* SOLVER_DDLSOLVER_H_ */