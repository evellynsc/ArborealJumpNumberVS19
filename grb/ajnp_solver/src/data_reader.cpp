#include "headers/data_reader.hpp"

DataReader::DataReader(std::string _filename) {
    this->filename = _filename;       	
}

bool DataReader::open_file() {
    ifs.open(this->filename.c_str());
    if (!ifs) {
        std::cerr << "Unable to open file\n" << this->filename << std::endl;
        return false; // terminate with error
    }

    int type;
    ifs >> type;

    switch (type)
    {
    case 1:
        read_pairs();
        break;
    
    default:
        read_matrix();
        break;
    }

    return true;
}

void DataReader::read_pairs() {
    int n, m, u, v;
    char l;

    ifs >> n;
    ifs >> m;
    ifs >> l; 
    std::cout << n << " " << m << " " << l << std::endl;

    while (ifs.good()) {
        ifs >> u;
        ifs >> v;
        std::cout << u << "," << v << std::endl;
    }
}

void DataReader::read_matrix() {
    int n;
    char l;
        
    ifs >> n;
    ifs >> l;

    while (ifs.good()) {
        int pos;
        for (int i = 0u; i < n; i++) {
            ifs >> pos;
        }
    }
}