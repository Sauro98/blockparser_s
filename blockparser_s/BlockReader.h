#pragma once

#include<stdint.h>
#include "BitwiseReader.h"
#include "globals.h"
#include "Block.h"

class BlockReader {

private:

	uint8_t* blockFile;
	uint64_t currentIndex;

public: 

	BlockReader(uint8_t* _blockFile) {
		blockFile = _blockFile;
		currentIndex = 0;
	}

	BBlockChain::Block readBlock() {
		BBlockChain::Block currentBlock;
		currentBlock.magicID = read32BitValue();
		currentBlock.headerLength = read32BitValue();
		currentBlock.versionNumber = read32BitValue();
		currentBlock.previousBlockHash = read256bitHash();
		currentBlock.merkleRoot = read256bitHash();
		currentBlock.timeStamp = read32BitValue();
		currentBlock.targetDifficulty = read32BitValue();
		currentBlock.nonce = read32BitValue();
		currentBlock.transactionCount = readVariableLengthInteger();
		currentBlock.transactions = readTransactions(currentBlock.transactionCount);
		currentBlock.printBlock();
		return currentBlock;
	}

private:

	BBlockChain::Transaction* readTransactions(uint64_t transactionCount) {
		BBlockChain::Transaction* transactions = new BBlockChain::Transaction[transactionCount];
		for (int transactionIndex = 0; transactionIndex < transactionCount; transactionIndex++) {
			transactions[transactionIndex] = readTransaction();
		}
		return transactions;
	}

	BBlockChain::Transaction readTransaction() {
		BBlockChain::Transaction transaction;
		transaction.transactionVersionNumber = read32BitValue();
		transaction.inputCount = readVariableLengthInteger();
		transaction.inputs = readInputs(transaction.inputCount);
		transaction.outputCount = readVariableLengthInteger();
		transaction.outputs = readOutputs(transaction.outputCount);
		transaction.transactionLockTime = read32BitValue();
		return transaction;
	}

	BBlockChain::Input* readInputs(uint64_t inputCount) {
		BBlockChain::Input* inputs = new BBlockChain::Input[inputCount];
		for (uint64_t inputIndex = 0; inputIndex < inputCount; inputIndex++) {
			inputs[inputIndex] = readInput();
		}
		return inputs;
	}

	BBlockChain::Input readInput() {
		BBlockChain::Input input;
		input.transactionHash = read256bitHash();
		input.transactionIndex = read32BitValue();
		input.scriptLength = readVariableLengthInteger();
		input.script = readScript(input.scriptLength);
		input.sequenceNumber = read32BitValue();
		return input;
	}

	BBlockChain::Output* readOutputs(uint64_t outputCount) {
		BBlockChain::Output* outputs = new BBlockChain::Output[outputCount];
		for (uint64_t outputIndex = 0; outputIndex < outputCount; outputIndex++) {
			outputs[outputIndex] = readOutput();
		}
		return outputs;
	}

	BBlockChain::Output readOutput() {
		BBlockChain::Output output;
		output.value = read64BitValue();
		output.scriptLength = readVariableLengthInteger();
		output.script = readScript(output.scriptLength);
		return output;
	}

	uint32_t read32BitValue() {
		uint32_t returnValue = BitwiseReader::read32bitReversed(blockFile, currentIndex);
		currentIndex += 4;
		return returnValue;
	}

	uint64_t read64BitValue() {
		uint64_t returnValue = BitwiseReader::read64bitReversed(blockFile, currentIndex);
		currentIndex += 8;
		return returnValue;
	}

	uint8_t* read256bitHash() {
		uint8_t* previousBlockHash = new uint8_t[32];
		for (int a = 0; a < SHA256_LENGTH; a++) {
			previousBlockHash[a] = blockFile[a + currentIndex];
		}
		currentIndex += 32;
		return previousBlockHash;
	}

	uint8_t* readScript(uint64_t scriptLength) {
		uint8_t* valueToReturn = new uint8_t[scriptLength];
		for (int a = 0; a < scriptLength; a++) {
			valueToReturn[a] = blockFile[a + currentIndex];
		}
		currentIndex += scriptLength;
		return valueToReturn;
	}

	uint64_t readVariableLengthInteger() {
		uint8_t firstByte = blockFile[currentIndex];
		uint64_t nextByteIndex = currentIndex + 1;
		if (firstByte < 0xFD) {
			currentIndex += 1;
			return firstByte;
		} else if (firstByte == 0xFD) {
			currentIndex += 3;
			return BitwiseReader::read16bitReversed(blockFile, nextByteIndex);
		} else if (firstByte == 0xFE) {
			currentIndex += 5;
			return BitwiseReader::read32bitReversed(blockFile, nextByteIndex);
		} else {
			currentIndex += 9;
			return BitwiseReader::read64bitReversed(blockFile, nextByteIndex);
		}
	}

};