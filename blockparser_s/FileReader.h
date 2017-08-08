#pragma once

#include<stdio.h>
#include "globals.h"

namespace FileReader {
	static uint8_t* getFileContent(char* fileName, size_t* fileLength) {
		FILE* file;
		errno_t fileError = fopen_s(&file,fileName, "rb");
		if (fileError != 0) {
			println("File not found");
			exit(ERR_CODE_NO_FILE);
		}
		// obtain file size:
		long fileSize;
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		rewind(file);
		//read content
		uint8_t* fileContent = new uint8_t[fileSize];
		size_t bytesRead = fread(fileContent, 1, fileSize, file);
		if (bytesRead != fileSize) {
			println("Reading error: ("<< bytesRead<<"/"<<fileSize<<")");
			exit(ERR_CODE_READ_ERROR);
		}
		fclose(file);
		*fileLength = fileSize;
		return fileContent;
	}
}