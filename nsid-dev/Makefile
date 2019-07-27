CC		:= gcc
CXX		:= -Wall

SRC		:= nsid.c main.c
EXE		:= nsid
DBG		:= nsid-dbg


$(EXE):
	$(CC) $(CXX) $(SRC) -o $@

clean:
	$(RM) $(nsid-dbg)
	$(RM) $(EXE)

debug:
	$(CC) $(CXX) $(SRC) -g -o $(DBG)

all: clean $(EXE)
	mv $(EXE) $(HOME)/bin
