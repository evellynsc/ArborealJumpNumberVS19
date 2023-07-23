#pragma once

#include "../headers/formulations.h"
#include "../headers/exact_solver.h"

class CharacterizationBasedFormulation :
    public ExactSolver
{
public:
    CharacterizationBasedFormulation(Instance _instance) : ExactSolver(_instance) { 
        s = _instance.num_jumps;
        auto m = instance.num_vertices * instance.num_vertices;
        auto ns = instance.num_vertices * (s + 1u);
        auto s1 = s + 1u;
        auto s2 = s1 * s1;
        index_parser_ns = vector<vector<int>>(ns);
        index_parser_m = vector<vector<int>>(m);
        index_parser_s2 = vector<vector<int>>(s2);

        for (int i = 0; i < instance.num_vertices; i++) {
            index_parser_ns[i] = vector<int>(instance.num_vertices);
            for (int j = 0; j < instance.num_vertices; j++) {
                index_parser_m[i][j] = i * instance.num_vertices + j;
            }
        }

        for (int i = 0; i < instance.num_vertices; i++) {
            index_parser_ns[i] = vector<int>(s1);
            for (int t = 0; t <= s; t++) {
                index_parser_ns[i][t] = i * instance.num_vertices + t;
            }
        }

        for (int u = 0; u <= s; u++) {
            index_parser_s2[u] = vector<int>(s1);
            for (int t = 0; t <= s; t++) {
                index_parser_s2[u][t] = u * instance.num_vertices + t;
            }
        }

        x = IloBoolVarArray(env, ns);
        r = IloBoolVarArray(env, ns);
        f = IloBoolVarArray(env, ns);
        g = IloBoolVarArray(env, ns);
        y = IloBoolVarArray(env, s2);

        a = IloArray<IloBoolVarArray>(env, s1);
        for (int t = 0; t <= s; t++) {
            a[t] = IloBoolVarArray(env, m);
        }

        w = IloArray<IloBoolVarArray>(env, ns);
        for (int t = 0; t < ns; t++) {
            w[t] = IloBoolVarArray(env, s1);
        }

        z = IloArray<IloBoolVarArray>(env, ns);
        for (int t = 0; t < ns; t++) {
            z[t] = IloBoolVarArray(env, s1);
        }

        q = IloArray<IloBoolVarArray>(env, ns);
        for (int t = 0; t < ns; t++) {
            q[t] = IloBoolVarArray(env, s1);
        }
    }

    Answer Solve(int timeLimit, double heuristics);
    ~CharacterizationBasedFormulation() {}

    void SetInitialSolution();
    void AddConstraintsAndObjective();

private:
    IloBoolVarArray x;
    IloBoolVarArray r;
    IloBoolVarArray f;
    IloBoolVarArray y;
    IloBoolVarArray g;

    IloArray<IloBoolVarArray> a;
    IloArray<IloBoolVarArray> w;
    IloArray<IloBoolVarArray> z;
    IloArray<IloBoolVarArray> q;

    vector<vector<int>> index_parser_ns;
    vector<vector<int>> index_parser_m;
    vector<vector<int>> index_parser_s2;

    int s;
 };



