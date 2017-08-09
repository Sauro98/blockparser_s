#pragma once

#include "../BlockReader.h"

class BlockReaderTester : BlockReader{
public:
	BlockReaderTester(uint8_t* content,uint64_t fileLength) : BlockReader(content, fileLength){}
	uint64_t testReadVariableLengthInteger() {
		return readVariableLengthInteger();
	}
};