/*
 * multi_flow_model.h
 *
 *  Created on: Feb 23, 2022
 *      Author: evellyn
 */

#ifndef SOLVER_MULTI_FLOW_MODEL_H_
#define SOLVER_MULTI_FLOW_MODEL_H_

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

#include "model.h"

namespace solver {

typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<NumVarMatrix>   NumVar3dMatrix;

class MultiFlowModel : public Model {
	void add_variables();
	void add_objective_function();
	void add_constraints();
	std::vector<bool> zero_variables;
//	flow goes through arc (i,j) transporting token k
//	token k has to be delivery to vertex k

//	y(i,j) = 1 if arc (i,j) is selected to be in the solution, otherwise, y(i,j) = 0
	

public:
	MultiFlowModel();
	MultiFlowModel(ajns::instance&);
	MultiFlowModel(ajns::instance&, bool);
	//MultiFlowModel(ajns::instance&, bool, std::vector<bool>);
	void reset_upper_bounds(std::vector<bool>);
	IloNumVarArray get_y_variables();
	IloNumVarArray y;
	IloNumVarArray x;
	
	virtual ~MultiFlowModel() = default;
};

} /* namespace solver */

#endif /* SOLVER_MULTIFLOWMODEL_H_ */
