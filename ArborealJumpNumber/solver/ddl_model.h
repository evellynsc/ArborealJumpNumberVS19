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
        IloBoolVarArray get_y_variables();
        IloBoolVarArray get_x_variables();
        IloBoolVarArray v; //transitive closure (order graph)
        IloBoolVarArray x; //resulting graph

        virtual ~DDLModel() = default;
    };
} /* namespace solver */



#endif /* SOLVER_DDLMODEL_H_ */

