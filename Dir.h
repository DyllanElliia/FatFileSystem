#pragma once
#include "Block.h"

class Dir : public Block {
private:
	DirData data;

public:
	Dir();
	~Dir();
	// Son of Dir
	bool changeSonDirHead(Offset o);
	Offset getSonDirHead();
	// Son of File
	bool changeSonFileHead(Offset o);
	Offset getSonFileHead();
	// Data Transform
	bool Data2Block(void* data_ptr);
	void* Block2Data();
	// Get Sizeof Data
	int getDataSize();
	// for all
	bool changeBrother(Offset o);
	Offset getBrotherOff();
	Stat getStat();
	bool setStat(Stat st);
	char* getDataCharPtr();
	// for File AND Dir
	Offset getFatherOff();
	bool changeFatherOff(Offset o);
	std::string getName();
	bool setName(std::string name);
	// for File
	bool changeStSize(int size);
	Offset getFileData();
	bool changeFileData(Offset o);
	// for FileContent
	char* getDataptr();
};