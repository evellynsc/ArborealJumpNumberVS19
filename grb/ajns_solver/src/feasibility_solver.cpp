#include "headers/feasibility_solver.h"

std::unordered_map<SolverEnum, const std::string> operations_research::FeasibilitySolver::solver_map = {{GUROBI, "GUROBI"}, {CPLEX, "CPLEX"}, {HIGHS, "HIGHS"}}; 


void operations_research::FeasibilitySolver::create_variables()
{
    // const double infinity = solver->infinity();
    
    for (unsigned long i = 0; i < data->n; i++) 
    {
        for (unsigned long t = 0; t < nparts; t++)
        {
            x.push_back(solver->MakeIntVar(0.0, 1, "x_" + std::to_string(i) + "_" + std::to_string(t)));
            r.push_back(solver->MakeIntVar(0.0, 1, "r_" + std::to_string(i) + "_" + std::to_string(t)));
            f.push_back(solver->MakeIntVar(0.0, 1, "f_" + std::to_string(i) + "_" + std::to_string(t)));
            g.push_back(solver->MakeIntVar(0.0, 1, "g_" + std::to_string(i) + "_" + std::to_string(t)));
        }
    }

    for (unsigned long i = 0; i < data->n; i++) 
    {
        for (unsigned long j = 0; j < data->n; j++)
            for (unsigned long t = 0; t < nparts; t++)
            {
                a.push_back(solver->MakeIntVar(0.0, 1, "a_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(t)));
                for (unsigned long u = 0; u < nparts; u++)
                    h.push_back(solver->MakeIntVar(0.0, 1, "h_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(t) + "_" + std::to_string(u)));
            }
    }

    for (unsigned long i = 0; i < data->n; i++) 
    {
        for (unsigned long t = 0; t < nparts; t++)
            for (unsigned long u = 0; u < nparts; u++)
                w.push_back(solver->MakeIntVar(0.0, 1, "w_" + std::to_string(i) + "_" + std::to_string(t) + "_" + std::to_string(u)));
    }

    LOG(INFO) << data->n;
    LOG(INFO) << "Number of variables = " << solver->NumVariables();
}


void operations_research::FeasibilitySolver::create_model()
{
    create_variables();
    // create_objective();
    create_constraints();
}

void operations_research::FeasibilitySolver::create_constraints()
{
    // ============== INDUCED ARBOREAL SUBPOSET ==============

    // \sum_{t \in nparts} x_{it} <= 1 \forall i \in data->n
    // each vertex can belong to one part, at maximum.
    for (long unsigned i = 0; i < data->n; ++i)
    {
        LinearExpr sum_x;
        for (long unsigned t = 0; t < nparts; ++t)
            sum_x += x[get_index_d2({i, data->n},{t, nparts})];

        solver->MakeRowConstraint(sum_x == 1.0); //reunião dia 28/02/2024
    }

    // x_{it} - a_{ijt} \geq 0 & \forall (i,j) \in data->adj_mtx_reduction[i][j], \forall t \in nparts
    // x_{jt} - a_{ijt} \geq 0 & \forall (i,j) \in data->adj_mtx_reduction[i][j], \forall t \in nparts
    // a_{ijt} - x_{it} -  x_{jt} \geq  - 1 & \forall (i,j) \in data->adj_mtx_reduction[i][j], \forall t \in nparts
    for (long unsigned t = 0; t < nparts; ++t)
        for (long unsigned i = 0; i < data->n; ++i)
            for (long unsigned j = 0; j < data->n; ++j) 
            {
                if (data->adj_mtx_reduction[i][j]) 
                {
                    LinearExpr lhs;
                    auto it = get_index_d2({i, data->n}, {t, nparts});
                    auto jt = get_index_d2({j, data->n}, {t, nparts});
                    auto ijt = get_index_d3({i, data->n}, {j, data->n}, {t, nparts});
                    
                    lhs = LinearExpr(x[it]) - LinearExpr(a[ijt]);
                    solver->MakeRowConstraint(lhs >= 0.0); 
                    
                    lhs = LinearExpr(x[jt]) - LinearExpr(a[ijt]);
                    solver->MakeRowConstraint(lhs >= 0.0);
                    
                    lhs = LinearExpr(a[ijt]) - LinearExpr(x[it]) - LinearExpr(x[jt]);
                    solver->MakeRowConstraint(lhs >= -1.0);
                }
                
            }  

    // \sum_{t>u} x_{it} + x_{ju} \leq 1 \forall (i,j) \in data->adj_mtx_reduction[i][j], \forall u,t \in nparts
    // if t > u and j \in u, then i cannot belong to part t.
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned j = 0; j < data->n; ++j)
            if (data->adj_mtx_reduction[i][j])
                for (long unsigned u = 0; u < nparts; ++u)//poderia fazer u < nparts-1
                {
                    LinearExpr sum_x;
                    bool add = false;
                    for (long unsigned t = u+1; t < nparts; ++t) 
                    {
                        sum_x += x[get_index_d2({i, data->n},{t, nparts})];
                        add = true; 
                    }
                    if (add)
                    {
                        sum_x += x[get_index_d2({j, data->n},{u, nparts})];
                        solver->MakeRowConstraint(sum_x <= 1.0);
                    }                        
                }
    
    // roots, vertices and arcs

    // the root is the vertex data->n-1
    // r_{00} = 1
    LinearExpr lhs = LinearExpr(r[get_index_d2({data->n-1, data->n}, {0, nparts})]);
    solver->MakeRowConstraint(lhs == 1.0);
    
    // \sum_{i \in V} r_{it} = 1 & \forall t \in nparts
    for (long unsigned t = 0; t < nparts; ++t)
    {
        LinearExpr sum_r;
        for (long unsigned i = 0; i < data->n; ++i)
            sum_r += r[get_index_d2({i, data->n},{t, nparts})];

        solver->MakeRowConstraint(sum_r == 1.0);
    }
    
    // x_{jt} \geq r_{jt} & \forall j \in data->n-1, \forall t \in nparts
    for (long unsigned j = 0; j < data->n-1; ++j) 
        for (long unsigned t = 0; t < nparts; ++t)
            solver->MakeRowConstraint(LinearExpr(x[get_index_d2({j,data->n},{t, nparts})]) >= 
                LinearExpr(r[get_index_d2({j,data->n},{t, nparts})]));

    // \sum_{t \in \pi} r_{jt} + \sum_{i \in \phi^-(j)}\sum_{t \in \pi}a_{ijt} = 1, \forall j \in data->n-1
    for (long unsigned j = 0; j < data->n-1; ++j) 
    {
        LinearExpr sum_r;
        LinearExpr sum_a;
        for (long unsigned t = 0; t < nparts; ++t)
        {
            for (long unsigned i = 0; i < data->n; ++i) // estava data->n-1
            {
                if (data->adj_mtx_reduction[i][j])
                    sum_a += a[get_index_d3({i, data->n},{j,data->n},{t, nparts})];
            }
            sum_r += r[get_index_d2({j,data->n},{t, nparts})];
        }
        solver->MakeRowConstraint(sum_r + sum_a == 1.0);
    }

    // \sum_{t\in \pi} a_{ijt} \leq 1& \forall (i,j) \in data->adj_mtx_reduction
    // \sum_{t\in \pi} a_{ijt} = 0& \forall (i,j) \notin data->adj_mtx_reduction
    for (long unsigned j = 0; j < data->n; j++) 
        for (long unsigned i = 0; i < data->n; i++) 
        {
            LinearExpr sum_a;            
            for (long unsigned t = 0; t < nparts; t++)
                sum_a += a[get_index_d3({i, data->n},{j, data->n},{t, nparts})];
            
            if (data->adj_mtx_reduction[i][j])
                solver->MakeRowConstraint(sum_a <= 1.0);
            else 
                solver->MakeRowConstraint(sum_a == 0.0);
        }

    // ============== f FUNCTION DEFINITION ==============

    // \sum_{i \in data->n} f_{it} = 1 & \forall t > 0, t \in nparts
    for (long unsigned t = 0; t < nparts; ++t)
    {
        LinearExpr sum_f;
        for (long unsigned i = 0; i < data->n; ++i)
            sum_f += f[get_index_d2({i,data->n},{t, nparts})];
        if (t > 0)
            solver->MakeRowConstraint(sum_f == 1.0);
        else 
            solver->MakeRowConstraint(sum_f == 0.0); //ALERT: is it mandatory?
    }

    // f_{iu} \leq \sum_{t \in nparts, t < u} x_{it} & \forall i \in data->n, \forall u > 0, u \in nparts
    for (long unsigned u = 1; u < nparts; ++u)
    {
        for (long unsigned i = 0; i < data->n; ++i)
        {
            LinearExpr sum_x;
            for (long unsigned t = 0; t < u; ++t)
                sum_x += x[get_index_d2({i,data->n},{t, nparts})];
            LinearExpr lhs = f[get_index_d2({i,data->n},{u, nparts})];
            solver->MakeRowConstraint(lhs <= sum_x);
        }
    }

    // f_{it} + r_{jt} + d_{ij} \leq 2 & \forall i,j \in data->n, \forall t \in nparts
    for (long unsigned t = 0; t < nparts; ++t)
        for (long unsigned i = 0; i < data->n; ++i)
            for (long unsigned j = 0; j < data->n; ++j)
            {
                LinearExpr lhs;
                lhs = f[get_index_d2({i, data->n}, {t, nparts})];
                lhs += r[get_index_d2({j, data->n}, {t, nparts})];
                lhs += (double) data->adj_mtx_closure[i][j];
                solver->MakeRowConstraint(lhs <= 2.0);
            }               

    // ============== g FUNCTION DEFINITION ==============
    // h_{ijtu} \leq f_{ju} & \forall i,j \in data->n, \forall u,t \in nparts, t < u
    // h_{ijtu} \leq x_{jt} & \forall i,j \in data->n, \forall u,t \in nparts, t < u
    // h_{ijtu} \leq g_{it} & \forall i,j \in data->n, \forall u,t \in nparts, t < u
    // h_{ijtu} \geq f_{ju} + x_{jt} + g_{it} - 2 & \forall i,j \in data->n, \forall u,t \in nparts, t < u
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned j = 0; j < data->n; ++j)            
            for (long unsigned u = 1; u < nparts; ++u)
                for (long unsigned t = 0; t < u; ++t)
                {
                    LinearExpr lhs = h[get_index_d4({i, data->n},{j, data->n},{t, nparts},{u, nparts})];
                    LinearExpr rhs = f[get_index_d2({j, data->n},{u, nparts})];
                    solver->MakeRowConstraint(lhs <= rhs);

                    rhs = x[get_index_d2({j, data->n},{t, nparts})];
                    solver->MakeRowConstraint(lhs <= rhs);

                    rhs = g[get_index_d2({i, data->n},{t, nparts})];
                    solver->MakeRowConstraint(lhs <= rhs);

                    rhs  = f[get_index_d2({j, data->n},{u, nparts})];
                    rhs += x[get_index_d2({j, data->n},{t, nparts})];
                    rhs += g[get_index_d2({i, data->n},{t, nparts})];
                    rhs -= 2.0;
                    solver->MakeRowConstraint(lhs >= rhs);                    
                }  

    // g_{iu} = f_{iu} + \sum_{j \in data->n} \sum_{t \in nparts, t < u} h_{ijtu} & \forall i \in data->n, \forall u \in nparts\{0} 
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned u = 1; u < nparts; ++u)
        {
            LinearExpr sum_h;
            for (long unsigned j = 0; j < data->n; ++j)
                for (long unsigned t = 0; t < u; ++t)
                    sum_h += h[get_index_d4({i, data->n},{j, data->n},{t, nparts},{u, nparts})];
            LinearExpr rhs = f[get_index_d2({i, data->n},{u, nparts})] + sum_h;
            LinearExpr lhs = g[get_index_d2({i, data->n},{u, nparts})];
            solver->MakeRowConstraint(lhs == rhs);  
        }

    // w_{itu} \leq x_{it} & \forall i \in data->n, \forall u,t \in nparts, t < u
    // w_{itu} \leq g_{iu} \forall i \in data->n, \forall u,t \in nparts, t < u 
    // w_{itu} \geq x_{it} + g_{iu}  - 1 & \forall i \in data->n, \forall u,t \in nparts, t < u
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned u = 1; u < nparts; ++u)
            for (long unsigned t = 0; t < u; ++t)
            {
                LinearExpr lhs = w[get_index_d3({i, data->n},{t, nparts},{u, nparts})];
                LinearExpr rhs = x[get_index_d2({i, data->n},{t, nparts})];
                solver->MakeRowConstraint(lhs <= rhs);  

                rhs = g[get_index_d2({i, data->n},{u, nparts})];
                solver->MakeRowConstraint(lhs <= rhs);

                rhs  = x[get_index_d2({i, data->n},{t, nparts})];
                rhs += g[get_index_d2({i, data->n},{u, nparts})];
                rhs -= 1.0;
                solver->MakeRowConstraint(lhs >= rhs);
            }

    // x_{it} + x_{ju} \leq 1 + \sum_{k \in data->adj_mtx_closure[i]} w_{ktu} & \forall (i,j) \in data->adj_mtx_reduction, \forall t, u \in nparts, t < u
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned j = 0; j < data->n; ++j)
            if (data->adj_mtx_reduction[i][j])
            {
                for (long unsigned u = 1; u < nparts; ++u)
                    for (long unsigned t = 0; t < u; ++t)
                    {
                        LinearExpr sum_w = w[get_index_d3({i, data->n},{t, nparts},{u, nparts})];
                        for (long unsigned k = 0; k < data->n; ++k)
                            if (data->adj_mtx_closure[i][k])
                                sum_w += w[get_index_d3({k, data->n},{t, nparts},{u, nparts})];
                        LinearExpr lhs = x[get_index_d2({i, data->n},{t, nparts})];
                        lhs += x[get_index_d2({j, data->n},{u, nparts})];
                        LinearExpr rhs = 1.0;
                        rhs += sum_w;

                        solver->MakeRowConstraint(lhs <= rhs);
                    }
            }

    LinearExpr sum_g;
    for (long unsigned i = 0; i < data->n; ++i)
    {
        sum_g += g[get_index_d2({i, data->n},{0, nparts})];
    }
    solver->MakeRowConstraint(sum_g == 0.0);
    
    std::string text;
    solver->ExportModelAsLpFormat(false, &text);
    std::ofstream myfile;
    myfile.open ("lp_model.txt");
    myfile << text;
    myfile.close();
    LOG(INFO) << "Number of constraints = " << solver->NumConstraints();
}

void operations_research::FeasibilitySolver::solve_model()
{
    const MPSolver::ResultStatus result_status = solver->Solve();

    // Check that the problem has an optimal solution.
    if (result_status == MPSolver::OPTIMAL) {
        LOG(INFO) << "The problem has an optimal solution!";
    } else if (result_status != MPSolver::OPTIMAL) {
        LOG(INFO) << "The problem does not have an optimal solution!";
        if (result_status == MPSolver::FEASIBLE) {
            LOG(INFO) << "A potentially suboptimal solution was found";
        } else {
            LOG(INFO) << "The solver could not solve the problem.";
            return;
        }
    }

    LOG(INFO) << "Problem solved in " << solver->wall_time() << " milliseconds";
    LOG(INFO) << "Problem solved in " << solver->iterations() << " iterations";

    std::cout << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned t = 0; t < nparts; ++t)
            if (x[get_index_d2({i, data->n},{t, nparts})]->solution_value() == 1)
                std::cout << "x(" << i << "," << t << ")" << std::endl;

    std::cout << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned j = 0; j < data->n; ++j)
            for (long unsigned t = 0; t < nparts; ++t)
                if (a[get_index_d3({i, data->n},{j, data->n},{t, nparts})]->solution_value() == 1)
                    std::cout << "a(" << i << "," << j << "," << t << ")" << std::endl;

    std::cout << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned t = 0; t < nparts; ++t)
            if (f[get_index_d2({i, data->n},{t, nparts})]->solution_value() == 1)
                std::cout << "f(" << i << "," << t << ")" << std::endl;

    std::cout << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned t = 0; t < nparts; ++t)
            if (r[get_index_d2({i, data->n},{t, nparts})]->solution_value() == 1)
                std::cout << "r(" << i << "," << t << ")" << std::endl;

    std::cout << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned t = 0; t < nparts; ++t)
            if (g[get_index_d2({i, data->n},{t, nparts})]->solution_value() == 1)
                std::cout << "g(" << i << "," << t << ")" << std::endl;
}
