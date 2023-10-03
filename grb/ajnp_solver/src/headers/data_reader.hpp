#pragma once

#include <iostream>
#include <fstream>


class DataReader {

    std::ifstream ifs;
    std::string filename;

    public:
    DataReader(std::string _filename);
    bool open_file();
    void read_matrix();
    void read_pairs();
};

