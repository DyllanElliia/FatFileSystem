#pragma once
#include <string>

#include "stat.h"

class DirData {
public:
	Stat st;
	char name[32];
	Offset sonDir = 0, sonFile = 0;
	Offset father = 0, brother = 0;
};
// using DirData_ptr = DirData*;
// using DirData_use = DirData&;

class FileData {
public:
	Stat st;
	char name[32];
	Offset father = 0, brother = 0, message = 0;
};

#define F_DATA_LEN_LIMIT 42
class FileDataData {
public:
	Stat st;
	char data[42];
	Offset brother = 0;
};
