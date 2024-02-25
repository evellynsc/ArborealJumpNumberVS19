#! /usr/bin/bash

g++ -m64 -g data_reader.cpp -o ex -I${GUROBI_HOME}/include -L${GUROBI_HOME}/lib -lgurobi_c++ -lgurobi100 -lm
g++ -m64 -g main.cpp -o ex -I${GUROBI_HOME}/include -L${GUROBI_HOME}/lib -lgurobi_c++ -lgurobi100 -lm
