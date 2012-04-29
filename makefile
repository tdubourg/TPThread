COMP=g++
EXE=tpthreads
EFFACE=clean
OBJ=lib.o arbre.o main.o
LIBS= -lpthread
CUSTOM_LIBS=
ECHO=echo
RM=rm
RMFLAGS=-f
CFLAGS=
LNKFLAGS=

%.o: %.cpp
#	$(ECHO) compilation de $<
	$(COMP) $(CFLAGS)-c $< -o $@

$(EXE): $(OBJ)
	$(ECHO) $(EXE)
	$(COMP) $(LNKFLAGS) -o"$(EXE)" $(OBJ) $(CUSTOM_LIBS) $(LIBS)

$(EFFACE):
	$(RM) $(RMFLAGS) $(EXE) $(OBJ)
