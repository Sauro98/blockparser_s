#pragma once

#include "globals.h"
#include <stdint.h>
#include <ctime>
#include<memory>
#include <fstream>

namespace BBlockChain {

	uint8_t publicAddressPattern[3] = { 0x76,0xA9,20 };

	static void printHash(std::shared_ptr<uint8_t> hash,std::ofstream* myFile);
	static void printTimestamp(uint32_t timestamp,std::ofstream* myFile);

	typedef struct Output {
		uint64_t value;
		uint64_t scriptLength;
		std::shared_ptr<uint8_t> script;

		void printOutput(std::ofstream* myFile) {
			(*myFile) << "|\t|\t| OutputValue: " << std::dec << value << std::endl;
			if (scriptLength == 67 || scriptLength == 66) {
				(*myFile) << "|\t|\t| Dest address(65): ";
				for (int a = 65; a > ((scriptLength == 67)?1:0); a--)
					(*myFile) << std::hex << (uint16_t)script.get()[a];
				(*myFile) << std::endl;
			} else if (scriptLength  >= 25) {
				(*myFile) << "|\t|\t| Dest address(25): ";
				bool isOk = true;
				for (int a = 0; a < 3; a++) {
					if (script.get()[a] != publicAddressPattern[a])
						isOk = false;
				}
				if (isOk) {
					for (int a = 3; a < 24; a++)
						(*myFile) << std::hex << (uint16_t)script.get()[a];
					(*myFile) << std::endl;
				}
			}
		}

	}Output;

	typedef struct Input {
		std::shared_ptr<uint8_t> transactionHash;
		uint32_t transactionIndex;
		uint64_t scriptLength;
		std::shared_ptr<uint8_t> script;
		uint32_t sequenceNumber;

		void printInput(std::ofstream* myFile) {
			(*myFile) << "|\t|\t| TransactionHash: ";
			printHash(transactionHash,myFile);
			(*myFile) << "|\t|\t| TransactionIndex: " << std::hex << transactionIndex << std::endl;
			(*myFile) << "|\t|\t| SequenceNumber: " << std::hex << sequenceNumber << std::endl;
		}

	}Input;

	typedef struct Transaction {
		uint32_t transactionVersionNumber;
		uint64_t inputCount;
		std::shared_ptr<Input> inputs;
		uint64_t outputCount;
		std::shared_ptr<Output> outputs;
		uint32_t transactionLockTime;

		void printTransaction(std::ofstream* myFile) {
			(*myFile) << "|\t| TransactionVersionNumber: " << std::dec << transactionVersionNumber << std::endl;
			(*myFile) << "|\t| InputCount: " << inputCount << std::endl;
			(*myFile) << "|\t|\t--------" << std::endl;
			for (uint64_t a = 0; a < inputCount; a++) {
				inputs.get()[a].printInput(myFile);
				(*myFile) << "|\t|\t----------" << std::endl;
			}
			(*myFile) << "|\t| OutputCount: " << outputCount << std::endl;
			(*myFile) << "|\t|\t--------" << std::endl;
			for (uint64_t a = 0; a < outputCount; a++) {
				outputs.get()[a].printOutput(myFile);
				(*myFile) << "|\t|\t----------" << std::endl;
			}
			(*myFile) << "|\t| TransactionLockTime: " << transactionLockTime << std::endl;
		}


	}Transaction;

	typedef struct Block {
		uint32_t magicID;
		uint32_t headerLength;
		uint32_t versionNumber;
		std::shared_ptr<uint8_t> previousBlockHash;
		std::shared_ptr<uint8_t> merkleRoot;
		uint32_t timeStamp;
		uint32_t targetDifficulty;
		uint32_t nonce;
		uint64_t transactionCount;
		std::shared_ptr<Transaction> transactions;

		void printBlockToFile(std::ofstream* myFile) {
			(*myFile) << "----------" <<std::endl;
			(*myFile) << "| MagicID: " << std::hex << magicID << std::endl;
			(*myFile) << "| HeaderLength: " << std::dec << headerLength << std::endl;
			(*myFile) << "| versionNumber: " << std::dec << versionNumber << std::endl;
			(*myFile) << "| previousBlockHash: ";
			printHash(previousBlockHash,myFile);
			(*myFile) << "| markleRoot: ";
			printHash(merkleRoot, myFile);
			(*myFile) << "| Timestamp: ";
			printTimestamp(timeStamp, myFile);
			(*myFile) << "| targetDifficulty: " << std::hex << targetDifficulty << std::endl;
			(*myFile) << "| nonce: " << std::dec << nonce << std::endl;
			(*myFile) << "| transactionCount: " << std::dec << transactionCount << std::endl;
			(*myFile) << "|\t--------" << std::endl;
			for (uint64_t a = 0; a < transactionCount; a++) {
				transactions.get()[a].printTransaction(myFile);
				(*myFile) << "|\t--------" << std::endl;
			}
			(*myFile) << "----------" << std::endl;
		}

	}Block;

	static void printHash(std::shared_ptr<uint8_t> hash,std::ofstream* myFile) {
		for (int a = 31; a >= 0; a--) {
			(*myFile) << std::hex << (uint16_t)(hash.get()[a]);
		}
		(*myFile) << std::endl;
	}

	static void printTimestamp(uint32_t timestamp,std::ofstream* myFile) {
		time_t t = (time_t)timestamp;
		struct tm time;
		localtime_s(&time, &t);
		(*myFile) << std::dec << time.tm_mday << "/";
		(*myFile) << std::dec << (1 + time.tm_mon) << "/";
		(*myFile) << std::dec << (1900 + time.tm_year) << " ";
		(*myFile) << std::dec << time.tm_hour << ":" << std::dec << time.tm_min << ":" << std::dec << time.tm_sec << std::endl;
	}
}