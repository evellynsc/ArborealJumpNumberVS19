SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CPLEXDIR      = /home/lapo/cplex/cplex201
CONCERTDIR    = /home/lapo/cplex/cplex201/concert
BOOSTLIBDIR = /home/lapo/evellyn/boost_1_78_0
CPLEXLIBDIR   = $(CPLEXDIR)/cplex/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CLNFLAGS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) -L$(BOOSTLIBDIR)  -lilocplex -lconcert -lcplex -m64 -lm -lpthread -ldl


COPT  = -m64 -O2 -fPIC -fexceptions -DNDEBUG -DIL_STD -DLONG_MAX=0x7FFFFFFFL
GENERALINCDIR   = -I ArborealJumpNumber
CPLEXINCDIR   = -I $(CPLEXDIR)/cplex/include -I $(CPLEXDIR)/concert/include
BOOSTINCDIR   = -I $(BOOSTLIBDIR)
CFLAGS = $(COPT) $(GENERALINCDIR) 
CFLAGS2  = $(COPT) $(CPLEXINCDIR) $(BOOSTINCDIR)

CC=g++ -std=c++17 -w
CC_VALGRIND=g++ -std=c++17 -g -O0

PROG_DIR=ArborealJumpNumber
PROG_BIN=bin

MAIN_SRC=$(PROG_DIR)/ArborealJumpNumber.cpp

FLOW_SRC=$(PROG_DIR)/algorithms/flow.cpp
FLOW_H=$(PROG_DIR)/algorithms/flow.h
FLOW_OBJ=$(PROG_BIN)/flow.o

ELEMENTARY_H=$(PROG_DIR)/base/elementary.h
ELEMENTARY_OBJ=$(PROG_BIN)/elementary.o

GRAPH_H=$(PROG_DIR)/base/graph.h
GRAPH_OBJ=$(PROG_BIN)/graph.o

INSTANCE_H=$(PROG_DIR)/base/instance.h
INSTANCE_OBJ=$(PROG_BIN)/instance.o

PROP_H	=$(PROG_DIR)/base/properties.h
PROP_OBJ=$(PROG_BIN)/properties.o

SOLUTION_SRC=$(PROG_DIR)/base/solution.cpp
SOLUTION_H=$(PROG_DIR)/base/solution.h
SOLUTION_OBJ=$(PROG_BIN)/solution.o

HEURISTIC_SRC	=$(PROG_DIR)/heuristic/minimal_extension.cpp
HEURISTIC_H	=$(PROG_DIR)/heuristic/minimal_extension.h
HEURISTIC_OBJ	=$(PROG_BIN)/minimal_extension.o

INSTANCE_GENERATOR_SRC	=$(PROG_DIR)/preprocessing/instance_generator.cpp
INSTANCE_GENERATOR_H	=$(PROG_DIR)/preprocessing/instance_generator.h
INSTANCE_GENERATOR_OBJ	=$(PROG_BIN)/instance_generator.o

READER_SRC	=$(PROG_DIR)/preprocessing/reader.cpp
READER_H	=$(PROG_DIR)/preprocessing/reader.h
READER_OBJ	=$(PROG_BIN)/reader.o

PROBLEM_DATA_H		=$(PROG_DIR)/preprocessing/problem_data.h
PROBLEM_DATA_OBJ	=$(PROG_BIN)/problem_data.o

MODEL_SRC	=$(PROG_DIR)/solver/model.cpp
MODEL_H		=$(PROG_DIR)/solver/model.h
MODEL_OBJ	=$(PROG_BIN)/model.o

FLOW_MODEL_SRC	=$(PROG_DIR)/solver/multi_flow_model.cpp
FLOW_MODEL_H	=$(PROG_DIR)/solver/multi_flow_model.h
FLOW_MODEL_OBJ	=$(PROG_BIN)/multi_flow_model.o

EXPONENTIAL_MODEL_SRC	=$(PROG_DIR)/solver/exponential_model.cpp
EXPONENTIAL_MODEL_H		=$(PROG_DIR)/solver/exponential_model.h
EXPONENTIAL_MODEL_OBJ	=$(PROG_BIN)/exponential_model.o

SOLVER_SRC	=$(PROG_DIR)/solver/solver.cpp
SOLVER_H	=$(PROG_DIR)/solver/solver.h
SOLVER_OBJ	=$(PROG_BIN)/solver.o

MF_SOLVER_SRC	=$(PROG_DIR)/solver/MFSolver.cpp
MF_SOLVER_H	=$(PROG_DIR)/solver/MFSolver.h
MF_SOLVER_OBJ	=$(PROG_BIN)/MFSolver.o

BC_SOLVER_SRC	=$(PROG_DIR)/solver/BCSolver.cpp
BC_SOLVER_H	=$(PROG_DIR)/solver/BCSolver.h
BC_SOLVER_OBJ	=$(PROG_BIN)/BCSolver.o

CHAR_SOLVER_SRC	=$(PROG_DIR)/solver/CharacterizationBasedFormulation.cpp
CHAR_SOLVER_H	=$(PROG_DIR)/solver/CharacterizationBasedFormulation.h
CHAR_SOLVER_OBJ	=$(PROG_BIN)/CharacterizationBasedFormulation.o

KERNEL_SOLVER_SRC	=$(PROG_DIR)/solver/Kernel.cpp
KERNEL_SOLVER_H	=$(PROG_DIR)/solver/Kernel.h
KERNEL_SOLVER_OBJ	=$(PROG_BIN)/Kernel.o

SOLVER_PARAMS_H		=$(PROG_DIR)/solver/solver_params.h
SOLVER_PARAMS_OBJ	=$(PROG_BIN)/solver_params.o

PI_INEQUALITY_H		=$(PROG_DIR)/solver/callbacks/pi_inequality.h
PI_INEQUALITY_OBJ	=$(PROG_BIN)/pi_inequality.o

PI_SIGMA_INEQUALITY_H	=$(PROG_DIR)/solver/callbacks/pi_sigma_inequality.h
PI_SIGMA_INEQUALITY_OBJ	=$(PROG_BIN)/pi_sigma_inequality.o

PREC_INEQUALITY_H	=$(PROG_DIR)/solver/callbacks/precedence_inequality.h
PREC_INEQUALITY_OBJ	=$(PROG_BIN)/precedence_inequality.o

REACHES_INEQUALITY_H	=$(PROG_DIR)/solver/callbacks/reachability.h
REACHES_INEQUALITY_OBJ	=$(PROG_BIN)/reachability.o

ADD_MIN_CUTS_LC_H	=$(PROG_DIR)/solver/callbacks/add_min_cuts_lc.h
ADD_MIN_CUTS_LC_OBJ	=$(PROG_BIN)/add_min_cuts_lc.o

ADD_MIN_CUTS_UC_H	=$(PROG_DIR)/solver/callbacks/add_min_cuts_uc.h
ADD_MIN_CUTS_UC_OBJ	=$(PROG_BIN)/add_min_cuts_uc.o

SIGMA_INEQUALITY_H	=$(PROG_DIR)/solver/callbacks/sigma_inequality.h
SIGMA_INEQUALITY_OBJ	=$(PROG_BIN)/sigma_inequality.o

UTILS_CALLBACKS_H	=$(PROG_DIR)/utils/callbacks.h
UTILS_CALLBACKS_OBJ	=$(PROG_BIN)/callbacks.o

UTILS_PREPROCESSING_H	=$(PROG_DIR)/utils/preprocessing.h
UTILS_PREPROCESSING_OBJ	=$(PROG_BIN)/preprocessing.o

ELEMENTARY_OBJ: $(ELEMENTARY_H)
	$(CC) $(CFLAGS2) -c $(ELEMENTARY_H) -o $(ELEMENTARY_OBJ)

GRAPH_OBJ: $(GRAPH_H)
	$(CC) $(CFLAGS2) -c $(GRAPH_H) -o $(GRAPH_OBJ)

INSTANCE_OBJ: $(INSTANCE_H)
	$(CC) $(CFLAGS2) -c $(INSTANCE_H) -o $(INSTANCE_OBJ)

SOLUTION_OBJ: $(SOLUTION_SRC) $(SOLUTION_H)
	$(CC) $(CFLAGS2) -c $(SOLUTION_SRC) -o $(SOLUTION_OBJ)

HEURISTIC_OBJ: $(HEURISTIC_SRC) $(HEURISTIC_H)
	$(CC) $(CFLAGS2) -c $(HEURISTIC_SRC) -o $(HEURISTIC_OBJ)

INSTANCE_GENERATOR_OBJ: $(INSTANCE_GENERATOR_SRC) $(INSTANCE_GENERATOR_H)
	$(CC) $(CFLAGS2) -c $(INSTANCE_GENERATOR_SRC) -o $(INSTANCE_GENERATOR_OBJ)

READER_OBJ: $(READER_SRC) $(READER_H)
	$(CC) $(CFLAGS2) -c $(READER_SRC) -o $(READER_OBJ)

PROBLEM_DATA_OBJ: $(PROBLEM_DATA_H)
	$(CC) $(CFLAGS2) -c $(PROBLEM_DATA_H) -o $(PROBLEM_DATA_OBJ)

FLOW_OBJ: $(FLOW_SRC) $(FLOW_H)
	$(CC) $(CFLAGS2) -c $(FLOW_SRC) -o $(FLOW_OBJ)

MODEL_OBJ: $(MODEL_SRC) $(MODEL_H)
	$(CC) $(CFLAGS2) -c $(MODEL_SRC) -o $(MODEL_OBJ)

EXPONENTIAL_MODEL_OBJ: $(EXPONENTIAL_MODEL_SRC) $(EXPONENTIAL_MODEL_H)
	$(CC) $(CFLAGS2) -c $(EXPONENTIAL_MODEL_SRC) -o $(EXPONENTIAL_MODEL_OBJ)
	
FLOW_MODEL_OBJ: $(FLOW_MODEL_SRC) $(FLOW_MODEL_H)
	$(CC) $(CFLAGS2) -c $(FLOW_MODEL_SRC) -o $(FLOW_MODEL_OBJ)

SOLVER_OBJ: $(SOLVER_SRC) $(SOLVER_H)
	$(CC) $(CFLAGS2) -c $(SOLVER_SRC) -o $(SOLVER_OBJ)

MF_SOLVER_OBJ: $(MF_SOLVER_SRC) $(MF_SOLVER_H)
	$(CC) $(CFLAGS2) -c $(MF_SOLVER_SRC) -o $(MF_SOLVER_OBJ)

CHAR_SOLVER_OBJ:  $(CHAR_SOLVER_SRC) $(CHAR_SOLVER_H)
	$(CC) $(CFLAGS2) -c $(CHAR_SOLVER_SRC) -o $(CHAR_SOLVER_OBJ)

KERNEL_SOLVER_OBJ:  $(KERNEL_SOLVER_SRC) $(KERNEL_SOLVER_H)
	$(CC) $(CFLAGS2) -c $(KERNEL_SOLVER_SRC) -o $(KERNEL_SOLVER_OBJ)

BC_SOLVER_OBJ: $(BC_SOLVER_H)
	$(CC) $(CFLAGS2) -c $(BC_SOLVER_SRC) -o $(BC_SOLVER_OBJ)

SOLVER_PARAMS_OBJ: $(SOLVER_PARAMS_H)
	$(CC) $(CFLAGS2) -c $(SOLVER_PARAMS_H) -o $(SOLVER_PARAMS_OBJ)

PI_INEQUALITY_OBJ: $(PI_INEQUALITY_H)
	$(CC) $(CFLAGS2) -c $(PI_INEQUALITY_H) -o $(PI_INEQUALITY_OBJ)

PI_SIGMA_INEQUALITY_OBJ: $(PI_SIGMA_INEQUALITY_H)
	$(CC) $(CFLAGS2) -c $(PI_SIGMA_INEQUALITY_H) -o $(PI_SIGMA_INEQUALITY_OBJ)

PREC_INEQUALITY_OBJ: $(PREC_INEQUALITY_H)
	$(CC) $(CFLAGS2) -c $(PREC_INEQUALITY_H) -o $(PREC_INEQUALITY_OBJ)

REACHES_INEQUALITY_OBJ: $(REACHES_INEQUALITY_H)
	$(CC) $(CFLAGS2) -c $(REACHES_INEQUALITY_H) -o $(REACHES_INEQUALITY_OBJ)

ADD_MIN_CUTS_LC_OBJ: $(ADD_MIN_CUTS_LC_H)
	$(CC) $(CFLAGS2) -c $(ADD_MIN_CUTS_LC_H) -o $(ADD_MIN_CUTS_LC_OBJ)

ADD_MIN_CUTS_UC_OBJ: $(ADD_MIN_CUTS_UC_H)
	$(CC) $(CFLAGS2) -c $(ADD_MIN_CUTS_UC_H) -o $(ADD_MIN_CUTS_UC_OBJ)

SIGMA_INEQUALITY_OBJ: $(SIGMA_INEQUALITY_H)
	$(CC) $(CFLAGS2) -c $(SIGMA_INEQUALITY_H) -o $(SIGMA_INEQUALITY_OBJ)

UTILS_CALLBACKS_OBJ: $(UTILS_CALLBACKS_H)
	$(CC) $(CFLAGS2) -c $(UTILS_CALLBACKS_H) -o $(UTILS_CALLBACKS_OBJ)

UTILS_PREPROCESSING_OBJ: $(UTILS_PREPROCESSING_H)
	$(CC) $(CFLAGS2) -c $(UTILS_PREPROCESSING_H) -o $(UTILS_PREPROCESSING_OBJ)

ajns: ELEMENTARY_OBJ GRAPH_OBJ INSTANCE_OBJ SOLUTION_OBJ HEURISTIC_OBJ INSTANCE_GENERATOR_OBJ READER_OBJ PROBLEM_DATA_OBJ FLOW_OBJ MODEL_OBJ EXPONENTIAL_MODEL_OBJ FLOW_MODEL_OBJ SOLVER_OBJ MF_SOLVER_OBJ BC_SOLVER_OBJ SOLVER_PARAMS_OBJ PI_INEQUALITY_OBJ PI_SIGMA_INEQUALITY_OBJ ADD_MIN_CUTS_LC_OBJ ADD_MIN_CUTS_UC_OBJ PREC_INEQUALITY_OBJ REACHES_INEQUALITY_OBJ SIGMA_INEQUALITY_OBJ UTILS_CALLBACKS_OBJ UTILS_PREPROCESSING_OBJ CHAR_SOLVER_OBJ KERNEL_SOLVER_OBJ
	$(CC) $(CFLAGS2) $(ELEMENTARY_SRC) $(GRAPH_H) $(INSTANCE_H) $(SOLUTION_SRC) $(HEURISTIC_SRC) $(INSTANCE_GENERATOR_SRC) $(READER_SRC) $(PROBLEM_DATA_H) $(FLOW_SRC) $(MODEL_SRC) $(EXPONENTIAL_MODEL_SRC) $(FLOW_MODEL_SRC) $(SOLVER_SRC) $(MF_SOLVER_SRC) $(BC_SOLVER_SRC) $(SOLVER_PARAMS_H) $(CHAR_SOLVER_H) $(KERNEL_SOLVER_H) $(PI_INEQUALITY_H) $(PI_SIGMA_INEQUALITY_H) $(PREC_INEQUALITY_H) $(REACHES_INEQUALITY_H) $(ADD_MIN_CUTS_UC_H) $(ADD_MIN_CUTS_LC_H) $(SIGMA_INEQUALITY_H) $(UTILS_CALLBACKS_H) $(UTILS_PREPROCESSING_H) $(MAIN_SRC) -o ajns $(CLNFLAGS)

clean:
	rm $(PROG_BIN)/*.o


#g++ -std=c++17 -w -m64 -O2 -fPIC -fexceptions -DNDEBUG -DIL_STD -DLONG_MAX=0x7FFFFFFFL -I /home/lapo/cplex/cplex201/cplex/include -I /home/lapo/cplex/cplex201/concert/include -I /home/lapo/evellyn/boost_1_78_0  ArborealJumpNumber/base/graph.h ArborealJumpNumber/base/instance.h ArborealJumpNumber/base/solution.cpp ArborealJumpNumber/heuristic/minimal_extension.cpp ArborealJumpNumber/preprocessing/instance_generator.cpp ArborealJumpNumber/preprocessing/reader.cpp ArborealJumpNumber/preprocessing/problem_data.h ArborealJumpNumber/algorithms/flow.cpp ArborealJumpNumber/solver/model.cpp ArborealJumpNumber/solver/exponential_model.cpp ArborealJumpNumber/solver/multi_flow_model.cpp ArborealJumpNumber/solver/solver.cpp ArborealJumpNumber/solver/MFSolver.cpp ArborealJumpNumber/solver/BCSolver.cpp ArborealJumpNumber/solver/solver_params.h ArborealJumpNumber/solver/CharacterizationBasedFormulation.h ArborealJumpNumber/solver/Kernel.h ArborealJumpNumber/solver/callbacks/pi_inequality.h ArborealJumpNumber/solver/callbacks/pi_sigma_inequality.h ArborealJumpNumber/solver/callbacks/precedence_inequality.h ArborealJumpNumber/solver/callbacks/reachability.h ArborealJumpNumber/solver/callbacks/add_min_cuts_uc.h ArborealJumpNumber/solver/callbacks/add_min_cuts_lc.h ArborealJumpNumber/solver/callbacks/sigma_inequality.h ArborealJumpNumber/utils/callbacks.h ArborealJumpNumber/utils/preprocessing.h ArborealJumpNumber/ArborealJumpNumber.cpp -o ajns -L/home/lapo/cplex/cplex201/cplex/lib/x86-64_linux/static_pic -L/home/lapo/cplex/cplex201/concert/lib/x86-64_linux/static_pic -L/home/lapo/evellyn/boost_1_78_0  -lilocplex -lconcert -lcplex -m64 -lm -lpthread -ldl