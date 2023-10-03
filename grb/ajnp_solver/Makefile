all: solver

solver: main.o data_reader.o
	g++ -std=c++17 -o solver src/main.cpp src/data_reader.cpp -I${GUROBI_HOME}/include -L${GUROBI_HOME}/lib -lgurobi_c++ -lgurobi100 -lm  

main.o: src/main.cpp src/headers/data_reader.hpp
	g++ -std=c++17 -o main.o -c src/main.cpp -W -Wall -ansi -pedantic -I${GUROBI_HOME}/include -L${GUROBI_HOME}/lib -lgurobi_c++ -lgurobi100 -lm  

data_reader.o: src/data_reader.cpp src/headers/data_reader.hpp
	g++ -std=c++17 -o data_reader.o -c src/data_reader.cpp -W -Wall -ansi -pedantic -I${GUROBI_HOME}/include -L${GUROBI_HOME}/lib -lgurobi_c++ -lgurobi100 -lm  

clean:
	rm -rf *.o *~ solver



# # My second makefile

# # Name of the project
# PROJ_NAME=ajnp_solver

# GRB_INCDIR = ${GUROBI_HOME}/include
# GRB_LIBDIR = ${GUROBI_HOME}/lib
# GRB_FLAGS = -lgurobi_c++ -lgurobi100 -lm 


# # .c files
# C_SOURCE=$(wildcard src/*.cpp)

# # .h files
# H_SOURCE=$(wildcard src/headers/*.hpp)

# # Object files
# OBJ=$(C_SOURCE:.cpp=.o)

# # Compiler
# CC=g++

# # Flags for compiler
# CC_FLAGS=-c         \
#          -W         \
#          -Wall      \
#          -ansi      \
#          -pedantic  \
#          -std=c++17

# #
# # Compilation and linking
# #
# all: $(PROJ_NAME)

# $(PROJ_NAME): $(OBJ)
# 	$(CC) -I$(GRB_INCDIR) -L$(GRB_LIBDIR) $(GRB_FLAGS) -o $@ $^ -I$(GRB_INCDIR) -L$(GRB_LIBDIR) $(GRB_FLAGS)

# %.o: %.c %.h
# 	$(CC) -I$(GRB_INCDIR) -L$(GRB_LIBDIR) $(GRB_FLAGS) -o $@ $< -I$(GRB_INCDIR) -L$(GRB_LIBDIR) $(GRB_FLAGS) $(CC_FLAGS) 

# main.o: main.c $(H_SOURCE)
# 	$(CC) -I$(GRB_INCDIR) -L$(GRB_LIBDIR) $(GRB_FLAGS) -o $@ $< -I$(GRB_INCDIR) -L$(GRB_LIBDIR) $(GRB_FLAGS) $(CC_FLAGS)

# clean:
# 	rm -rf *.o $(PROJ_NAME) *~