#CXX=g++
#CXX=FCC
#CXX=mpiFCC
CXX=icpc
#MPICXX=mpicxx
#MPICXX=mpiFCC
MPICXX=mpiicpc
#MPICXX="mpiFCC -Kfast"
#CXXFLAGS= -O2 -std=c++11 -Wall -Kfast
CXXFLAGS= -O2 -std=c++11 -Wall
#CXXFLAGS= -Kfast

#DISABLE_REUSE?=no
DISABLE_REUSE?=yes #REUSE off
ifeq ($(DISABLE_REUSE),no)
CXXFLAGS += -DPARTICLE_SIMULATOR_REUSE_LIST
endif

#PS_PATH?=$(HOME)/Documents/code/2020/nomuraFDPS/FDPS-master/src
PS_PATH?=../../../src
FDPSFLAGS= -I$(PS_PATH)

#下の記述逆なきがする岡田2020/05/13
#MPIFLAGS= -fopenmp -DPARTICLE_SIMULATOR_THREAD_PARALLEL 
#OMPFLAGS= -DPARTICLE_SIMULATOR_MPI_PARALLEL

#再記述2020/05/13岡田
MPIFLAGS= -DPARTICLE_SIMULATOR_MPI_PARALLEL
#OMPFLAGS= -qopenmp -DPARTICLE_SIMULATOR_THREAD_PARALLEL -Xg
OMPFLAGS= -qopenmp -DPARTICLE_SIMULATOR_THREAD_PARALLEL
#OMPFLAGS= module load intel/2019.4 -qopenmp -DPARTICLE_SIMULATOR_THREAD_PARALLEL 
#OMPFLAGS= -qopenmp -DPARTICLE_SIMULATOR_THREAD_PARALLEL -l/home/app/intel/intel2019_up4/compilers_and_libraries_2019.4.243/linux/compiler/lib/intel64_lin
#OMPFLAGS= -qopenmp -DPARTICLE_SIMULATOR_THREAD_PARALLEL -static-intel



SRC=main.cpp
HED=user_defined_class.hpp

all:	$(SRC) $(HED)
	$(CXX) $(CXXFLAGS) $(FDPSFLAGS) -o serial.out $(SRC)
mpi:	$(SRC) $(HED)
	$(MPICXX) $(CXXFLAGS) $(FDPSFLAGS) $(MPIFLAGS) -o mpi.out $(SRC)
omp:	$(SRC) $(HED)
	$(CXX) $(CXXFLAGS) $(FDPSFLAGS) $(OMPFLAGS) -o omp.out $(SRC)
mpiomp:	$(SRC) $(HED)
	$(MPICXX) $(CXXFLAGS) $(FDPSFLAGS) $(MPIFLAGS) $(OMPFLAGS) -o mpiomp.out $(SRC)
ompmpi:	mpiomp

include Makefile.pezy
