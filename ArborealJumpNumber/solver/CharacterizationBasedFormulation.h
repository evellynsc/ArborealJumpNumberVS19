#pragma once

#include "model.h"
#include "../base/instance.h"
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN

namespace solver {

    class CharacterizationBasedFormulation : public Model
    {
        void add_variables();
        void add_objective_function();
        void add_constraints();

        IloBoolVarArray x;
        IloBoolVarArray r;
        IloBoolVarArray f;
        IloBoolVarArray g;

        IloArray<IloBoolVarArray> a;
        IloArray<IloBoolVarArray> h;
        IloArray<IloBoolVarArray> w;

        vector<vector<int>> index_parser_ns;
        vector<vector<int>> index_parser_m;
        vector<vector<int>> index_parser_s2;

        int s;


    public:
        CharacterizationBasedFormulation();
        CharacterizationBasedFormulation(ajns::instance&, int s);
        CharacterizationBasedFormulation(ajns::instance&, int s, bool _linear_relaxation);
   /*     IloNumVarArray get_v_variables();*/
        IloBoolVarArray get_x_variables();
        IloBoolVarArray get_r_variables();
        IloBoolVarArray get_f_variables();
        IloBoolVarArray get_g_variables();
        IloArray<IloBoolVarArray> get_a_variables();
        int get_s();
        int idx_ns(int i, int j);

        virtual ~CharacterizationBasedFormulation() = default;
    };
} /* namespace solver */