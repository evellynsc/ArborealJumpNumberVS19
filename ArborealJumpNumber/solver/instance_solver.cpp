#pragma once

#include "../headers/instance_solver.h"

#include "../headers/answer.h"
#include "../headers/formulations.h"
#include "../headers/instance.h"
#include "../preprocessing/instance_generator.h"
#include "../headers/exact_solver.h"
#include "../preprocessing/reader.h"

Answer InstanceSolver::Solve(string file_output_name, int time_limit,
    double heuristics) const {
    Answer result = solver->Solve(time_limit, heuristics);
    if (not file_output_name.empty()) {
        ofstream myfile;
        /*myfile.open(file_output_name);
        myfile << result;*/
        myfile.close();
    }
    return result;
}

inline void InstanceSolver::PrintBest() {
    int tot = 0;
    cout << "#PrintSolution" << endl;
    //for (int i = 0; i < best_.size(); ++i) {
    //    int index = best_[i];
    //    cout << "( " << instance->arcs[index].second.first;
    //    cout << ", " << instance->arcs[index].second.second << ")";
    //    tot += internal_s_->arcs[index].first;
    //    cout << " = " << internal_s_->arcs[index].first << endl;
    //}
    //cout << "tot:" << tot << endl;
}

InstanceSolver InstanceSolver::GetInstance(std::string path_file_name,
    string formulation, int s) {
    auto input_file = reader(path_file_name);
    auto problem_data = input_file.read();
    auto generator = instance_generator();
    auto ajns_instance = generator.create_instance(problem_data);
    if (s != 0) {
        ajns_instance.set_num_jumps(s);
    }  
    return &InstanceSolver(Formulations::GetFormulation(ajns_instance, formulation));
}