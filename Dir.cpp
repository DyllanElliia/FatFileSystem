#include "Dir.h"

Dir::Dir() : Block() {
	data.st.dev = 0;
	data.st.type = T_dir;
	data.st.size = 0;
	data.name[0] = '\0';
}
Dir::~Dir() {}
// Son of Dir
bool Dir::changeSonDirHead(Offset o) {
	data.sonDir = o;
	return true;
}
Offset Dir::getSonDirHead() {
	return data.sonDir;
}
// Son of File
bool Dir::changeSonFileHead(Offset o) {
	data.sonFile = o;
	return true;
}
Offset Dir::getSonFileHead() {
	return data.sonFile;
}
// Data Transform
bool Dir::Data2Block(void* data_ptr) {
	DirData* d = (DirData*)data_ptr;
	data.st = d->st;
	strcpy(data.name, d->name);
	data.sonDir = d->sonFile;
	data.sonFile = d->sonFile;
	data.father = d->father;
	data.brother = d->brother;
	return true;
}
void* Dir::Block2Data() {
	return &data;
}
// Get Sizeof Data
int Dir::getDataSize() {
	return sizeof(DirData);
}
// for all
bool Dir::changeBrother(Offset o) {
	data.brother = o;
	return true;
}
Offset Dir::getBrotherOff() {
	return data.brother;
}
Stat Dir::getStat() {
	return data.st;
}
bool Dir::setStat(Stat st) {
	data.st = st;
	return true;
}
char* Dir::getDataCharPtr() {
	return (char*)&data;
}

// for File AND Dir
Offset Dir::getFatherOff() {
	return data.father;
}
bool Dir::changeFatherOff(Offset o) {
	data.father = o;
	return true;
}

std::string Dir::getName() {
	return std::string(data.name);
}
bool Dir::setName(std::string name) {
	strcpy(data.name, name.c_str());
	return true;
}
// for File
bool Dir::changeStSize(int size) {
	return false;
}
Offset Dir::getFileData() {
	return -1;
}
bool Dir::changeFileData(Offset o) {
	return false;
}

char* Dir::getDataptr() {
	return nullptr;
}