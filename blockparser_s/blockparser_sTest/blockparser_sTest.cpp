// blockparser_sTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gtest/gtest.h"
#include "../BitwiseReader.h"
#include "../BlockReader.h" 

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

