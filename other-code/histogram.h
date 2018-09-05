// header file for class WordUnit and class WordHistogram
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

// declare class WordUnit
class WordUnit
{
// declare private data members of WordUnit
private:
	string word;
	int num;

// declare member function prototypes
public:
	// default constructor
	WordUnit();
	
	//setters
	void setWord(string);
	
	void setNum();

	void addNum(int);

	//getters
	string getWord();

	int getNum();

	// print for testing purposes
	void print();

}; // end class WordUnit


 // declare class WordHistogram
class WordHistogram
{
// declare private data members of WordHistogram
private:
	WordUnit wordArray[10000];
	int counter;
	string fileName;

//declare public member functions
public:
	// defaulty constructor
	WordHistogram(string);

	void makeHistogram();

	void sortAlphaHistogram();

	void sortFreqHistogram();

	void exportHistogram(string);

}; // end class WordHistogram

// end of header file
