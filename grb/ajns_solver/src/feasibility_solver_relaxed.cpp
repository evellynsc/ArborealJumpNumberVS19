#include "headers/feasibility_solver_relaxed.h"

std::unordered_map<SolverEnum, const std::string> operations_research::FeasibilitySolverRelaxed::solver_map = {{GUROBI, "GUROBI"}, {CPLEX, "CPLEX"}, {HIGHS, "HIGHS"}}; 


void operations_research::FeasibilitySolverRelaxed::create_variables()
{
    // const double infinity = solver->infinity();
    
    for (unsigned long i = 0; i < data->n; i++) 
    {
        for (unsigned long t = 0; t < nparts; t++)
        {
            x.push_back(solver->MakeNumVar(0.0, 1, "x_" + std::to_string(i) + "_" + std::to_string(t)));
            r.push_back(solver->MakeNumVar(0.0, 1, "r_" + std::to_string(i) + "_" + std::to_string(t)));
            f.push_back(solver->MakeNumVar(0.0, 1, "f_" + std::to_string(i) + "_" + std::to_string(t)));
            g.push_back(solver->MakeNumVar(0.0, 1, "g_" + std::to_string(i) + "_" + std::to_string(t)));
        }
    }

    for (unsigned long i = 0; i < data->n; i++) 
    {
        for (unsigned long j = 0; j < data->n; j++)
            for (unsigned long t = 0; t < nparts; t++)
            {
                a.push_back(solver->MakeNumVar(0.0, 1, "a_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(t)));
                for (unsigned long u = 0; u < nparts; u++)
                    h.push_back(solver->MakeNumVar(0.0, 1, "h_" + std::to_string(i) + "_" + std::to_string(j) + "_" + std::to_string(t) + "_" + std::to_string(u)));
            }
    }

    for (unsigned long i = 0; i < data->n; i++) 
    {
        for (unsigned long t = 0; t < nparts; t++)
            for (unsigned long u = 0; u < nparts; u++)
                w.push_back(solver->MakeNumVar(0.0, 1, "w_" + std::to_string(i) + "_" + std::to_string(t) + "_" + std::to_string(u)));
    }

    LOG(INFO) << data->n;
    LOG(INFO) << "Number of variables = " << solver->NumVariables();
}


void operations_research::FeasibilitySolverRelaxed::create_model()
{
    create_variables();
    // create_objective();
    create_constraints();
}

void operations_research::FeasibilitySolverRelaxed::create_constraints()
{
    // arcs and vertices
    std::cout << "sum_x" << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
    {
        LinearExpr sum_x;
        for (long unsigned t = 0; t < nparts; ++t)
            sum_x += x[get_index_d2({i, data->n},{t, nparts})];

        solver->MakeRowConstraint(sum_x <= 1.0);
    }

    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned j = 0; j < data->n; ++j)
            if (data->adj_mtx_reduction[i][j])
                for (long unsigned u = 0; u < nparts; ++u)
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
         
    for (long unsigned t = 0; t < nparts; ++t)
        for (long unsigned i = 0; i < data->n; ++i)
            for (long unsigned j = 0; j < data->n; ++j) 
            {
                if (data->adj_mtx_reduction[i][j]) 
                {
                    LinearExpr lhs;
                    auto idx_xi = get_index_d2({i, data->n}, {t, nparts});
                    auto idx_xj = get_index_d2({j, data->n}, {t, nparts});
                    auto idx_a = get_index_d3({i, data->n}, {j, data->n}, {t, nparts});
                    lhs = LinearExpr(x[idx_xi]) - LinearExpr(a[idx_a]);
                    solver->MakeRowConstraint(lhs >= 0.0); 
                    lhs = LinearExpr(x[idx_xj]) - LinearExpr(a[idx_a]);
                    solver->MakeRowConstraint(lhs >= 0.0);
                    lhs = LinearExpr(a[idx_a]) - LinearExpr(x[idx_xi]) - LinearExpr(x[idx_xj]);
                    solver->MakeRowConstraint(lhs >= -1.0);
                }
                
            }  

    
    // roots, vertices and arcs
    // the root is the vertex data->n-1
    LinearExpr lhs = LinearExpr(r[get_index_d2({data->n-1, data->n}, {0, nparts})]);
    solver->MakeRowConstraint(lhs == 1.0);
    
    for (long unsigned t = 0; t < nparts; ++t)
    {
        LinearExpr sum_r;
        for (long unsigned i = 0; i < data->n; ++i)
            sum_r += r[get_index_d2({i, data->n},{t, nparts})];

        solver->MakeRowConstraint(sum_r == 1.0);
    }
    
    // the root is the vertex data->n-1
    for (long unsigned j = 0; j < data->n-1; ++j) 
        for (long unsigned t = 0; t < nparts; ++t)
            solver->MakeRowConstraint(LinearExpr(x[get_index_d2({j,data->n},{t, nparts})]) >= LinearExpr(r[get_index_d2({j,data->n},{t, nparts})]));

    for (long unsigned j = 0; j < data->n-1; ++j) 
    {
        LinearExpr sum_r;
        LinearExpr sum_a;
        for (long unsigned t = 0; t < nparts; ++t)
        {
            for (long unsigned i = 0; i < data->n; ++i)
            {
                if (data->adj_mtx_reduction[i][j])
                    sum_a += a[get_index_d3({i, data->n},{j,data->n},{t, nparts})];
            }
            sum_r += r[get_index_d2({j,data->n},{t, nparts})];
        }
        solver->MakeRowConstraint(sum_r + sum_a == 1.0);
    }

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

    //define f 


    for (long unsigned t = 0; t < nparts; ++t)
    {
        LinearExpr sum_f;
        for (long unsigned i = 0; i < data->n; ++i)
            sum_f += f[get_index_d2({i,data->n},{t, nparts})];
        if (t > 0)
            solver->MakeRowConstraint(sum_f == 1.0);
        else 
            solver->MakeRowConstraint(sum_f == 0.0);
    }

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
        
/**/
                             
        

    
    std::string text;
    solver->ExportModelAsLpFormat(false, &text);
    std::ofstream myfile;
    myfile.open ("lp_model.txt");
    myfile << text;
    myfile.close();
    LOG(INFO) << "Number of constraints = " << solver->NumConstraints();
}

void operations_research::FeasibilitySolverRelaxed::solve_model()
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
            if (x[get_index_d2({i, data->n},{t, nparts})]->solution_value() >= 0.000000001)
                std::cout << "x(" << i << "," << t << ")" << std::endl;

    std::cout << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned j = 0; j < data->n; ++j)
            for (long unsigned t = 0; t < nparts; ++t)
                if (a[get_index_d3({i, data->n},{j, data->n},{t, nparts})]->solution_value() >= 0.000000001)
                    std::cout << "a(" << i << "," << j << "," << t << ")" << std::endl;

    std::cout << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned t = 0; t < nparts; ++t)
            if (f[get_index_d2({i, data->n},{t, nparts})]->solution_value() >= 0.000000001)
                std::cout << "f(" << i << "," << t << ")" << std::endl;

    std::cout << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned t = 0; t < nparts; ++t)
            if (r[get_index_d2({i, data->n},{t, nparts})]->solution_value() >= 0.000000001)
                std::cout << "r(" << i << "," << t << ")" << std::endl;

    std::cout << std::endl;
    for (long unsigned i = 0; i < data->n; ++i)
        for (long unsigned t = 0; t < nparts; ++t)
            if (g[get_index_d2({i, data->n},{t, nparts})]->solution_value() >= 0.000000001)
                std::cout << "g(" << i << "," << t << ")" << std::endl;
}