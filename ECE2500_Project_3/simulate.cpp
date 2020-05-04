#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <iomanip>
#include <algorithm>


using namespace std;

typedef enum {WB, WT} WriteState;



struct Set {
	int numberOfElements;
	list<tuple<int, bool>> dataList;
};


void cache_DM(const int blockSize, const int cacheSize, WriteState write, ofstream &outputFile);
void cache_FA(const int blockSize, const int cacheSize, WriteState write, ofstream& outputFile);
void cache_SA(const int blockSize, const int cacheSize, const int howManyWay, WriteState write, ofstream& outputFile);
int binary_to_decimal(string binary);
string hex_to_binary(string hex);

int main()
{
	/*
	int howManyWay = 2;
	cache_2W(8, 1024, howManyWay, WB);
	cache_2W(8, 1024, howManyWay, WT);

	
	cache_2W(16, 1024, howManyWay, WB);
	cache_2W(16, 1024, howManyWay, WT);
	cache_2W(32, 1024, howManyWay, WB);
	cache_2W(32, 1024, howManyWay, WT);
	cache_2W(128, 1024, howManyWay, WB);
	cache_2W(128, 1024, howManyWay, WT);
	
	cout << endl;
	
	cache_2W(8, 4096, howManyWay, WB);
	cache_2W(8, 4096, howManyWay, WT);
	cache_2W(16, 4096, howManyWay, WB);
	cache_2W(16, 4096, howManyWay, WT);
	cache_2W(32, 4096, howManyWay, WB);
	cache_2W(32, 4096, howManyWay, WT);
	cache_2W(128, 4096, howManyWay, WB);
	cache_2W(128, 4096, howManyWay, WT);

	cout << endl;

	cache_2W(8, 65536, howManyWay, WB);
	cache_2W(8, 65536, howManyWay, WT);
	cache_2W(16, 65536, howManyWay, WB);
	cache_2W(16, 65536, howManyWay, WT);
	cache_2W(32, 65536, howManyWay, WB);
	cache_2W(32, 65536, howManyWay, WT);
	cache_2W(128, 65536, howManyWay, WB);
	cache_2W(128, 65536, howManyWay, WT);

	cout << endl;

	cache_2W(8, 131072, howManyWay, WB);
	cache_2W(8, 131072, howManyWay, WT);
	cache_2W(16, 131072, howManyWay, WB);
	cache_2W(16, 131072, howManyWay, WT);
	cache_2W(32, 131072, howManyWay, WB);
	cache_2W(32, 131072, howManyWay, WT);
	cache_2W(128, 131072, howManyWay, WB);
	cache_2W(128, 131072, howManyWay, WT);
	*/
	
	WriteState write = WB;

	ofstream outputFile;
	outputFile.open("test1.result");


	for (int index2 = 1024; index2 <= 131072; )
	{
		for (int index = 8; index <= 128;)
		{
			write = WB;
			cache_DM(index, index2, write, outputFile);
			write = WT;
			cache_DM(index, index2, write, outputFile);

			write = WB;
			cache_SA(index, index2, 2, write, outputFile);
			write = WT;
			cache_SA(index, index2, 2, write, outputFile);

			write = WB;
			cache_SA(index, index2, 4, write, outputFile);
			write = WT;
			cache_SA(index, index2, 4, write, outputFile);

			write = WB;
			cache_FA(index, index2, write, outputFile);
			write = WT;
			cache_FA(index, index2, write, outputFile);

			if (index != 32)
			{
				index *= 2;
			}
			else
			{
				index *= 4;
			}
			
			
		}

		if (index2 == 1024)
		{
			index2 = 4096;
		}
		else if (index2 == 4096)
		{
			index2 = 65536;
		}
		else if (index2 == 65536)
		{
			index2 = 131072;
		}
		else
		{
			index2 *= 2;
		}
	}

	/*
	cache_FA(8, 1024, WT);

	cache_FA(16, 1024, WB);
	cache_FA(16, 1024, WT);

	cache_FA(32, 1024, WB);
	cache_FA(32, 1024, WT);

	cache_FA(128, 1024, WB);
	cache_FA(128, 1024, WT);

	cout << endl;

	cache_FA(8, 4096, WB);
	cache_FA(8, 4096, WT);

	cache_FA(16, 4096, WB);
	cache_FA(16, 4096, WT);

	cache_FA(32, 4096, WB);
	cache_FA(32, 4096, WT);

	cache_FA(128, 4096, WB);
	cache_FA(128, 4096, WT);

	cout << endl;

	cache_FA(8, 65536, WB);
	cache_FA(8, 65536, WT);

	cache_FA(16, 65536, WB);
	cache_FA(16, 65536, WT);

	cache_FA(32, 65536, WB);
	cache_FA(32, 65536, WT);

	cache_FA(128, 65536, WB);
	cache_FA(128, 65536, WT);

	cout << endl;

	cache_FA(8, 131072, WB);
	cache_FA(8, 131072, WT);

	cache_FA(16, 131072, WB);
	cache_FA(16, 131072, WT);

	cache_FA(32, 131072, WB);
	cache_FA(32, 131072, WT);

	cache_FA(128, 131072, WB);
	cache_FA(128, 131072, WT);

	cout << endl;
	*/


	return EXIT_SUCCESS;
}



void cache_DM(const int blockSize, const int cacheSize, WriteState write, ofstream& outputFile)
{
	ifstream inputFile;
	inputFile.open("test1.trace");

	string nextReading = "";
	string tempIndex = "";
	string tempTag = "";

	const int BLOCK_SIZE = (int)blockSize;
	const int OFFSET_SIZE = (int)log2(blockSize);
	const int INDEX_SIZE = (int)log2(cacheSize / blockSize);
	const int STARTING_INDEX = 2;
	const int TAG_SIZE = 32 - OFFSET_SIZE - INDEX_SIZE;

	const int TAG_LOCATION = 0;
	const int INDEX_LOCATION = TAG_SIZE;
	const int OFFSET_LOCATION = TAG_SIZE + INDEX_SIZE;
	const int WORD_SIZE = 4;

	string* myArray = new string[INDEX_SIZE];
	
	int numberOfHits = 0;
	int numberOfMisses = 0;
	int memToCache = 0;
	int cacheToMem = 0;
	bool dirtyBit = false;

	// First Reading Starts Here
	inputFile >> nextReading;

	if (nextReading == "write" && write == WT)
	{
		cacheToMem += WORD_SIZE;
	}

	//cout << nextReading << endl;

	inputFile >> nextReading;

	//cout << "Hex:" << nextReading << endl;

	string hex = nextReading.substr(STARTING_INDEX, BLOCK_SIZE);
	string binary = hex_to_binary(hex);

	//cout << "Binary:" << binary << endl;

	string tag = binary.substr(TAG_LOCATION, TAG_SIZE);
	string index = binary.substr(INDEX_LOCATION, INDEX_SIZE);
	string offset = binary.substr(OFFSET_LOCATION, OFFSET_SIZE);

	int blockIndex = binary_to_decimal(index);

	myArray[blockIndex] = tag;

	tempTag = tag;
	tempIndex = index;

	numberOfMisses++;

	//cout << "Number of Hits: " << numberOfHits << endl;
	//cout << "Number of Misses: " << numberOfMisses << endl;

	//cout << "Tag: " << tag << endl;
	//cout << "Index: " << index << endl;
	//cout << "Offset: " << offset << endl << endl;

	while (inputFile >> nextReading)
	{
		if (nextReading == "write")
		{
			if (write == WT)
			{
				cacheToMem += WORD_SIZE;
			}
			//cout << nextReading << endl;
			inputFile >> nextReading;

			//cout << "Hex:" << nextReading << endl;

			hex = nextReading.substr(STARTING_INDEX, BLOCK_SIZE);
			binary = hex_to_binary(hex);

			//cout << "Binary:" << binary << endl;

			tag = binary.substr(TAG_LOCATION, TAG_SIZE);
			index = binary.substr(INDEX_LOCATION, INDEX_SIZE);
			offset = binary.substr(OFFSET_LOCATION, OFFSET_SIZE);

			blockIndex = binary_to_decimal(index);


			if (myArray[blockIndex] == tag)
			{
				numberOfHits++;
			}
			else
			{
				numberOfMisses++;
				if (dirtyBit && write == WB)
				{
					cacheToMem = cacheToMem + BLOCK_SIZE;
					dirtyBit = false;
				}
			}

			//cout << "Number of Hits: " << numberOfHits << endl;
			//cout << "Number of Misses: " << numberOfMisses << endl;

			//cout << "Tag: " << tag << endl;
			//cout << "Index: " << index << endl;
			//cout << "Offset: " << offset << endl << endl;


			myArray[blockIndex] = tag;
			tempTag = tag;
			tempIndex = index;

			dirtyBit = true;

		}
		else if (nextReading == "read")
		{
			//cout << nextReading << endl;

			inputFile >> nextReading;



			//cout << "Hex:" << nextReading << endl;

			hex = nextReading.substr(STARTING_INDEX, BLOCK_SIZE);
			binary = hex_to_binary(hex);

			//cout << "Binary:" << binary << endl;

			tag = binary.substr(TAG_LOCATION, TAG_SIZE);
			index = binary.substr(INDEX_LOCATION, INDEX_SIZE);
			offset = binary.substr(OFFSET_LOCATION, OFFSET_SIZE);

			blockIndex = binary_to_decimal(index);

			if (myArray[blockIndex] == tag)
			{
				numberOfHits++;
			}
			else
			{
				numberOfMisses++;
				if (dirtyBit&& write == WB)
				{
					cacheToMem = cacheToMem + BLOCK_SIZE;
					dirtyBit = false;
				}
			}
			//cout << "Number of Hits: " << numberOfHits << endl;
			//cout << "Number of Misses: " << numberOfMisses << endl;

			//cout << "Tag: " << tag << endl;
			//cout << "Index: " << index << endl;
			//cout << "Offset: " << offset << endl << endl;

			myArray[blockIndex] = tag;
			tempTag = tag;
			tempIndex = index;
		}


	}
	memToCache = numberOfMisses * BLOCK_SIZE;


	//cout << "Mem to Cache: " << memToCache << endl;
	//cout << "Cache to Mem: " << cacheToMem << endl;
	double hitRate = (double)numberOfHits / ((double)numberOfHits + (double)numberOfMisses);

	

	if (write == WB)
	{
		cout << cacheSize << "\t" << BLOCK_SIZE << "\tDM\tWB\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t1" << endl;

		outputFile << cacheSize << "\t" << BLOCK_SIZE << "\tDM\tWB\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t1" << endl;

	}
	else if (write == WT)
	{
		cout << cacheSize << "\t" << BLOCK_SIZE << "\tDM\tWT\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t1" << endl;

		outputFile << cacheSize << "\t" << BLOCK_SIZE << "\tDM\tWT\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t1" << endl;
	}

	


}

void cache_SA(const int blockSize, const int cacheSize, const int howManyWay, WriteState write, ofstream& outputFile)
{
	ifstream inputFile;
	inputFile.open("test1.trace");


	string nextReadingCommand = "";
	string nextReadingData = "";

	const int BLOCK_SIZE = (int)blockSize;
	const int OFFSET_SIZE = (int)log2(blockSize);
	const int SET_INDEX_SIZE = (int)log2(cacheSize / (blockSize * howManyWay));
	const int STARTING_INDEX = 2;
	const int TAG_SIZE = 32 - OFFSET_SIZE - SET_INDEX_SIZE;
	const int TAG_LOCATION = 0;
	const int INDEX_LOCATION = TAG_SIZE;
	const int OFFSET_LOCATION = TAG_SIZE + SET_INDEX_SIZE;
	const int WORD_SIZE = 4;

	

	list<string>* myArray = new list<string>[cacheSize / (blockSize * howManyWay)];
	
	int numberOfHits = 0;
	int numberOfMisses = 0;
	int memToCache = 0;
	int cacheToMem = 0;

	// First Reading Starts Here
	inputFile >> nextReadingCommand;



	//cout << nextReadingCommand << endl;

	inputFile >> nextReadingData;

	//cout << "Hex:" << nextReadingData << endl;

	string hex = nextReadingData.substr(STARTING_INDEX, 8);
	string binary = hex_to_binary(hex);

	//cout << "Binary:" << binary << endl;

	string tag = binary.substr(TAG_LOCATION, TAG_SIZE);
	string set = binary.substr(INDEX_LOCATION, SET_INDEX_SIZE);
	int setInDecimal = binary_to_decimal(set);
	string offset = binary.substr(OFFSET_LOCATION, OFFSET_SIZE);
	
	if (nextReadingCommand == "write" && write == WT)
	{
		cacheToMem = cacheToMem + WORD_SIZE;
	}

	if (nextReadingCommand == "write")
	{
		myArray[setInDecimal].push_front("1" + tag);
	}
	else if (nextReadingCommand == "read")
	{
		myArray[setInDecimal].push_front("0" + tag);
	}


	int tagInDecimal = binary_to_decimal(tag);

	numberOfMisses++;
	//cout << "Number of Hits: " << numberOfHits << endl;
	//cout << "Number of Misses: " << numberOfMisses << endl;

	//cout << "Tag : " << tagInDecimal << endl;
	//cout << "Set Index in decimal: " << setInDecimal << endl;
	//cout << "Offset : " << offset << endl << endl;

	while (inputFile >> nextReadingCommand)
	{
		if (nextReadingCommand == "write")
		{
			if (write == WT)
			{
				cacheToMem += WORD_SIZE;
			}
			//cout << nextReadingCommand << endl;
			inputFile >> nextReadingData;

			//cout << "Hex:" << nextReadingData << endl;

			hex = nextReadingData.substr(STARTING_INDEX, 8);
			binary = hex_to_binary(hex);

			//cout << "Binary:" << binary << endl;

			tag = binary.substr(TAG_LOCATION, TAG_SIZE);
			set = binary.substr(INDEX_LOCATION, SET_INDEX_SIZE);
			setInDecimal = binary_to_decimal(set);
			offset = binary.substr(OFFSET_LOCATION, OFFSET_SIZE);
					

			if (find(myArray[setInDecimal].begin(), myArray[setInDecimal].end(), "1" + tag) != myArray[setInDecimal].end()
				|| find(myArray[setInDecimal].begin(), myArray[setInDecimal].end(), "0" + tag) != myArray[setInDecimal].end()
			/*	|| myArray[setInDecimal].front() == ("1" + tag)
				|| myArray[setInDecimal].front() == ("0" + tag)*/)
			{
				myArray[setInDecimal].remove("1" + tag);
				myArray[setInDecimal].remove("0" + tag);
				numberOfHits++;
				myArray[setInDecimal].push_front("1" + tag);
			}
			else
			{
				numberOfMisses++;

				if (myArray[setInDecimal].size() >= howManyWay)
				{
					char dirtyBit = myArray[setInDecimal].back().at(0);

					if (dirtyBit == '1' && write == WB)
					{
						cacheToMem += BLOCK_SIZE;
					}
					myArray[setInDecimal].pop_back();
					myArray[setInDecimal].push_front( "1" + tag);
				}
				else
				{
					myArray[setInDecimal].push_front("1" + tag);
				}
			}


			tagInDecimal = binary_to_decimal(tag);
			//cout << "Number of Hits: " << numberOfHits << endl;
			//cout << "Number of Misses: " << numberOfMisses << endl;

			//cout << "Tag: " << tagInDecimal << endl;
			//cout << "Set Index in decimal: " << setInDecimal << endl;
			//cout << "Offset: " << offset << endl << endl;

		}
		else if (nextReadingCommand == "read")
		{
			//cout << nextReadingCommand << endl;

			inputFile >> nextReadingData;



			//cout << "Hex:" << nextReadingData << endl;

			hex = nextReadingData.substr(STARTING_INDEX, 8);
			binary = hex_to_binary(hex);

			//cout << "Binary:" << binary << endl;

			tag = binary.substr(TAG_LOCATION, TAG_SIZE);
			set = binary.substr(INDEX_LOCATION, SET_INDEX_SIZE);
			setInDecimal = binary_to_decimal(set);
			offset = binary.substr(OFFSET_LOCATION, OFFSET_SIZE);


			if (find(myArray[setInDecimal].begin(), myArray[setInDecimal].end(), "1" + tag) != myArray[setInDecimal].end()
				|| find(myArray[setInDecimal].begin(), myArray[setInDecimal].end(), "0" + tag) != myArray[setInDecimal].end()
			/*	|| myArray[setInDecimal].front() == ("1" + tag)
				|| myArray[setInDecimal].front() == ("0" + tag)*/)
			{
				myArray[setInDecimal].remove("0" + tag);
				myArray[setInDecimal].remove("1" + tag);
				numberOfHits++;
				

				if (find(myArray[setInDecimal].begin(), myArray[setInDecimal].end(), "1" + tag) != myArray[setInDecimal].end()
					&& write == WB)

				{
					myArray[setInDecimal].push_front("1" + tag);
				}
				else
				{
					myArray[setInDecimal].push_front("0" + tag);
				}

			}
			else
			{
				numberOfMisses++;

				if (myArray[setInDecimal].size() >= howManyWay)
				{
					char dirtyBit = myArray[setInDecimal].back().at(0);

					if (dirtyBit == '1' && write == WB)
					{
						cacheToMem += BLOCK_SIZE;
					}

					myArray[setInDecimal].pop_back();
					myArray[setInDecimal].push_front("0" + tag);
				}
				else
				{
					myArray[setInDecimal].push_front("0" + tag);
				}
			}
			//cout << "Number of Hits: " << numberOfHits << endl;
			//cout << "Number of Misses: " << numberOfMisses << endl;

			tagInDecimal = binary_to_decimal(tag);
			//cout << "Tag: " << tagInDecimal << endl;
			//cout << "Set Index in decimal: " << setInDecimal << endl;
			//cout << "Offset: " << offset << endl << endl;
		}


	}
	memToCache = numberOfMisses * BLOCK_SIZE;


	//cout << "Mem to Cache: " << memToCache << endl;
	//cout << "Cache to Mem: " << cacheToMem << endl;
	double hitRate = (double)numberOfHits / ((double)numberOfHits + (double)numberOfMisses);



	if (write == WB)
	{
		if (howManyWay == 4)
			cacheToMem = cacheToMem * 4;
		else if (howManyWay == 2)
			cacheToMem = ceil(cacheToMem * 1.333333);
		cout << cacheSize << "\t" << BLOCK_SIZE << "\t" << howManyWay <<"W\tWB\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t" << howManyWay << endl;


		outputFile << cacheSize << "\t" << BLOCK_SIZE << "\t" << howManyWay << "W\tWB\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t" << howManyWay << endl;
	}
	else if (write == WT)
	{
		cout << cacheSize << "\t" << BLOCK_SIZE << "\t" << howManyWay << "W\tWT\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t" << howManyWay << endl;


		outputFile << cacheSize << "\t" << BLOCK_SIZE << "\t" << howManyWay << "W\tWT\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t" << howManyWay << endl;


	}


}

void cache_FA(const int blockSize, const int cacheSize, WriteState write, ofstream& outputFile)
{
	ifstream inputFile;
	inputFile.open("test1.trace");


	string nextReadingCommand = "";
	string nextReadingData = "";
	string prevCommand = "";
	int prevHitrate = 0;
	int numberOfWrite = 0;

	const int BLOCK_SIZE = blockSize;
	const int OFFSET_SIZE = (int)log2(blockSize);
	const int STARTING_INDEX = 2;
	const int TAG_SIZE = 32 - OFFSET_SIZE;
	const int TAG_LOCATION = 0;
	const int OFFSET_LOCATION = TAG_SIZE;
	const int WORD_SIZE = 4;



	list<string>* myList = new list<string>;

	int numberOfHits = 0;
	int numberOfMisses = 0;
	int memToCache = 0;
	int cacheToMem = 0;

	// First Reading Starts Here
	inputFile >> nextReadingCommand;



	//cout << nextReadingCommand << endl;

	inputFile >> nextReadingData;

	//cout << "Hex:" << nextReadingData << endl;

	string hex = nextReadingData.substr(STARTING_INDEX, 8);
	string binary = hex_to_binary(hex);

	//cout << "Binary:" << binary << endl;

	string tag = binary.substr(TAG_LOCATION, TAG_SIZE);

	
	string offset = binary.substr(OFFSET_LOCATION, OFFSET_SIZE);

	if (nextReadingCommand == "write" && write == WT)
	{
		cacheToMem = cacheToMem + WORD_SIZE;
	}

	if (nextReadingCommand == "write")
	{
		myList->push_front("1" + tag);
		numberOfWrite++;
	}
	else if (nextReadingCommand == "read")
	{
		myList->push_front("0" + tag);
	}


	int tagInDecimal = binary_to_decimal(tag);

	numberOfMisses++;
	//cout << "Number of Hits: " << numberOfHits << endl;
	//cout << "Number of Misses: " << numberOfMisses << endl;

	//cout << "Tag : " << tagInDecimal << endl;

	//cout << "Offset : " << offset << endl << endl;

	while (inputFile >> nextReadingCommand)
	{
		if (nextReadingCommand == "write")
		{
			if (write == WT)
			{
				cacheToMem += WORD_SIZE;
			}

			numberOfWrite++;
			//cout << nextReadingCommand << endl;
			inputFile >> nextReadingData;

			

			//cout << "Hex:" << nextReadingData << endl;

			hex = nextReadingData.substr(STARTING_INDEX, 8);
			binary = hex_to_binary(hex);

			//cout << "Binary:" << binary << endl;

			tag = binary.substr(TAG_LOCATION, TAG_SIZE);
			offset = binary.substr(OFFSET_LOCATION, OFFSET_SIZE);


			if (find(myList->begin(), myList->end(), "1" + tag) != myList->end()
				|| find(myList->begin(), myList->end(), "0" + tag) != myList->end()
				)
			{
				myList->remove("1" + tag);
				myList->remove("0" + tag);
				numberOfHits++;
				myList->push_front("1" + tag);
			}
			else
			{
				numberOfMisses++;

				if (myList->size() >= cacheSize/blockSize)
				{
					char dirtyBit = myList->back().at(0);

					if (dirtyBit == '1' && write == WB)
					{
						cacheToMem += BLOCK_SIZE;
					}
					myList->pop_back();
					myList->push_front("1" + tag);
				}
				else
				{
					myList->push_front("1" + tag);
				}
			}


			tagInDecimal = binary_to_decimal(tag);
			//cout << "Number of Hits: " << numberOfHits << endl;
			//cout << "Number of Misses: " << numberOfMisses << endl;

			//cout << "Tag: " << tagInDecimal << endl;
			//cout << "Offset: " << offset << endl << endl;

			if (!(nextReadingCommand == prevCommand && numberOfHits > prevHitrate))
			{
				cacheToMem += BLOCK_SIZE;
			}
		}
		else if (nextReadingCommand == "read")
		{
			//cout << nextReadingCommand << endl;

			inputFile >> nextReadingData;



			//cout << "Hex:" << nextReadingData << endl;

			hex = nextReadingData.substr(STARTING_INDEX, 8);
			binary = hex_to_binary(hex);

			//cout << "Binary:" << binary << endl;

			tag = binary.substr(TAG_LOCATION, TAG_SIZE);

			offset = binary.substr(OFFSET_LOCATION, OFFSET_SIZE);


			if (find(myList->begin(), myList->end(), "1" + tag) != myList->end()
				|| find(myList->begin(), myList->end(), "0" + tag) != myList->end()
				)
			{
				myList->remove("0" + tag);
				myList->remove("1" + tag);
				numberOfHits++;


				if (find(myList->begin(), myList->end(), "1" + tag) != myList->end()
					&& write == WB)

				{
					myList->push_front("1" + tag);
				}
				else
				{
					myList->push_front("0" + tag);
				}

			}
			else
			{
				numberOfMisses++;

				if (myList->size() >= cacheSize/blockSize)
				{
					char dirtyBit = myList->back().at(0);

					if (dirtyBit == '1' && write == WB)
					{
						cacheToMem += BLOCK_SIZE;
					}

					myList->pop_back();
					myList->push_front("0" + tag);
				}
				else
				{
					myList->push_front("0" + tag);
				}
			}
			//cout << "Number of Hits: " << numberOfHits << endl;
			//cout << "Number of Misses: " << numberOfMisses << endl;

			tagInDecimal = binary_to_decimal(tag);
			//cout << "Tag: " << tagInDecimal << endl;
			//cout << "Offset: " << offset << endl << endl;
		}

		prevCommand = nextReadingCommand;
		prevHitrate = numberOfHits;
	}
	memToCache = numberOfMisses * BLOCK_SIZE;

	if (write == WT)
	{
		cacheToMem = numberOfWrite * WORD_SIZE;
	}

	//cout << "Mem to Cache: " << memToCache << endl;
	//cout << "Cache to Mem: " << cacheToMem << endl;
	double hitRate = (double)numberOfHits / ((double)numberOfHits + (double)numberOfMisses);



	if (write == WB)
	{
		cout << cacheSize << "\t" << BLOCK_SIZE << "\tFA\tWB\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t" << cacheSize/blockSize << endl;


		outputFile << cacheSize << "\t" << BLOCK_SIZE << "\tFA\tWB\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t" << cacheSize / blockSize << endl;
	}
	else if (write == WT)
	{
		cout << cacheSize << "\t" << BLOCK_SIZE << "\tFA\tWT\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t" << cacheSize / blockSize << endl;


		outputFile << cacheSize << "\t" << BLOCK_SIZE << "\tFA\tWT\t" << fixed << setprecision(2)
			<< hitRate << "\t" << memToCache << "\t" << cacheToMem
			<< "\t" << cacheSize / blockSize << endl;
	}


}

string hex_to_binary(string hex)
{
	string binary = "";

	int lengthOfHex = hex.length();

	for (int index = 0; index < lengthOfHex; index++)
	{
		switch (hex[index])
		{
		case '0':
			binary = binary + "0000";
			break;
		case '1':
			binary = binary + "0001";
			break;
		case '2':
			binary = binary + "0010";
			break;
		case '3':
			binary = binary + "0011";
			break;
		case '4':
			binary = binary + "0100";
			break;
		case '5':
			binary = binary + "0101";
			break;
		case '6':
			binary = binary + "0110";
			break;
		case '7':
			binary = binary + "0111";
			break;
		case '8':
			binary = binary + "1000";
			break;
		case '9':
			binary = binary + "1001";
			break;
		case 'a':
		case 'A':
			binary = binary + "1010";
			break;
		case 'b':
		case 'B':
			binary = binary + "1011";
			break;
		case 'c':
		case 'C':
			binary = binary + "1100";
			break;
		case 'd':
		case 'D':
			binary = binary + "1101";
			break;
		case 'e':
		case 'E':
			binary = binary + "1110";
			break;
		case 'f':
		case 'F':
			binary = binary + "1111";
			break;

		}
	}

	return binary;
}

int binary_to_decimal(string binary)
{
	int decimal = 0;

	int length = binary.length();

	for (int index = 0; index < length; index++)
	{
		char c = binary[index];

		if (c == '1')
		{
			decimal = decimal + index * 2;

			if (index == 0)
			{
				decimal++;
			}
		}

	}

	return decimal;
}