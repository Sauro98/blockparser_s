#pragma once

#include "globals.h"
#include <stdint.h>
#include <ctime>

namespace BBlockChain {

	static void printHash(uint8_t* hash);
	static void printTimestamp(uint32_t timestamp);

	typedef struct Output {
		uint64_t value;
		uint64_t scriptLength;
		uint8_t* script;

		void printOutput() {
			println("|\t|\t| OutputValue: " << std::dec << value);
		}
	}Output;

	typedef struct Input {
		uint8_t* transactionHash;
		uint32_t transactionIndex;
		uint64_t scriptLength;
		uint8_t* script;
		uint32_t sequenceNumber;

		void printInput() {
			print("|\t|\t| TransactionHash: ");
			printHash(transactionHash);
			println("|\t|\t| TransactionIndex: " << std::hex << transactionIndex);
			println("|\t|\t| SequenceNumber: " << std::dec << sequenceNumber);
		}
	}Input;

	typedef struct Transaction {
		uint32_t transactionVersionNumber;
		uint64_t inputCount;
		Input* inputs;
		uint64_t outputCount;
		Output* outputs;
		uint32_t transactionLockTime;

		void printTransaction() {
			println("|\t| TransactionVersionNumber: " << std::dec << transactionVersionNumber);
			println("|\t| InputCount: " << inputCount);
			println("|\t|\t--------");
			for (uint64_t a = 0; a < inputCount; a++) {
				inputs[a].printInput();
				println("|\t|\t----------");
			}
			println("|\t| OutputCount: " << outputCount);
			println("|\t|\t--------");
			for (uint64_t a = 0; a < outputCount; a++) {
				outputs[a].printOutput();
				println("|\t|\t----------");
			}
			println("|\t| TransactionLockTime: " << transactionLockTime);
		}
	}Transaction;

	typedef struct Block {
		uint32_t magicID;
		uint32_t headerLength;
		uint32_t versionNumber;
		uint8_t* previousBlockHash;
		uint8_t* merkleRoot;
		uint32_t timeStamp;
		uint32_t targetDifficulty;
		uint32_t nonce;
		uint64_t transactionCount;
		Transaction* transactions;

		void printBlock() {
			println("----------");
			println("| MagicID: " << std::hex << magicID);
			println("| HeaderLength: " << std::dec << headerLength);
			println("| versionNumber: " << std::dec << versionNumber);
			print("| previousBlockHash: ");
			printHash(previousBlockHash);
			print("| markleRoot: ");
			printHash(merkleRoot);
			print("| Timestamp: ");
			printTimestamp(timeStamp);
			println("| targetDifficulty: " << std::hex << targetDifficulty);
			println("| nonce: " << std::dec << nonce);
			println("| transactionCount: " << std::dec << transactionCount);
			println("|\t--------");
			for (uint64_t a = 0; a < transactionCount; a++) {
				transactions[a].printTransaction();
				println("|\t--------");
			}
			println("----------");
		}
	}Block;

	static void printHash(uint8_t* hash) {
		for (int a = SHA256_LENGTH - 1; a >= 0; a--) {
			print(std::hex << (uint16_t)hash[a]);
		}
		delete[] hash;
		newline;
	}

	static void printTimestamp(uint32_t timestamp) {
		time_t t = (time_t)timestamp;
		struct tm time;
		localtime_s(&time, &t);
		print(std::dec << time.tm_mday << "/");
		print(std::dec << (1 + time.tm_mon) << "/");
		print(std::dec << (1900 + time.tm_year) << " ");
		println(std::dec << time.tm_hour << ":" << std::dec << time.tm_min << ":" << std::dec << time.tm_sec);
	}
}