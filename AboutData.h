#include <string>

#include "stat.h"
struct DirData {
	Stat st;
	char name[32];
	Offset NextDir = 0, NextFile = 0;
	Offset father, brother;
};
using DirData = struct DirData;

struct FileData {
	Stat st;
	char name[32];
	int size;
	Offset father, brother;
};
using FileData = struct FileData;

#define F_DATA_LEN_LIMIT 42
struct FileDataData {
	Stat st;
	char data[42];
	Offset brother;
};
using FileDataData = struct FileDataData;