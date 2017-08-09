// blockparser_s.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileReader.h"
#include "BlockReader.h"
#include <fstream>
#include <time.h>

int main()
{
	size_t fileSize = 0;
	uint8_t* fileContent = FileReader::getFileContent("../blk00000t.dat", &fileSize);
	BlockReader blockReader(fileContent,fileSize);
	BBlockChain::Block block;
	println("Lettura della blockchain");
	clock_t tStart = clock();
	while (blockReader.hasNext()) {
		block = blockReader.readBlock();
	}
	printf("\nFile letto in: %.2f secondi\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	std::ofstream myfile;
	myfile.open("lastblock.txt");
	block.printBlockToFile(&myfile);
	myfile.close();
	println("Total blocks in file: " << std::dec <<blockReader.totalBlocksRead());
	delete[] fileContent;
	return 0;
}

