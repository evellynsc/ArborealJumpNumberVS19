#pragma once

#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

//#include "model.h"
//#include "solver_params.h"
//#include "../base/properties.h"

#include <string>

#include "instance.h"
#include "answer.h"
//#include "optimal_struct.h"

class ExactSolver {
public:
    //static const int kInf = (1 << 29);
    //static const double kHeuristicOp;
    //static const int kTimeLimitOp;
    const std::string formulationName;

    ExactSolver(Instance _instance)
        : instance(_instance), env(IloEnv()), model(IloModel(env)), cplex(IloCplex(model)) {
        InitModel();
    }
    virtual Answer Solve(int timeLimit, double heuristics) = 0;
    virtual ~ExactSolver() {}
    virtual void SetInitialSolution() = 0;
    virtual void AddConstraintsAndObjective() = 0;

    static double GetUpper(IloModel& model_);
    static double GetLower(IloModel& model_);
    static double GetOptimalityGap(IloModel& model_);


    Instance instance;

protected:
    IloEnv env;
    IloModel model;
    IloCplex cplex;

private:
    void InitModel() {
    }
};

// best known answer
inline double ExactSolver::GetUpper(IloModel& model_) {
   /* if (model_.get(GRB_IntAttr_SolCount) == 0) {
        return GRB_INFINITY;
    }
    return model_.get(GRB_DoubleAttr_ObjVal);*/
    return 0;
}
inline double ExactSolver::GetLower(IloModel& model_) {
    return 0;
    //return max(model_.get(GRB_DoubleAttr_ObjBound), 0.0);
}

inline double ExactSolver::GetOptimalityGap(IloModel& model_) {
    return 0;
    //double upper = Solver::GetUpper(model_);
    //double lower = Solver::GetLower(model_);
    //return fabs(lower - upper) / upper;
    //// return model_.get(GRB_DoubleAttr_MIPGap);
}



