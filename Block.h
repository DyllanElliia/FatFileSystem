#pragma once
#include <cstdio>
#include <cstring>
#include <string>

#include "AboutData.h"
// #include "function.cpp"

class Block {
public:
	Block() {}
	~Block() {}
	// Son of Dir
	virtual bool changeSonDirHead(Offset o) = 0;
	virtual Offset getSonDirHead(Offset o) = 0;
	// Son of File
	virtual bool changeSonFileHead(Offset o) = 0;
	virtual Offset getSonFileHead(Offset o) = 0;
	// Data Transform
	virtual bool Data2Block(void* data_ptr) = 0;
	virtual void* Block2Data() = 0;
	// Get Sizeof Data
	virtual int getDataSize() = 0;
	// for all
	virtual bool changeBrother(Offset o) = 0;
	virtual Offset getBrotherOff() = 0;
	virtual Stat getStat() = 0;
	virtual bool setStat(Stat st) = 0;
	// for File AND Dir
	virtual Offset getFatherOff() = 0;
	virtual bool setName(std::string name) = 0;
	virtual std::string getName() = 0;
	// for File
	virtual Offset getFileData() = 0;
	// for File AND FileContent
	virtual bool changeStSize(int size) = 0;
};