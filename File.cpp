#include "File.h"

File::File() {
	data.st.dev = 0;
	data.st.type = T_file;
	data.st.size = 0;
	data.name[0] = '\0';
}
File::~File() {}
// Son of Dir
bool File::changeSonDirHead(Offset o) {
	return false;
}
Offset File::getSonDirHead() {
	return -1;
}
// Son of File
bool File::changeSonFileHead(Offset o) {
	return false;
}
Offset File::getSonFileHead() {
	return -1;
}
// Data Transform
bool File::Data2Block(void* data_ptr) {
	FileData* d = (FileData*)data_ptr;
	data.st = d->st;
	strcpy(data.name, d->name);
	data.father = d->father;
	data.brother = d->brother;
	data.message = d->message;
	return true;
}
void* File::Block2Data() {
	return &data;
}
// Get Sizeof Data
int File::getDataSize() {
	return sizeof(FileData);
}
// for all
bool File::changeBrother(Offset o) {
	data.brother = o;
	return true;
}
Offset File::getBrotherOff() {
	return data.brother;
}
Stat File::getStat() {
	return data.st;
}
bool File::setStat(Stat st) {
	data.st = st;
	return true;
}
char* File::getDataCharPtr() {
	return (char*)&data;
}
// for File AND Dir
Offset File::getFatherOff() {
	return data.father;
}
bool File::changeFatherOff(Offset o) {
	data.father = o;
	return true;
}
std::string File::getName() {
	return std::string(data.name);
}
bool File::setName(std::string name) {
	strcpy(data.name, name.c_str());
	return true;
}
// for File
bool File::changeStSize(int size) {
	data.st.size = size;
	return true;
}
Offset File::getFileData() {
	return data.message;
}