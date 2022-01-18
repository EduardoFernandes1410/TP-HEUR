SYSTEM     = x86-64_linux
# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++ -std=c++17

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG
COPT  = -m64 -fPIC
JOPT  = -classpath $(CPLEXDIR)/lib/cplex.jar -O

all:
	make all_cpp

execute: all
	make execute_cpp

EXDIR         = $(CPLEXDIR)/examples
EXINC         = $(EXDIR)/include
EXDATA        = $(EXDIR)/data
EXSRCCPP      = $(EXDIR)/src/cpp

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 

#------------------------------------------------------------
#  make all      : to compile the examples. 
#  make execute  : to compile and execute the examples.
#------------------------------------------------------------

CPP_EX = MAIN

all_cpp: $(CPP_EX)

# ------------------------------------------------------------

clean :
	/bin/rm -rf *.o *~ *.class
# 	/bin/rm -rf $(CPP_EX)
	/bin/rm -rf *.mps *.ord *.sos *.lp *.sav *.net *.msg *.log *.clp

# ------------------------------------------------------------
#
# The examples
#

# The DMLCPR program itself
MAIN: MAIN.o aco_namespace.o 
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o MAIN MAIN.o aco_namespace.o 

MAIN.o: main.cpp
	$(CCC) -c $(CCFLAGS) main.cpp -o MAIN.o

aco_namespace.o: NAMESPACE/aco_namespace.cpp
	$(CCC) -c $(CCFLAGS) NAMESPACE/aco_namespace.cpp -o aco_namespace.o

# Local Variables:
# mode: makefile
# End:
