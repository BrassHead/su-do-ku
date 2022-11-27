# Makefile !
#

CFLAGS=-g
CPP=c++

sudoku: sudoku.o
	$(CPP) $(CFLAGS) sudoku.o -o sudoku

sudoku.o: sudoku.cpp
	$(CPP) $(CFLAGS) -c sudoku.cpp

clean:
	rm -f *~ core *.o
	rm -f sudoku


