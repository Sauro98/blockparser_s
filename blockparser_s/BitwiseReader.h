#pragma once

#include<stdint.h>
#include "globals.h"

namespace BitwiseReader {

	//abstract class for bit shifting
	struct Shifter {
		int shiftValue;
		Shifter(int _startShifter) {
			shiftValue = _startShifter;
		}
		virtual void nextShifter() = 0;
	};

	//shifts the bytes from 0 to end
	struct IncrementalShifter : Shifter {
		IncrementalShifter() : Shifter(0) {}
		IncrementalShifter(int start) : Shifter(start) {}

		void nextShifter() {
			shiftValue += 8;
		}
	};

	//shifts the bytes from start to 0
	struct DecrementalShifter : Shifter {
		DecrementalShifter(int start) : Shifter(start) {}

		void nextShifter() {
			shiftValue -= 8;
		}
	};

	//lower level functions for reading 32 bits
	static uint32_t read32BitInt(uint8_t* content, size_t offset,Shifter* shifter) {
		uint32_t returnValue = 0;
		for (size_t a = offset; a < offset + 4; a++) {
			uint32_t currbyte = content[a];
			currbyte <<= shifter->shiftValue;
			returnValue |= currbyte;
			shifter->nextShifter();
		}
		delete shifter;
		return returnValue;
	}

	//lower level function for reading 16 bits
	static uint16_t read16BitInt(uint8_t* content, size_t offset, Shifter* shifter) {
		uint16_t returnValue = 0;
		for (size_t a = offset; a < offset + 2; a++) {
			uint16_t currbyte = content[a];
			currbyte <<= shifter->shiftValue;
			returnValue |= currbyte;
			shifter->nextShifter();
		}
		delete shifter;
		return returnValue;
	}

	//lower level function for reading 64 bits
	static uint64_t read64BitInt(uint8_t* content, size_t offset, Shifter* shifter) {
		uint64_t returnValue = 0;
		for (size_t a = offset; a < offset + 8; a++) {
			uint64_t currbyte = content[a];
			currbyte <<= shifter->shiftValue;
			returnValue |= currbyte;
			shifter->nextShifter();
		}
		delete shifter;
		return returnValue;
	}

	//reads 32 bits in big endian
	static uint32_t read32bit(uint8_t* content, size_t offset) {
		return read32BitInt(content,offset,new DecrementalShifter(24));
	}

	//reads 32 bits in little endian
	static uint32_t read32bitReversed(uint8_t* content, size_t offset) {
		return read32BitInt(content, offset, new IncrementalShifter());
	}

	//reads 16 bits in big endian
	static uint16_t read16bit(uint8_t* content, size_t offset) {
		return read16BitInt(content, offset, new DecrementalShifter(8));
	}

	//reads 16 bits in little endian
	static uint16_t read16bitReversed(uint8_t* content, size_t offset) {
		return read16BitInt(content, offset, new IncrementalShifter());
	}

	//reads 64 bits in big endian
	static uint64_t read64bit(uint8_t* content, size_t offset) {
		return read64BitInt(content, offset, new DecrementalShifter(56));
	}

	//reads 64 bits in little endian
	static uint64_t read64bitReversed(uint8_t* content, size_t offset) {
		return read64BitInt(content, offset, new IncrementalShifter());
	}
	

}