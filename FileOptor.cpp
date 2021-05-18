#include "FileOptor.h"

FileOptor::FileOptor() {
	f.open("./FAT_DATA", std::ios::in | std::ios::out | std::ios::binary);
	if (!f.is_open()) {
		std::cout << "ERROW! Failure to open file "
				  << "./FAT_DATA" << std::endl;
		exit(EXIT_FAILURE);
	}
	if (f.eof()) {
		std::cout << "-----------Initializing FAT_DATA-----------" << std::endl;
		struct head_data head_save;
		strcpy(head_save.systemName, "FAT-Based File System");
		strcpy(head_save.producerName, "Dyllan & Taka");
		strcpy(head_save.vendorName, "UESTC");
		head_save.emptyFatBlockNumber = FAT_BLOCK_NUM;
		head_save.emptyBlockNumber = BLOCK_NUM;
		f.write((char*)&head_save, sizeof(struct head_data));
		char* buf = new char[FAT1_SIZE + FAT2_SIZE + BLOCK_NUM * BLOCK_SIZE];
		f.write(buf, sizeof(FAT1_SIZE + FAT2_SIZE + BLOCK_NUM * BLOCK_SIZE) * sizeof(char));
		f.seekp(0, std::ios::beg);
	}
	std::cout << "----------------System Data----------------" << std::endl;
	f.read((char*)&HeadData_, sizeof(struct head_data));
	std::cout << "System Name: " << HeadData_.systemName << std::endl;
	std::cout << "Producers' Name: " << HeadData_.producerName << std::endl;
	std::cout << "Vendor Name: " << HeadData_.vendorName << std::endl;
	std::cout << "Fat Space (64B/64B): " << HeadData_.emptyFatBlockNumber << "/" << FAT_BLOCK_NUM << std::endl;
	std::cout << "Memory Space (64B/64B): " << HeadData_.emptyBlockNumber << "/" << BLOCK_NUM << std::endl;
	std::cout << "---------------System Running--------------" << std::endl;
}
FileOptor::~FileOptor() {
	f.seekp(0, std::ios::beg);
	f.seekg(0, std::ios::beg);
	f.write((char*)&HeadData_, sizeof(struct head_data));
	f.close();
	std::cout << "----------------System Exit----------------" << std::endl;
}
inline void FileOptor::move2offset_short(Offset o) {
	f.seekg(o, std::ios::beg);
	f.seekp(o, std::ios::beg);
}
inline void FileOptor::move2offset(Offset o) {
	f_offset = o;
	f.seekg(f_offset, std::ios::beg);
	f.seekp(f_offset, std::ios::beg);
}
inline Offset FileOptor::getOffset() {
	return f_offset;
}

bool FileOptor::readData(Block* b, int len) {
	f.read((char*)b, len);
	move2offset(f_offset);
	return true;
}
bool FileOptor::saveData(Block* b, int len) {
	f.write((char*)&b, len);
	move2offset(f_offset);
	return true;
}

Offset FileOptor::findEmptyBlock() {
	Offset blockOffset = HEAD_SIZE + FAT1_SIZE + FAT2_SIZE;
	move2offset_short(blockOffset);
	if (HeadData_.emptyBlockNumber == 0)
		return -1;
	for (int i = 0; i < BLOCK_NUM; ++i) {
		char blockData[BLOCK_SIZE];
		f.read(blockData, BLOCK_SIZE);
		if (((DirData*)blockData)->st.dev == 0) {
			blockOffset += BLOCK_SIZE * i;
			break;
		}
	}
	move2offset_short(f_offset);
	HeadData_.emptyBlockNumber--;
	return blockOffset;
}
Offset FileOptor::findEmptyFatBlock() {
	Offset blockOffset = HEAD_SIZE;
	move2offset_short(blockOffset);
	if (HeadData_.emptyFatBlockNumber == 0)
		return -1;
	for (int i = 0; i < FAT_BLOCK_NUM; ++i) {
		char blockData[BLOCK_SIZE];
		f.read(blockData, BLOCK_SIZE);
		if (((DirData*)blockData)->st.dev == 0) {
			blockOffset += BLOCK_SIZE * i;
			break;
		}
	}
	move2offset_short(f_offset);
	HeadData_.emptyFatBlockNumber--;
	return blockOffset;
}
bool FileOptor::cleanBlock(Offset o) {
	Offset blockOffset = o;
	move2offset_short(blockOffset);
	char blockData[BLOCK_SIZE];
	f.write(blockData, BLOCK_SIZE);
	move2offset_short(f_offset);
	return true;
}

bool FileOptor::cleanFatBlock(Offset FAT1_o) {
	Offset Fat1_of = FAT1_o;
	Offset Fat2_of = Fat1_of + FAT1_SIZE;
	if (cleanBlock(Fat1_of) &&
		cleanBlock(Fat2_of))
		return true;
	else
		return false;
}