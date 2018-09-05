// histogram.cpp - implementation of classes WordUnit and WordHistogram

#include "histogram.h"

using namespace std;

// implement functions for class WordUnit

// default constructor - sets blank word
WordUnit::WordUnit() : word(""), num(0) {  }

void WordUnit::setWord(string w) {  word = w;  }

void WordUnit::setNum() {  num++;  }

void WordUnit::addNum(int n) {  num = n;  }

string WordUnit::getWord() {  return word;  }

int WordUnit::getNum() {  return num;  }

void WordUnit::print() {  cout << word << num << endl;  }


// implement functions for class WordHistogram

// default constructor - sets filename
WordHistogram::WordHistogram(string fn) : fileName(fn) { counter = 0; }

// creates histogram
void WordHistogram::makeHistogram()
{
	// open filestream
	ifstream oldText(fileName.c_str(), ios::in);

	// string to store words from file
	string fileWord;

	int i = 0;

	// while loop runs until return value !=1
	while (oldText >> fileWord)
	{
		// for loop to record word occurence
		for ((i = 0); i < 10000; i++)
		{
			// add new word to array
			if (wordArray[i].getWord() == "")
			{
				wordArray[i].setWord(fileWord);
				wordArray[i].setNum();
				counter++;
				break;
			}
			// increment incidence of repeated word
			else if (wordArray[i].getWord() == fileWord)
			{
				wordArray[i].setNum();
				break;
			}
		}
	}
	// close file
	oldText.close();
} // end member function



void WordHistogram::sortAlphaHistogram()
{
	// insertion sort function to sort array alphabetically	
		
	int i, j, n;
	string alpha;

	// for loop to sort array
	for (i = 1; i < counter; i++)
	{
		alpha = wordArray[i].getWord();
		n = wordArray[i].getNum();
		j = i - 1; 

		// while loop runs until selected word is in alphabetical order
		while (j >= 0 & wordArray[j].getWord() > alpha)
		{
			wordArray[j + 1] = wordArray[j];
			j--;
		}

		wordArray[j + 1].setWord(alpha);
		wordArray[j + 1].addNum(n);
	}
} // end member function



void WordHistogram::sortFreqHistogram()
{
	// insertion sort function to sort array by frequency

	int i, j, freq;
	string w;

	// for loop to sort array
	for (i = 1; i < counter; i++)
	{
		w = wordArray[i].getWord();
		freq = wordArray[i].getNum();
		j = i - 1;

		// while loop runs until selected word has highest frequency
		while (j >= 0 & wordArray[j].getNum() < freq)
		{
			wordArray[j + 1] = wordArray[j];
			j--;
		}

		wordArray[j + 1].setWord(w);
		wordArray[j + 1].addNum(freq);

	}
} // end member function



void WordHistogram::exportHistogram(string fileText)
{	
	fileName = fileText;

	ofstream newText(fileName.c_str(), ios::out);

	int i = 0, n;
	string w;

	// print to output file
	for ((i = 0); i < counter; i++)
	{
		w = wordArray[i].getWord();
		n = wordArray[i].getNum();

		newText << w << " " << n << endl;
	}

	// close output file
	newText.close();
} // end member function


// end of file histogram.cpp