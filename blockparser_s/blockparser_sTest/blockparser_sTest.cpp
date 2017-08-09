// blockparser_sTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"
#include "../BitwiseReader.h"
#include "../BlockReader.h" 
#include "../FileReader.h"
#include "BlockReaderTester.h"

uint8_t genesisMerkleRoot[32] = { 0x4a,0x5e,0x1e,0x4b,0xaa,0xb8,0x9f,0x3a,0x32,0x51,0x8a,0x88,0xc3,0x1b,0xc8,0x7f,0x61,0x8f,0x76,0x67,0x3e,0x2c,0xc7,0x7a,0xb2,0x12,0x7b,0x7a,0xfd,0xed,0xa3,0x3b };
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(BitwiseReaderTest, canRead64bit) {
	uint8_t bytes[8] = { 0xD9,0xB4,0xBE,0xF9,0xF3,0x52,0x01,0x23 };
	EXPECT_EQ(BitwiseReader::read64bit(bytes, 0), 0xD9B4BEF9F3520123);
}

TEST(BitwiseReaderTest, canRead64bitReversed) {
	uint8_t bytes[8] = { 0x23,0x01,0x52,0xF3,0xF9,0xBE,0xB4,0xD9 };
	EXPECT_EQ(BitwiseReader::read64bitReversed(bytes, 0), 0xD9B4BEF9F3520123);
}

TEST(BitwiseReaderTest, canRead32bit) {
	uint8_t bytes[4] = { 0xD9,0xB4,0xBE,0xF9 };
	EXPECT_EQ(BitwiseReader::read32bit(bytes,0), 0xD9B4BEF9);
}

TEST(BitwiseReaderTest, canRead32bitReversed) {
	uint8_t bytes[4] = { 0xF9,0xBE,0xB4,0xD9 };
	EXPECT_EQ(BitwiseReader::read32bitReversed(bytes, 0), 0xD9B4BEF9);
}

TEST(BitwiseReaderTest, canRead16bit) {
	uint8_t bytes[2] = { 0xD9,0xB4 };
	EXPECT_EQ(BitwiseReader::read16bit(bytes, 0), 0xD9B4);
}

TEST(BitwiseReaderTest, canRead16bitReversed) {
	uint8_t bytes[2] = { 0xB4,0xD9 };
	EXPECT_EQ(BitwiseReader::read16bitReversed(bytes, 0), 0xD9B4);
}

TEST(BlockReaderTest, CanReadVariableLengthInteger) {
	uint8_t content[1] = { 0x08};
	uint8_t content3[3] = { 0xFD,0x03,0x00 };
	uint8_t content5[5] = { 0xFE,0x05,0x00,0x00,0x00 };
	uint8_t content9[9] = { 0xFF,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	BlockReaderTester tester(content,1);
	EXPECT_EQ(tester.testReadVariableLengthInteger(), 8);
	tester = BlockReaderTester(content3,3);
	EXPECT_EQ(tester.testReadVariableLengthInteger(), 3);
	tester = BlockReaderTester(content5,5);
	EXPECT_EQ(tester.testReadVariableLengthInteger(), 5);
	tester = BlockReaderTester(content9,9);
	EXPECT_EQ(tester.testReadVariableLengthInteger(), 9);
}

TEST(BlockReaderTest, canReadGenesisBlockHeader) {
	size_t fileSize = 0;
	uint8_t* fileContent = FileReader::getFileContent("../../blk00000.dat", &fileSize);
	BlockReader blockReader(fileContent,fileSize);
	BBlockChain::Block genesisBlock = blockReader.readBlock();
	EXPECT_EQ(genesisBlock.magicID, 0xD9B4BEF9);
	EXPECT_EQ(genesisBlock.headerLength, 285);
	EXPECT_EQ(genesisBlock.versionNumber, 1);
	for (int a = 0; a < 32; a++) {
		EXPECT_EQ(genesisBlock.previousBlockHash.get()[a], 0);
	}
	for (int a = 0; a < 32; a++) {
		EXPECT_EQ(genesisBlock.merkleRoot.get()[31 - a], genesisMerkleRoot[a]);
	}
	EXPECT_EQ(genesisBlock.timeStamp, 1231006505);
	EXPECT_EQ(genesisBlock.targetDifficulty, 0x1d00ffff);
	EXPECT_EQ(genesisBlock.nonce, 2083236893);
	EXPECT_EQ(genesisBlock.transactionCount, 1);
}

TEST(BlockReaderTest, canReadGenesisTransaction) {
	size_t fileSize = 0;
	uint8_t* fileContent = FileReader::getFileContent("../../blk00000.dat", &fileSize);
	BlockReader blockReader(fileContent,fileSize);
	BBlockChain::Block genesisBlock = blockReader.readBlock();
	BBlockChain::Transaction genesisTransaction = genesisBlock.transactions.get()[0];
	EXPECT_EQ(genesisTransaction.transactionVersionNumber, 1);
	EXPECT_EQ(genesisTransaction.inputCount, 1);
	EXPECT_EQ(genesisTransaction.outputCount, 1);
	EXPECT_EQ(genesisTransaction.transactionLockTime, 0);
}

TEST(BlockReaderTest, canReadGenesisInput) {
	size_t fileSize = 0;
	uint8_t* fileContent = FileReader::getFileContent("../../blk00000.dat", &fileSize);
	BlockReader blockReader(fileContent,fileSize);
	BBlockChain::Block genesisBlock = blockReader.readBlock();
	BBlockChain::Transaction genesisTransaction = genesisBlock.transactions.get()[0];
	BBlockChain::Input genesisInput = genesisTransaction.inputs.get()[0];
	for (int a = 0; a < 32; a++) {
		EXPECT_EQ(genesisInput.transactionHash.get()[a], 0);
	}
	EXPECT_EQ(genesisInput.transactionIndex, 0xFFFFFFFF);
	EXPECT_EQ(genesisInput.scriptLength, 0x4D);
	EXPECT_EQ(genesisInput.sequenceNumber,0xFFFFFFFF);
}

TEST(BlockReaderTest, canReadGenesisOutput) {
	size_t fileSize = 0;
	uint8_t* fileContent = FileReader::getFileContent("../../blk00000.dat", &fileSize);
	BlockReader blockReader(fileContent,fileSize);
	BBlockChain::Block genesisBlock = blockReader.readBlock();
	BBlockChain::Transaction genesisTransaction = genesisBlock.transactions.get()[0];
	BBlockChain::Output genesisOutput = genesisTransaction.outputs.get()[0];
	EXPECT_EQ(genesisOutput.value, 0x000000012A05F200);
	EXPECT_EQ(genesisOutput.scriptLength, 0x43);
}
