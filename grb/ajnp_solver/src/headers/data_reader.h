#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <CXXGraph/CXXGraph.hpp>

#include "node.h"
#include "edge.h"

class DataReader {

    std::ifstream ifs;
    std::string filename;

    public:
        DataReader(std::string _filename);
        std::unordered_map<std::string, std::shared_ptr<CXXGraph::Node<NodeData>>> create_nodes(std::string, int);
        CXXGraph::T_EdgeSet<NodeData> create_edges(int, std::unordered_map<std::string, std::shared_ptr<CXXGraph::Node<NodeData>>>);
        bool open_file();
        void read_matrix();
        void read_pairs();
};
