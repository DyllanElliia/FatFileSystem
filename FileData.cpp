#include "FileData.h"

FileContent::FileContent() {
	data.st.dev = 0;
	data.st.type = T_file_content;
	data.st.size = 0;
	data.data[0] = '\0';
}
FileContent::~FileContent() {}
// Son of Dir
bool FileContent::changeSonDirHead(Offset o) {
	return false;
}
Offset FileContent::getSonDirHead() {
	return -1;
}
// Son of File
bool FileContent::changeSonFileHead(Offset o) {
	return false;
}
Offset FileContent::getSonFileHead() {
	return -1;
}
// Data Transform
bool FileContent::Data2Block(void* data_ptr) {
	FileDataData* d = (FileDataData*)data_ptr;
	if (d->st.size > F_DATA_LEN_LIMIT)
		return false;
	data.st = d->st;
	for (int i = 0; i < d->st.size; ++i) {
		data.data[i] = d->data[i];
	}
	return true;
}
void* FileContent::Block2Data() {
	return &data;
}
// Get Sizeof Data
int FileContent::getDataSize() {
	return sizeof(FileDataData);
}
// for all
bool FileContent::changeBrother(Offset o) {
	data.brother = o;
	return true;
}
Offset FileContent::getBrotherOff() {
	return data.brother;
}
Stat FileContent::getStat() {
	return data.st;
}
bool FileContent::setStat(Stat st) {
	data.st = st;
	return true;
}
char* FileContent::getDataCharPtr() {
	return (char*)&data;
}
// for File AND Dir
Offset FileContent::getFatherOff() {
	return -1;
}
bool FileContent::changeFatherOff(Offset o) {
	return false;
}
std::string FileContent::getName() {
	return std::string("");
}
bool FileContent::setName(std::string name) {
	return false;
}
// for File
bool FileContent::changeStSize(int size) {
	data.st.size = size;
	return true;
}
Offset FileContent::getFileData() {
	return -1;
}
bool FileContent::changeFileData(Offset o) {
	return false;
}

char* FileContent::getDataptr() {
	return data.data;
}