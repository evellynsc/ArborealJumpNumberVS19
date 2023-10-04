main_src=src/main.cpp
reader_src=src/data_reader.cpp
reader_h=src/headers/data_reader.hpp

GRB_FLAGS=-lgurobi_c++ -lgurobi100 -lm
GRB_INCDIR=${GUROBI_HOME}/include
GRB_LIBDIR=${GUROBI_HOME}/lib

HEADERS_INCDIR=src/headers

CPP_FLAGS=-std=c++17 -W -Wall

all: ajns

ajns: main.o data_reader.o
	g++ $(CPP_FLAGS) -o ajns $(main_src) $(reader_src) -I$(GRB_INCDIR) -I$(HEADERS_INCDIR) -L$(GRB_LIBDIR) $(GRB_FLAGS)

main.o: $(main_src)
	g++ $(CPP_FLAGS) -o main.o -c $(main_src) -I$(GRB_INCDIR) -I$(HEADERS_INCDIR) -L$(GRB_LIBDIR) $(GRB_FLAGS)

data_reader.o: $(reader_src)
	g++ $(CPP_FLAGS) -o data_reader.o -c $(reader_src) -I$(HEADERS_INCDIR)

clean:
	rm -rf *.o *~ ajns
