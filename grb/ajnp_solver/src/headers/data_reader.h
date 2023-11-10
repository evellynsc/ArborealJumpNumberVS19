#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <vector>
#include <unordered_map>
#include "global.h"

class DataReader {

    std::ifstream ifs;
    std::string filename;
    FileExtension file_extension;
    
    void open_file();
    void read_matrix(long unsigned, std::vector<std::vector<bool>>&);
    void read_pairs();
    void read_nodes(long unsigned,std::map<std::string, long unsigned>&);

    public:
        DataReader(std::string);
        DataReader(std::string, FileExtension);
        void read(std::map<std::string, long unsigned>&, std::unordered_map<long unsigned, std::pair<std::string, std::string>>&);
        void read(std::vector<std::string>&,std::vector<std::vector<bool>>&);
};
