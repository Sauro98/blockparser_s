#pragma once

#include<stdint.h>
#include <math.h>
#include "BitwiseReader.h"
#include "globals.h"
#include "Block.h"

typedef std::shared_ptr<BBlockChain::Transaction> TransactionArray;
typedef std::shared_ptr<BBlockChain::Input> InputArray;
typedef std::shared_ptr<BBlockChain::Output> OutputArray;
typedef std::shared_ptr<uint8_t> CharArray;

class BlockReader {

private:

	uint8_t* blockFile;
	uint64_t blocksRead;
	uint64_t currentIndex;
	uint64_t fileSize;
	long double lastperc = -1.0;
public: 

	BlockReader(uint8_t* _blockFile,uint64_t _fileSize) {
		blockFile = _blockFile;
		fileSize = _fileSize;
		currentIndex = 0;
		blocksRead = 0;
	}

	BBlockChain::Block readBlock() {
		BBlockChain::Block currentBlock;
		currentBlock.magicID = read32BitValue();
		currentBlock.headerLength = read32BitValue();
		uint64_t startIndex = currentIndex;
		currentBlock.versionNumber = read32BitValue();
		currentBlock.previousBlockHash = read256bitHash();
		currentBlock.merkleRoot = read256bitHash();
		currentBlock.timeStamp = read32BitValue();
		currentBlock.targetDifficulty = read32BitValue();
		currentBlock.nonce = read32BitValue();
		currentBlock.transactionCount = readVariableLengthInteger();
		currentBlock.transactions = readTransactions(currentBlock.transactionCount);
		blocksRead++;
		if (currentIndex - startIndex < currentBlock.headerLength) {
			println("Gap found: "<<currentIndex);
			uint64_t diff = currentBlock.headerLength - (currentIndex - startIndex);
			currentIndex += diff;
		}
		return currentBlock;
	}

	bool hasNext() {
		long double perc = currentIndex / ((long double)fileSize);
		perc = floorf((float)perc * 100) / 100;
		lastperc = floorf((float)lastperc * 100) / 100;
		if (perc != lastperc) {
			lastperc = perc;
			rollbackLine;
			print(perc * 100 << "%");
		}
		return (currentIndex < fileSize) && (fileSize - currentIndex) > 80;
	}

	uint64_t totalBlocksRead() {
		return blocksRead;
	}

protected:

	TransactionArray readTransactions(uint64_t transactionCount) {
		TransactionArray transactions (new BBlockChain::Transaction[transactionCount],std::default_delete<BBlockChain::Transaction[]>());
		for (int transactionIndex = 0; transactionIndex < transactionCount; transactionIndex++) {
			transactions.get()[transactionIndex] = readTransaction();
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

	InputArray readInputs(uint64_t inputCount) {
		InputArray inputs (new BBlockChain::Input[inputCount], std::default_delete<BBlockChain::Input[]>());
		for (uint64_t inputIndex = 0; inputIndex < inputCount; inputIndex++) {
			inputs.get()[inputIndex] = readInput();
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

	OutputArray readOutputs(uint64_t outputCount) {
		OutputArray outputs(new BBlockChain::Output[outputCount], std::default_delete<BBlockChain::Output[]>());
		for (uint64_t outputIndex = 0; outputIndex < outputCount; outputIndex++) {
			outputs.get()[outputIndex] = readOutput();
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

	CharArray read256bitHash() {
		CharArray previousBlockHash (new uint8_t[32], std::default_delete<uint8_t[]>());
		for (int a = 0; a < SHA256_LENGTH; a++) {
			previousBlockHash.get()[a] = blockFile[a + currentIndex];
		}
		currentIndex += 32;
		return previousBlockHash;
	}

	CharArray readScript(uint64_t scriptLength) {
		CharArray valueToReturn (new uint8_t[scriptLength], std::default_delete<uint8_t[]>());
		for (int a = 0; a < scriptLength; a++) {
			valueToReturn.get()[a] = blockFile[a + currentIndex];
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