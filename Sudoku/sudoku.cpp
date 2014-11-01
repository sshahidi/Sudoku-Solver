#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <assert.h>
using namespace std;

#define TAB_SIZE (9)
//#define NDEBUG //comment out to disable asserts


void loadCSV(string path,int[TAB_SIZE][TAB_SIZE]);
bool solvePuzzle(int table[TAB_SIZE][TAB_SIZE]);
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
	//solvePuzzle(table);

	//printing the output in a CSV file
	printCSV(out_path,table);

	return 0;
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