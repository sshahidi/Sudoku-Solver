#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <exception>
#include <string.h>

#define NDEBUG //comment out to disable asserts
#include <assert.h>

using namespace std;

/**
* Sudoku class. loads,solved,and prints a soduko with different sizes of 4,9,16,25,...
* last edited 2014-11-01
* @author Shervin Shahidi, sh.shervin@gmail.com
*/
class Sudoku
{
private:
	const int TAB_SIZE;
	int **table;
public:

	/**
	* The number base can be any integer^2, i.e., 4,9(default),16,25,... for 25 it takes a few hours though!
	*/
	Sudoku(int tab_size=9):TAB_SIZE(tab_size)
	{
		if((int)sqrt(TAB_SIZE) != sqrt(TAB_SIZE))
		{
			cerr<<"ERROR: the size of the table should be the complete square of an integer.\n Quitting..."<<endl;
			exit(EXIT_FAILURE);
		}
		table = new int*[TAB_SIZE];
		for(int i=0;i<TAB_SIZE;i++)
			table[i]=new int[TAB_SIZE];
	}
	~Sudoku()
	{
		for(int i=0;i<TAB_SIZE;i++)
			delete[] table[i];
		delete[] table;
	}

	/**
	* Loads a csv file, as an input to an unsolved soduko game
	*/
	void loadCSV(string path);
	/**
	* Solves the soduko.
	* The recursive approach is used. For more optimality it can be easily converted to iterative using a std::stack.
	* However, the iterative code will be harder to understand. So I left it this way.
	*/
	bool solvePuzzle();
	/**
	* prints the Soduko (either solved or not solved) to the given CSV file path.
	*/
	void printCSV(string out_path);
};



int main(int argc,char* argv[])
{
	//reading the file path
	string in_path,out_path;
	if(argc <= 1)						//no input arguments
	{
		cout<<"Enter the csv file path: ";
		cout.flush();
		cin>>in_path;
		out_path="sudoku.out";			//setting a default output name.
	}
	else if(argc ==2)					//only input path is given
	{
		in_path.assign(argv[1]);
		out_path="sudoku.out";			//setting a default output name.
	}
	else if (argc == 3)					//both input path and output path is given
	{
		in_path.assign(argv[1]);
		out_path.assign(argv[2]);
	}
	else								//too many arguments are given as input
	{
		cerr<<"ERROR: too many arguments"<<endl;
		exit(EXIT_FAILURE);
	}

	//defining a sudoku with proper number base. (default is base 10, which meanss 9x9 tables)
	Sudoku my_sudoku(25);

	try
	{
		//loading the csv file
		my_sudoku.loadCSV(in_path);
		//solving the puzzle
		if(my_sudoku.solvePuzzle())
		{
			//printing the output in a CSV file
			my_sudoku.printCSV(out_path);
			cout<<"done!"<<endl;
		}
		else
			cout<<"No Answer was found. The given sudoku does not have an answer"<<endl;
	}
	catch(exception e)
	{
		cerr<<e.what()<<endl;
		exit(EXIT_FAILURE);
	}
	

	return 0;
}


bool Sudoku::solvePuzzle()
{
	bool found=false;

	//finding the first unkonwn cell
	int row,column;
	for(row=0;row<TAB_SIZE;row++)
	{
		for(column=0;column<TAB_SIZE;column++)
			if(table[row][column]==0)
				break;
		if(column<TAB_SIZE)
			break;
	}

	if(row==TAB_SIZE) //no unkonwn exists the answer is found
		return true;

	//finding the possible options for the unkonwn cell
	int* hash=new int[TAB_SIZE+1];
	memset(hash,0,(TAB_SIZE+1)*sizeof(int));
	//noting down the numbers not allowed for the cell
	//numbers taken on the row:
	for(int k=0;k<TAB_SIZE;k++)
		if(table[row][k]!=0)
			hash[table[row][k]]=1;
	//numbers taken on the column
	for(int k=0;k<TAB_SIZE;k++)
		if(table[k][column]!=0)
			hash[table[k][column]]=1;
	//numbers taken in the small surronding square (assuming it is always a 3x3 square, for base 10 numbers)
	int limit= (int)sqrt(TAB_SIZE); //size of the small squares.
	for(int k=0;k<limit;k++)
		for(int m=0;m<limit;m++)
			if(table[row- (row%limit)+k][column- (column%limit)+m]!=0)
				hash[table[row- (row%limit)+k][column- (column%limit)+m]]=1;

	//solving for the remaining options
	for(int k=1;k<TAB_SIZE+1;k++)
	{
		if(hash[k]==0)
		{
			table[row][column]=k;
			//updating the options for other cells:
			found = solvePuzzle();
			if (found)
				break;
		}
	}

	delete[] hash; //releasing the memory
	if(found)
		return true;
	else //no options remianed and the puzzle is not solved
	{
		table[row][column]=0; //undoing the changes on table
		return false; 
	}

}

void Sudoku::loadCSV(string path)
{
	char delimiters[]=" ,;\t\r\n";
	ifstream fin(path);
	if(fin==NULL || fin.bad())
	{
		cerr<<"ERROR: could not open the input file.\n Quitting..."<<endl;
		exit(EXIT_FAILURE);
	}
	int i=0;
	while(!fin.eof())
	{
		
		//reading a line
		string line;
		getline(fin,line);
		if(line.length()==0)
			continue;

		//tokenizing
		int j=0;
		stringstream sin(line);
		while(!sin.eof())
		{
			char c=sin.peek(); //skipping the delimiters
			if(strchr(delimiters,c)!=NULL)
			{
				sin>>c;
				continue;
			}
			sin>>table[i][j];
			j++;
			assert(j<=TAB_SIZE && "Allocateed table size is not enough for the given input"); //for debugging
			if(j>TAB_SIZE)
				throw out_of_range("Allocateed table size is not enough for the given input");
		}
		i++;
		assert(i<=TAB_SIZE && "Allocateed table size is not enough for the given input"); //for debugging
		if(j>TAB_SIZE)
				throw out_of_range("Allocateed table size is not enough for the given input");
	}

	fin.close();
}

void Sudoku::printCSV(string out_path)
{
	ofstream fout(out_path);
	for(int i=0;i<TAB_SIZE;i++)
	{
		for(int j=0;j<TAB_SIZE-1;j++)
		{
			fout<<table[i][j]<<",";
		}
		fout<<table[i][TAB_SIZE-1]<<endl;
	}
	fout.flush();
	fout.close();
}
