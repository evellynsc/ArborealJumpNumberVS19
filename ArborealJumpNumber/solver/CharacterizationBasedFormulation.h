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

        IloNumVarArray x;
        IloNumVarArray r;
        IloNumVarArray f;
        IloNumVarArray g;

        IloArray<IloNumVarArray> a;
        IloArray<IloNumVarArray> h;
        IloArray<IloNumVarArray> w;

        vector<vector<int>> index_parser_ns;
        vector<vector<int>> index_parser_m;
        vector<vector<int>> index_parser_s2;

        int s;


    public:
        CharacterizationBasedFormulation();
        CharacterizationBasedFormulation(ajns::instance&, int s);
        CharacterizationBasedFormulation(ajns::instance&, int s, bool _linear_relaxation);
   /*     IloNumVarArray get_v_variables();*/
        IloNumVarArray get_x_variables();
        IloNumVarArray get_r_variables();
        IloNumVarArray get_f_variables();
        IloNumVarArray get_g_variables();
        IloArray<IloNumVarArray> get_a_variables();
        int get_s();
        int idx_ns(int i, int j);

        virtual ~CharacterizationBasedFormulation() = default;
    };
} /* namespace solver */