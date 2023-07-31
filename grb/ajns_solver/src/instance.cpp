#include "headers/instance.h"

#include <fstream>

Instance::Instance(std::string _path, FileExtension _fextension, bool _preprocess) {
    reader = std::make_unique<DataReader>(_path, _fextension);
    std::vector<std::string> node_vct; //o que fazer com isso?
    
    reader->read(node_vct, adj_mtx_original);

    n_original = adj_mtx_original.size();

    for (long unsigned i = 0; i < n_original; i++) 
        adj_mtx_original[i].push_back(false);
    n_original++;
    adj_mtx_original.push_back(std::vector<bool>(n_original, true));
    adj_mtx_original[n_original-1][n_original-1] = false;

    // mudar estrutura de dados para lista de adjacências?
    auto adj_mtx_aux = adj_mtx_original;
    for (long unsigned i = 0; i < n_original; i++)
        for (long unsigned j = 0; j < n_original; j++)
            for (long unsigned k = 0; k < n_original; k++)
                if (adj_mtx_aux[i][j] and adj_mtx_aux[j][k] and adj_mtx_aux[i][k]) 
                    adj_mtx_original[i][k] = false;
  

    auto removed = std::vector<bool>(n_original, false);
    auto in_degree = std::vector<long unsigned>(n_original, 0);
    auto out_degree = std::vector<long unsigned>(n_original, 0);

    for (long unsigned i = 0; i < n_original; i++)
        for (long unsigned j = 0; j < n_original; j++)
            if (adj_mtx_original[i][j]) 
            {
                out_degree[i]++;
                in_degree[j]++;
            }

    for (long unsigned k = 0; k < n_original; k++)
        for (long unsigned i = 0; i < n_original; i++)
            for (long unsigned j = 0; j < n_original; j++)
                if (adj_mtx_original[i][j] and not removed[j] and (in_degree[j] == 1 and out_degree[j] == 0))
                {
                    removed[j] = true;
                    out_degree[i]--;
                }

    for (long unsigned i = 0; i < n_original; i++)
        std::cout << i << ":" << removed[i] << std::endl;
    
    resultant_mapping = std::vector<long unsigned>(n_original, n_original+1);
    unsigned int new_idx = 0;
    for (long unsigned i = 0; i < n_original; i++) 
        if (not removed[i]) 
        {       
            resultant_mapping[i] = new_idx;
            new_idx++;
        }
    
    n = new_idx;
    adj_mtx_resultant = std::vector<std::vector<bool>>(n, std::vector<bool>(n, false));

    adj_lst_original = std::unordered_map<long unsigned, std::vector<long unsigned>>();
    for (long unsigned i = 0; i < n_original; i++) 
        if (not removed[i])
            for (long unsigned j = 0; j < n_original; j++)
                if (not removed[j] and adj_mtx_original[i][j]) 
                {
                    auto u = resultant_mapping[i];
                    auto v = resultant_mapping[j];
                    adj_mtx_resultant[u][v] = true;
                }

    adj_mtx_reduction = adj_mtx_resultant;
    adj_mtx_closure = adj_mtx_resultant;

    adj_lst_artificial = std::unordered_map<long unsigned, std::vector<long unsigned>>();
    for (long unsigned i = 0; i < n; i++)
        for (long unsigned j = 0; j < n; j++)
            for (long unsigned k = 0; k < n; k++)
                if (adj_mtx_resultant[i][j] and adj_mtx_resultant[j][k]) 
                {
                    adj_mtx_closure[i][k] = true;
                    adj_mtx_reduction[i][k] = false;
                }
                    
    
    for (long unsigned i = 0; i < n; i++)
        for (long unsigned j = i+1; j < n; j++)
            if (not adj_mtx_closure[i][j] and not adj_mtx_closure[j][i])
            {
                adj_mtx_resultant[i][j] = adj_mtx_resultant[j][i] = true;
                adj_lst_artificial[i].push_back(j);
                adj_lst_artificial[j].push_back(i);
            }

    // print_resultant_reduction();
    // print_resultant_closure();
    std::cout << "number of vertices " << n << std::endl;
    
    if (_preprocess) 
    {
        preprocess();
    }
}

void Instance::print_resultant_graph() 
{
    std::cout << "resultant graph\n";
    for (long unsigned i = 0; i < n; i++)
        for (long unsigned j = 0; j < n; j++)
            if (adj_mtx_resultant[i][j])
                std::cout << i << " " << j << std::endl;
}

void Instance::print_resultant_closure() 
{
    std::cout << "resultant closure\n";
    for (long unsigned i = 0; i < n; i++)
        for (long unsigned j = 0; j < n; j++)
            if (adj_mtx_closure[i][j])
                std::cout << i << " " << j << std::endl;
}

void Instance::print_resultant_reduction() 
{
    std::cout << "resultant reduction\n";
    for (long unsigned i = 0; i < n; i++)
        for (long unsigned j = 0; j < n; j++)
            if (adj_mtx_reduction[i][j])
                std::cout << i << " " << j << std::endl;
}

void Instance::preprocess() 
{}

void Instance::remove_safe_leaves() 
{}