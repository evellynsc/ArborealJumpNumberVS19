/*
 * solver.h
 *
 *  Created on: 22 de fev de 2021
 *      Author: evellyn
 */

#ifndef SOLVER_MFSOLVER_H_
#define SOLVER_MFSOLVER_H_


#include <ilcplex/ilocplex.h>
ILOSTLBEGIN


#include "solver_params.h"
#include "../base/properties.h"
#include "solver.h"
#include "multi_flow_model.h"

namespace solver {
class MFSolver : public solver {
	MultiFlowModel model;


public:
//	MFSolver();
	MFSolver(solver_params& solver_config, MultiFlowModel& ajnp_model) :
		solver(solver_config, &ajnp_model) {model = ajnp_model;}
	void solve();
	virtual ~MFSolver() = default;
};

} /* namespace solver */

#endif /* SOLVER_MFSOLVER_H_ */
