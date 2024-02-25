#ifndef SOLVER_DDLMODEL_H_
#define SOLVER_DDLMODEL_H_

#include "model.h"
#include "../base/instance.h"
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN


namespace solver {

    class DDLModel : public Model
    {
        void add_variables();
        void add_objective_function();
        void add_constraints();



    public:
        DDLModel();
        DDLModel(ajns::instance&);
        DDLModel(ajns::instance&, bool _linear_relaxation);
        IloNumVarArray get_v_variables();
        IloNumVarArray get_x_variables();
        IloNumVarArray y; //transitive closure (order graph)
        IloNumVarArray x; //resulting graph
        IloNumVarArray u; //auxiliar variable
        size_t t_max;

        ~DDLModel();
    };
} /* namespace solver */



#endif /* SOLVER_DDLMODEL_H_ */

