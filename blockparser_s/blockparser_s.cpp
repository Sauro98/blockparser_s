// blockparser_s.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FileReader.h"
#include "BlockReader.h"
int main()
{
	size_t fileSize = 0;
	uint8_t* fileContent = FileReader::getFileContent("../blk00000.dat", &fileSize);
	BlockReader blockReader(fileContent);
	//TODO:: write test based on genesis block
	BBlockChain::Block block = blockReader.readBlock();
	return 0;
}

