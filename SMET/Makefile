SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /opt/ibm/ILOG/CPLEX_Studio201/cplex
CONCERTDIR    = /opt/ibm/ILOG/CPLEX_Studio201/concert

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

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# For dynamic linking
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(SYSTEM)
CPLEXLIB      = cplex$(dynamic:yes=2010)
run           = $(dynamic:yes=LD_LIBRARY_PATH=$(CPLEXBINDIR))

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) $(dynamic:yes=-L$(CPLEXBINDIR))
CLNDIRS   = -L$(CPLEXLIBDIR) $(dynamic:yes=-L$(CPLEXBINDIR))
CCLNFLAGS = -lconcert -lilocplex -l$(CPLEXLIB) -lm -lpthread -ldl
CLNFLAGS  = -l$(CPLEXLIB) -lm -lpthread -ldl


all:
	make all_cpp

execute: all
	make execute_cpp

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

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

execute_cpp: $(CPP_EX)
	$(run) ./blend
	$(run) ./cutstock
	$(run) ./etsp
	$(run) ./facility
	$(run) ./fixcost1
	$(run) ./foodmanufact
	$(run) ./iloadmipex1 $(EXDATA)/mexample.mps
	$(run) ./iloadmipex2 $(EXDATA)/p0033.mps
	$(run) ./iloadmipex3 $(EXDATA)/sosex3.lp
	$(run) ./iloadmipex4
	$(run) ./iloadmipex5
	$(run) ./iloadmipex6 $(EXDATA)/mexample.mps
	$(run) ./iloadmipex8
	$(run) ./iloadmipex9 $(EXDATA)/p0033.mps
	$(run) ./iloconflictex1 $(EXDATA)/infeasible.lp
	$(run) ./ilodiet
	$(run) ./ilofixnet
	$(run) ./ilogoalex1 $(EXDATA)/mexample.mps
	$(run) ./ilogoalex2
	$(run) ./ilogoalex3 $(EXDATA)/mexample.mps
	$(run) ./iloindefqpex1
	$(run) ./iloglobalqpex1 $(EXDATA)/nonconvexqp.lp g
	$(run) ./iloglobalqpex1 $(EXDATA)/nonconvexmiqp.lp g
	$(run) ./ilolpex1 -r
	$(run) ./ilolpex2 $(EXDATA)/example.mps p
	$(run) ./ilolpex3
	$(run) ./ilolpex4
	$(run) ./ilolpex6
	$(run) ./ilolpex7 $(EXDATA)/afiro.mps p
	$(run) ./ilomipex1
	$(run) ./ilomipex2 $(EXDATA)/mexample.mps
	$(run) ./ilomipex3
	$(run) ./ilomipex4 $(EXDATA)/p0033.mps l
	$(run) ./ilomiqpex1
	$(run) ./ilomultiobjex1 $(EXDATA)/multiobj.lp
	$(run) ./ilopopulate $(EXDATA)/location.lp
	$(run) ./iloqcpex1
	$(run) ./iloqpex1
	$(run) ./iloqpex2 $(EXDATA)/qpex.lp o
	$(run) ./iloqpex3
	$(run) ./ilotuneset $(EXDATA)/p0033.mps
	$(run) ./ilobenders $(EXDATA)/UFL_25_35_1.mps
	$(run) ./ilobendersatsp 1 $(EXDATA)/atsp.dat
	$(run) ./ilobendersatsp2 1 $(EXDATA)/atsp.dat
	$(run) ./ilosocpex1
	$(run) ./iloqcpdual
	$(run) ./inout1
	$(run) ./inout3
	$(run) ./mixblend
	$(run) ./rates
	$(run) ./ilosteel
	$(run) ./ilotransport 1
	$(run) ./warehouse
	$(run) ./ilogenericbranch $(EXDATA)/noswot.mps

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
MAIN: MAIN.o smet_namespace.o constructive.o improvement.o smptsp_sub.o smptsp_impr.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o MAIN MAIN.o smet_namespace.o constructive.o improvement.o smptsp_sub.o smptsp_impr.o $(CCLNFLAGS)

MAIN.o: main.cpp
	$(CCC) -c $(CCFLAGS) main.cpp -o MAIN.o

smet_namespace.o: NAMESPACE/smet_namespace.cpp
	$(CCC) -c $(CCFLAGS) NAMESPACE/smet_namespace.cpp -o smet_namespace.o

constructive.o: CONSTRUCTIVE/constructive.cpp
	$(CCC) -c $(CCFLAGS) CONSTRUCTIVE/constructive.cpp -o constructive.o

smptsp_sub.o: SMPTSP_SUB/smptsp_sub.cpp
	$(CCC) -c $(CCFLAGS) SMPTSP_SUB/smptsp_sub.cpp -o smptsp_sub.o

smptsp_impr.o: SMPTSP_IMPR/smptsp_impr.cpp
	$(CCC) -c $(CCFLAGS) SMPTSP_IMPR/smptsp_impr.cpp -o smptsp_impr.o

improvement.o: IMPROVEMENT/improvement.cpp
	$(CCC) -c $(CCFLAGS) IMPROVEMENT/improvement.cpp -o improvement.o

# Local Variables:
# mode: makefile
# End:
