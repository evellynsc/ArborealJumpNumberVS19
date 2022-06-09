SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic
CPLEXDIR      = /home/lapo/cplex/cplex201
CONCERTDIR    = /home/lapo/cplex/cplex201/concert
BOOSTLIBDIR = /home/lapo/evellyn/boost_1_79_0
CPLEXLIBDIR   = $(CPLEXDIR)/cplex/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CLNFLAGS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) -L$(BOOSTLIBDIR)  -lilocplex -lconcert -lcplex -m64 -lm -lpthread


COPT  = -m64 -O2 -fPIC -fexceptions -DNDEBUG -DIL_STD -DLONG_MAX=0x7FFFFFFFL
GENERALINCDIR   = -I ArborealJumpNumber
CPLEXINCDIR   = -I $(CPLEXDIR)/cplex/include -I $(CPLEXDIR)/concert/include
CFLAGS = $(COPT) $(GENERALINCDIR) 
CFLAGS2  = $(COPT) $(CPLEXINCDIR)

CC=g++ -std=c++17 -w
CC_VALGRIND=g++ -std=c++17 -g -O0

PROG_DIR=ArborealJumpNumber
PROG_BIN=bin

MAIN_SRC=$(PROG_DIR)/ArborealJumpNumber.cpp

ARQUIVOS_SRC = $(wildcard **/*.cpp)
ARQUIVOS_H = $(wildcard **/*.h)
ARQUIVOS_OBJ = $(PROG_BIN)/*.o

# SEUARQUIVO_SRC=$(PROG_DIR)/SEU_ARQUIVO.cpp
# SEUARQUIVO_H=$(PROG_DIR)/SEU_ARQUIVO.h
# SEUARQUIVO_OBJ=$(PROG_BIN)/SEU_ARQUIVO.o

# SUBDIRS := \
# ArborealJumpNumber/algorithms \
# ArborealJumpNumber/base \
# ArborealJumpNumber/heuristic \
# ArborealJumpNumber \
# ArborealJumpNumber/output \
# ArborealJumpNumber/preprocessing \
# ArborealJumpNumber/solver \
# ArborealJumpNumber/solver/callbacks \
# ArborealJumpNumber/utils \

SEUARQUIVO_OBJ: $(ARQUIVOS_SRC) $(ARQUIVOS_H)
	$(CC) $(CFLAGS) $(CFLAGS2) -c $(ARQUIVOS_SRC) -o $(ARQUIVOS_OBJ)

stop: SEUARQUIVO_OBJ
	$(CC) $(CFLAGS) $(CFLAGS2) $(ARQUIVOS_SRC) $(MAIN_SRC) -o stop $(CLNFLAGS)

clean:
	rm $(PROG_BIN)/*.o
