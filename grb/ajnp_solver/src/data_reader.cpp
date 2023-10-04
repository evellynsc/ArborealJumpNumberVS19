#include "headers/data_reader.h"

// using MyNode = CXXGraph::Node<NodeData>;

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

std::unordered_map<std::string, std::shared_ptr<CXXGraph::Node<NodeData>>> DataReader::create_nodes(std::string l, int n) {
    std::unordered_map<std::string, std::shared_ptr<CXXGraph::Node<NodeData>>> nodes;
    std::string id;
    if (l == "n")
        for (auto i = 0; i < n; i++) {
            auto s = std::to_string(i);
            nodes.emplace(s, std::make_shared<CXXGraph::Node<NodeData>>(s, NodeData(i)));
        }
    else
        for (auto i = 0; i < n; i++) {
            ifs >> id;
            nodes.emplace(id, std::make_shared<CXXGraph::Node<NodeData>>(id, NodeData(i)));
        }
    return nodes;
}

CXXGraph::T_EdgeSet<NodeData> DataReader::create_edges(int m, std::unordered_map<std::string, std::shared_ptr<CXXGraph::Node<NodeData>>> nodes) {
    using Edge_t = CXXGraph::DirectedWeightedEdge<NodeData>;
    CXXGraph::T_EdgeSet<NodeData> edges;
    std::string u, v;
    while (ifs >> u) {
        ifs >> v;
        int idx_edge = nodes[u]->getData().idx*m + nodes[v]->getData().idx;
        Edge_t edge(idx_edge, nodes[u], nodes[v], 0.0);
        edges.insert(std::make_shared<Edge_t>(edge));
    }
    if (edges.size() != m) {
        std::cerr << "Error: wrong number of edges." << std::endl;
        std::exit(0);
    }
    return edges;
}


void DataReader::read_pairs() {
    int n, m;
    std::string l;

    ifs >> l;
    ifs >> n;
    ifs >> m;

    auto nodes = create_nodes(l, n);
    auto edges = create_edges(m, nodes);

    CXXGraph::Graph<NodeData> graph(edges);
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
