#pragma once

#include <list>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include <memory>

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "data_reader.h"
#include "ortools/base/logging.h"

// FIXME: add atribute root
class Instance {
    std::string name;
    std::unique_ptr<DataReader> reader;

    long unsigned m_closure;
    long unsigned m_reduction;

    long unsigned n_original;

    long unsigned upper_bound;
    long unsigned lower_bound;




    std::vector<std::vector<bool>> adj_mtx_original;

    // map original to resultant
    std::vector<long unsigned> resultant_mapping;

    // TODO: create a class for Solution
    // Graph initial_solution;

    void preprocess();
    void remove_safe_leaves();

public:
    // std::unique_ptr<Graph> closure;
    // std::unique_ptr<Graph> reduction;
    // std::unique_ptr<Graph> input;

    // key is source_id + target_id*n
    std::unordered_map<long unsigned, Edge> edge_map;
    std::vector<Node> node_vct;

    long unsigned n;
    long unsigned m;
    std::unordered_map<long unsigned, std::vector<long unsigned>> adj_lst_original;
    std::unordered_map<long unsigned, std::vector<long unsigned>> adj_lst_artificial;
    std::vector<std::vector<bool>> adj_mtx_reduction;
    std::vector<std::vector<bool>> adj_mtx_closure;
    std::vector<std::vector<bool>> adj_mtx_resultant;

    Instance() : n(0), m(0), m_closure(0), m_reduction(0), n_original(0)
    {
        std::cerr << "You need to provide a path file!!" << std::endl;
        exit(0);
    };
    Instance(std::string, FileExtension, bool);
    void create();
    void add_root();
    void remove_safe_vertices();
    void print_resultant_graph();
    void print_resultant_reduction();
    void print_resultant_closure();
    ~Instance() = default;
};
