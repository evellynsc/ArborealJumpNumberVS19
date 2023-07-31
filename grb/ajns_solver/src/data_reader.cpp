#include "headers/data_reader.h"

DataReader::DataReader(std::string _filename)
{
    this->filename = _filename;
    this->file_extension = TXT;
}

DataReader::DataReader(std::string _filename, FileExtension _file_extension)
{
    this->filename = _filename;
    this->file_extension = _file_extension;
}

void DataReader::open_file()
{
    ifs.open(this->filename.c_str());
    if (!ifs)
    {
        std::cerr << "Unable to open file " << this->filename << std::endl;
        exit(0); // terminate with error
    }
}

void DataReader::read_pairs()
{
    long unsigned int n, m;
    ifs >> n;
    ifs >> m;

    // auto nodes = create_nodes(l, n);
    // auto edges = create_edges(m, nodes);

    // CXXGraph::Graph<NodeData> graph(edges);
}

void DataReader::read_matrix(long unsigned n, std::vector<std::vector<bool>>& adj_mtx)
{
    adj_mtx = std::vector<std::vector<bool>>(n, std::vector<bool>(n,0));
    
    bool e;    
    for (long unsigned i = 0; i < n; i++) {
        for (long unsigned j = 0; j < n; j++) {
            ifs >> e;            
            if (e == 1)
                adj_mtx[i][j] = true; 
            // {

                // auto idx = node_map[std::to_string(i)] + node_map[std::to_string(j)]*n;
                // edge_map.insert({idx, {std::to_string(i), std::to_string(j)}});
            // }
        }
    }
}



void DataReader::read_nodes(long unsigned n, std::map<std::string, long unsigned>& node_map) {
    std::string label;

    for (long unsigned i = 0; i < n; i++) {
        ifs >> label;
        node_map.insert({label,i});
    }        
}

void DataReader::read(std::map<std::string, long unsigned>& node_map, std::unordered_map<long unsigned, std::pair<std::string, std::string>>& edge_map) {
    open_file();
    
    int type;
    ifs >> type;

    std::string l;
    ifs >> l;

    long unsigned int n;
    ifs >> n;

    // std::cout << l << " " << type << std::endl;
    if (l == "n") {
        for (long unsigned i = 0; i < n; i++) 
            node_map.insert({std::to_string(i),i});    
    } else {
        read_nodes(n, node_map);
    }
    
    switch (type)
    {
    case TXT:
        // read_matrix(node_map, edge_map);        
        break;

    case PR:        
        break;
    }
    ifs.close();

    // std::unique_ptr<Graph> graph = std::make_unique<Graph>(node_vct.size());
    // graph->add_edges(edge_map);
    // graph->print();
}

void DataReader::read(std::vector<std::string>& node_vct, std::vector<std::vector<bool>>& adj_mtx) {
    open_file();
    
    int type;
    ifs >> type;
    std::cout << type << " ";

    std::string l;
    ifs >> l;
    std::cout << l << " ";

    long unsigned int n;
    ifs >> n;
    std::cout << n << std::endl;

    // std::cout << l << " " << type << std::endl;
    if (l == "n") {
        for (long unsigned i = 0; i < n; i++) 
            node_vct.push_back(std::to_string(i));    
    } 
    // else {
    //     read_nodes(n, adj_mtx);
    // }
    
    switch (type)
    {
    case TXT:
        read_matrix(n, adj_mtx);        
        break;

    case PR:        
        break;
    }
    ifs.close();
}