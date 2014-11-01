#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <string.h>
#include <assert.h>
using namespace std;

#define NUM_BASE 10
#define TAB_SIZE (NUM_BASE-1)

//#define NDEBUG //comment out to disable asserts


void loadCSV(string path,int[TAB_SIZE][TAB_SIZE]);
bool solvePuzzle(int table[TAB_SIZE][TAB_SIZE]);
//void calcOptions(int table[TAB_SIZE][TAB_SIZE],list<int> options[TAB_SIZE][TAB_SIZE]);
void printCSV(string out_path,int table[TAB_SIZE][TAB_SIZE]);

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
		cout<<"ERROR: too many arguments"<<endl;
		exit(EXIT_FAILURE);
	}


	//loading the csv file
	int table[TAB_SIZE][TAB_SIZE];		//the sudoku table is 9x9.
	loadCSV(in_path,table);

	//solving the puzzle
	//list<int> options[TAB_SIZE][TAB_SIZE];
	//calcOptions(table,options);
	solvePuzzle(table);

	//printing the output in a CSV file
	printCSV(out_path,table);

	return 0;
}


bool solvePuzzle(int table[TAB_SIZE][TAB_SIZE])
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
	int hash[TAB_SIZE+1];
	memset(hash,0,sizeof(hash));
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
			found = solvePuzzle(table);
			if (found)
				break;
		}
	}

	if(found)
		return true;
	else //no options remianed and the puzzle is not solved
	{
		table[row][column]=0; //undoing the changes on table
		return false; 
	}

}

void loadCSV(string path,int table[TAB_SIZE][TAB_SIZE])
{
	char delimiters[]=" ,;\t\r\n";
	ifstream fin(path);
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
			assert(j<=9 && "Allocateed table size is not enough for the given input");
		}
		i++;
		assert(i<=9 && "Allocateed table size is not enough for the given input");
	}

	fin.close();
}

void printCSV(string out_path,int table[TAB_SIZE][TAB_SIZE])
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

/*
void calcOptions(int table[TAB_SIZE][TAB_SIZE],list<int> options[TAB_SIZE][TAB_SIZE])
{
	for(int i=0;i<TAB_SIZE;i++)
	{
		for(int j=0;j<TAB_SIZE;j++)
		{
			if(table[i][j]!=0)
				continue;
			int hash[TAB_SIZE+1];
			memset(hash,0,sizeof(hash));
			//noting down the numbers not allowed for each cell in the table
			//numbers taken on the row:
			for(int k=0;k<TAB_SIZE;k++)
				if(table[i][k]!=0)
					hash[table[i][k]]=1;
			//numbers taken on the column
			for(int k=0;k<TAB_SIZE;k++)
				if(table[k][j]!=0)
					hash[table[k][j]]=1;
			//numbers taken in the small surronding square (assuming it is always a 3x3 square, for base 10 numbers)
			int limit= (int)sqrt(TAB_SIZE); //size of the small squares.
			for(int k=0;k<limit;k++)
				for(int m=0;m<limit;k++)
					if(table[i- (i%limit)+k][j- (j*limit)+m]!=0)
						hash[table[i- (i%limit)+k][j- (j*limit)+m]]=1;

			//adding the reamining options to the list
			for(int k=1;k<TAB_SIZE+1;k++)
				if(hash[k]==0)
					options[i][j].push_back(k);
		}
	}
}*/
