# GRB_FLAGS=-lgurobi_c++ -lgurobi100 -lm
# GRB_INCDIR=${GUROBI_HOME}/include
# GRB_LIBDIR=${GUROBI_HOME}/lib
# g++ $(CPP_FLAGS) $(LDFLAGS) -o ajns $(main_src) $(reader_src) $(graph_src) $(edge_src) $(node_src) $(instance_src) $(solver_src) $(INCLUDE_DIRS) -L$(GRB_LIBDIR) $(GRB_FLAGS)
# OR_TOOLS=/usr/local/include/ortools

ORTOOLS_LIB=/usr/local/lib/
ORTOOLS_FLAGS=-lortools

BOOST_LIBDIR=/home/evellyn/boost_1_83_0

LDFLAGS= -pthread -lpthread

HEADERS_INCDIR=src/headers

CPP_FLAGS=-std=c++17 -W -Wall -O3

INCLUDE_DIRS= -I$(BOOST_LIBDIR) -I$(HEADERS_INCDIR)

main_src=src/main.cpp

reader_src=src/data_reader.cpp
reader_h=src/headers/data_reader.h

node_src=src/node.cpp
node_h=src/headers/node.h

edge_src=src/edge.cpp
edge_h=src/headers/edge.h

graph_src=src/graph.cpp
graph_h=src/headers/graph.h

instance_src=src/instance.cpp
instance_h=src/headers/instance.h

solver_src=src/solver.cpp
solver_h=src/headers/solver.h

feasibility_src=src/feasibility_solver.cpp
feasibility_h=src/headers/feasibility_solver.h

all: ajns

ajns: main.o data_reader.o graph.o edge.o node.o instance.o feasibility.o
	g++ $(CPP_FLAGS) $(LDFLAGS) -o ajns $(main_src) $(reader_src) $(graph_src) $(edge_src) $(node_src) $(instance_src) $(feasibility_src) $(INCLUDE_DIRS) $(ORTOOLS_FLAGS)
	
main.o: $(main_src)
	g++ $(CPP_FLAGS) $(LDFLAGS) -o main.o -c $(main_src) $(INCLUDE_DIRS)

data_reader.o: $(reader_src)
	g++ $(CPP_FLAGS) $(LDFLAGS) -o data_reader.o -c $(reader_src) $(INCLUDE_DIRS)

node.o: $(node_src)
	g++ $(CPP_FLAGS) $(LDFLAGS) -o node.o -c $(node_src) $(INCLUDE_DIRS)

edge.o: $(edge_src)
	g++ $(CPP_FLAGS) $(LDFLAGS) -o edge.o -c $(edge_src) $(INCLUDE_DIRS)

graph.o: $(graph_src)
	g++ $(CPP_FLAGS) $(LDFLAGS) -o graph.o -c $(graph_src) $(INCLUDE_DIRS)

instance.o: $(instance_src)
	g++ $(CPP_FLAGS) $(LDFLAGS) -o instance.o -c $(instance_src) $(INCLUDE_DIRS)

solver.o: $(solver_src)
	g++ $(CPP_FLAGS) $(LDFLAGS) -o solver.o -c $(solver_src) $(INCLUDE_DIRS)

feasibility.o: $(feasibility_src)
	g++ $(CPP_FLAGS) $(LDFLAGS) -o feasibility.o -c $(feasibility_src) $(INCLUDE_DIRS) $(ORTOOLS_FLAGS)

clean:
	rm -rf *.o *~ ajns
