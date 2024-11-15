BIN_DIR=./bin
PROGRAM=$(BIN_DIR)/sudoku

CC=g++
CFLAGS=-c -O3 -fopenmp
LDFLAGS=-fopenmp

OBJS=SudokuBoard.o sudoku.o

all: $(PROGRAM) 

$(PROGRAM): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ $^ 

$(BIN_DIR): 
	mkdir $@

clean:
	rm -rf $(PROGRAM) *.o

wipe: clean
	rm -rf *.out *.err
