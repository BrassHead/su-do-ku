// Playing around with SU | DO | KU
//
// Just to see what logic is needed to solve
//
/*

Copyright 2005 Brian Bray

See attached MIT Licence.

*/

#include <iostream>

using std::istream;
using std::ostream;
using std::ios;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;


class Board {
private:
	int	known[9][9];
	int	deductions[9][9];

	void clear(void);
	void cleardeduct(void);

	int rowHasA(int row, int num) {
		int c=0;
		for (int col=0; col<9; col++)
			if (known[row][col]==num)
				c++;
		return c;
	}

	int colHasA(int col, int num) {
		int c=0;
		for (int row=0; row<9; row++)
			if (known[row][col]==num)
				c++;
		return c;
	}

	int blkHasA(int blk, int num) {
		int srow = (blk/3) * 3;
		int scol = (blk%3) * 3;
		int c=0;

		for (int row=srow; row<srow+3; row++)
			for (int col=scol; col<scol+3; col++)
				if (known[row][col]==num)
					c++;
		return c;
	}

	int inBlk(int row,int col) {
		return (row/3)*3 + (col/3);
	}

	bool canBe(int row, int col, int num) {
		return rowHasA(row,num)==0 && colHasA(col,num)==0 && blkHasA(inBlk(row,col),num)==0;
	}

	void writemat(const ostream& ios, const int mat[9][9]);

public:

	Board(void);
	bool isValid(void);
	bool isComplete(void);
	void read(istream& ios);
	void write(const ostream& ios);
	void writededuct(const ostream& ios);
	void deduce(void);
	bool haveDeduct(void);
	void mergeDeduct(void);

};

Board theBoard;

Board::Board(void)
{
	clear();
}


void Board::clear(void)
{
	cleardeduct();
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			known[i][j] = 0;
}

void Board::cleardeduct(void)
{
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			deductions[i][j] = 0;
}

bool Board::isValid(void)
{
	// Validity means that the rules are not violated by the current state of
	// the board

	// Each row has zero or one occurance of each digit

	bool val = true;

	for (int num=1; num<10; num++) {
		for (int idx=0; idx<9; idx++)
			if ( rowHasA(idx,num)>1 || colHasA(idx,num)>1 || blkHasA(idx,num)>1 ) {
				val = false;
				break;
			}
	}
	return val;
}

bool Board::isComplete(void)
{
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			if (known[i][j] == 0)
				return false;
	return true;
}


void Board::read(istream& ios)
{
	char ch;
	clear();

	for (int i=0; i<9; i++) {
		ch = '\n';
		for (int j=0; j<9; j++) {
			if (ios.get(ch) && ch!='\n' ) {
				if (ch>'0' && ch<='9')
					known[i][j] = ch - '0';
			} else
				break;
		}
		while (ch!='\n' && ios.get(ch))
			;
		ch = '\n';
	}
}

void Board::write(const ostream& ios)
{
	writemat(ios,known);
}

void Board::writededuct(const ostream& ios)
{
	writemat(ios,deductions);
}

void Board::writemat(const ostream& ios, const int mat[9][9])
{
	for (int i=0; i<9; i++) {
		if (i%3 == 0)
			cout << "+---+---+---+" << endl;

		for (int j=0; j<9; j++) {
			if (j%3 == 0)
				cout << "|";
			if (mat[i][j]>0)
				cout << mat[i][j];
			else
				cout << ' ';
		}

		cout << "|" << endl;
	}
	cout << "+---+---+---+" << endl;
}

void Board::deduce(void)
{
	int lastpos;
	int numpos;
	int r, c, lastrow, lastcol;

	cleardeduct();

	for (int idx=0; idx<9; idx++) {
		for (int num=1; num<10; num++) {
			if (rowHasA(idx,num)==0) {
				// Looking to see if row idx has just one possible place for the number num
				numpos = 0;
				for (int pos=0; pos<9; pos++) {
					if (known[idx][pos]==0 && canBe(idx,pos,num)) {
						lastpos = pos;
						++numpos;
					}
				}
				if (numpos==0) {
					cout << "Impossible to place " << num << " in row " << idx << endl;
				} else if (numpos==1) {
					deductions[idx][lastpos] = num;
				}
			}

			if (colHasA(idx,num)==0) {
				// Looking to see if col idx has just one possible place for the number num
				numpos = 0;
				for (int pos=0; pos<9; pos++) {
					if (known[pos][idx]==0 && canBe(pos,idx,num)) {
						lastpos = pos;
						++numpos;
					}
				}
				if (numpos==0) {
					cout << "Impossible to place " << num << " in column " << idx << endl;
				} else if (numpos==1) {
					deductions[lastpos][idx] = num;
				}
			}

			if (blkHasA(idx,num)==0) {
				// Looking to see if block idx has just one possible place for the number num
				numpos = 0;
				for (int pos=0; pos<9; pos++) {
					r = (idx/3)*3 + pos/3;
					c = (idx%3)*3 + pos%3;
					if (known[r][c]==0 && canBe(r,c,num)) {
						lastrow = r;
						lastcol = c;
						++numpos;
					}
				}
				if (numpos==0) {
					cout << "Impossible to place " << num << " in block " << idx << endl;
				} else if (numpos==1) {
					deductions[lastrow][lastcol] = num;
				}
			}
		}
	}
}

bool Board::haveDeduct(void)
{
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			if (deductions[i][j]>0)
				return true;
	return false;
}

void Board::mergeDeduct(void)
{
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			if (deductions[i][j]>0)
				known[i][j] = deductions[i][j];
	if (!isValid())
		cout << "Error, invalid game after deductions." << endl;

}



int main( int argc, char** argv )
{
	cout << "Reading board." << endl;
	theBoard.read(cin);
	cout << "Board is:" << endl;
	theBoard.write(cout);

	if (theBoard.isValid())
		cout << "Board is valid" << endl;
	else
		cout << "Board is invalid" << endl;

	theBoard.deduce();

	while (theBoard.haveDeduct()) {
		cout << "\nDeduced:" << endl;
		theBoard.writededuct(cout);
		theBoard.mergeDeduct();
		theBoard.deduce();
	}

	if (theBoard.isComplete())
		cout << "Board is complete" << endl;
	else
		cout << "Board is incomplete ==> Board is ambiguous???" << endl;

	cout << "Final board is:" << endl;
	theBoard.write(cout);

	return 0;
}

