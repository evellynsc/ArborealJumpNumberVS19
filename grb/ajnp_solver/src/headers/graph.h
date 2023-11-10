#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>
#include <map>
#include <memory>

#include "node.h"
#include "edge.h"

// using node_set = std::unordered_set<Node, NodeHash>;
class Graph {
    long unsigned n; //number of nodes
    long unsigned m; //number of edges

    std::vector<std::vector<bool>> adj_mtx; 
    std::unordered_map<std::unique_ptr<Node>, std::list<std::unique_ptr<Node>>> adj_lst;
    // std::vector<std::list<long unsigned>> adj_lst;
     
    std::unordered_map<std::unique_ptr<Node>, std::list<std::unique_ptr<Node>>> transpose;

public:
    Graph() : n(0), m(0) {};    
    Graph(int _n);

    void add_edge(long unsigned, long unsigned);
    void add_edge(std::pair<long unsigned, long unsigned>);
    void add_edges(std::vector<std::pair<long unsigned, long unsigned>>);

    void get_successors();
    void get_predecessors();

    void remove_vertex(long unsigned);

    void print();
};