#include "headers/graph.h"

Graph::Graph(int _n){
    // this->n = _n;
    // this->m = 0;
    // for (long unsigned i = 0; i < n; i++) {
    //     adj_lst.push_back(std::list<long unsigned>());
    //     adj_mtx.push_back(std::vector<bool>(n, false));
    // }
}
/*
void Graph::add_edge(long unsigned s, long unsigned t)
{
    node_map.insert({i, s});
    node_map.insert({j, t});
    adj_lst[i].emplace_back(j);
    adj_mtx[i][j] = true;
    m++;    
}

void Graph::add_edge(Edge& e)
{
    edge_set.insert(e);
    add_edge(e.get_source(), e.get_target());
}

void Graph::add_edges(std::unordered_set<Edge, EdgeHash>& e_set) 
{

    for (auto iter : e_set)
    {
        add_edge(iter.get_source(), iter.get_target());
    }
}

void Graph::print() {
    for (long unsigned s = 0; s < n; s++) {
        for (auto t : adj_lst[s]) {
            std::cout << s << " " << t << std::endl;
        }
    }
}
*/

// Graph::Graph(int _n){
//     this->n = _n;
//     this->m = 0;
//     for (long unsigned i = 0; i < n; i++) {
//         adj_lst.push_back(std::list<long unsigned>());
//         adj_mtx.push_back(std::vector<bool>(n, false));
//     }
// }

// void Graph::add_edge(const Node s, const Node t)
// {
//     auto i = s.get_id();
//     auto j = t.get_id();
//     node_map.insert({i, s});
//     node_map.insert({j, t});
//     adj_lst[i].emplace_back(j);
//     adj_mtx[i][j] = true;
//     m++;    
// }

// void Graph::add_edge(Edge& e)
// {
//     edge_set.insert(e);
//     add_edge(e.get_source(), e.get_target());
// }

// void Graph::add_edges(std::unordered_set<Edge, EdgeHash>& e_set) 
// {

//     for (auto iter : e_set)
//     {
//         add_edge(iter.get_source(), iter.get_target());
//     }
// }

// void Graph::print() {
//     for (long unsigned s = 0; s < n; s++) {
//         for (auto t : adj_lst[s]) {
//             std::cout << s << " " << t << std::endl;
//         }
//     }
// }