# makefile for non-installed user
# author: xin cheng
# usage: change the following variable COLPACK_ROOT accordingly
#        delete OMP_FLAG=-fopenmp in MAC OS system
SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)
EXE = a.out


# compiler
COMPILER = g++      # gnu
OMP_FLAG = -fopenmp 

#COMPILER = icc      # intel(R)
#OMP_FLAG = -openmp

# compile flags
CCFLAGS = -Wall -std=c++11 $(OMP_FLAG)  -Ofast #-O3 
# link flags
LDFLAGS = -Wall -std=c++11 $(OMP_FLAG)  -Ofast #-O3


INCLUDES = -I./

all: $(EXE)

%.o : %.cpp
	$(COMPILER) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(COMPILER) $^ $(INCLUDES) $(LDFLAGS)  -o $@

clean:
	rm -f $(OBJ) $(EXE)

