#pragma once
#include <fstream>
#include <iostream>
#include <string>

#include "Block.h"
#include "Dir.h"
#include "File.h"
#include "FileData.h"

using F_D = int;
using W_LEN = int;
using R_LEN = int;
using W_RET = int;
using R_RET = int;
using string = std::string;

class FileOptor {
private:
	struct head_data {
		char systemName[20];
		char producerName[20];
		char vendorName[20];
		int emptyFatBlockNumber;
		int emptyBlockNumber;
	};
	std::fstream f;
	Offset f_offset = 0;
	struct head_data HeadData_;
	inline void move2offset_short(Offset o);

public:
#define HEAD_SIZE 128
#define FAT_BLOCK_NUM 16
#define FAT1_SIZE 1024
#define FAT2_SIZE 1024
#define BLOCK_SIZE 64
#define BLOCK_NUM 128
	FileOptor();
	~FileOptor();

	inline void move2offset(Offset o);
	inline Offset getOffset();

	bool readData(Block* b, int len);
	bool saveData(Block* b, int len);

	Offset findEmptyBlock();
	Offset findEmptyFatBlock();
	bool cleanBlock(Offset o);
	bool cleanFatBlock(Offset o);
};