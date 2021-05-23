#include "FileOptor.h"

#include <ostream>

FileOptor::FileOptor() {
	// std::ofstream of("./FAT_DATA", std::ios::out);
	// of.close();

	f.open("./FAT_DATA", std::ios::in | std::ios::out | std::ios::binary);
	// if (!f.is_open()) {
	// 	std::cout << "ERROW! Failure to open file "
	// 			  << "./FAT_DATA" << std::endl;
	// 	exit(EXIT_FAILURE);
	// }
	if (!f.is_open()) {
		std::ofstream of("./FAT_DATA", std::ios::out | std::ios::binary);
		of.close();

		f.open("./FAT_DATA", std::ios::in | std::ios::out | std::ios::binary);
		std::cout << "-----------Initializing FAT_DATA-----------" << std::endl;
		struct head_data head_save;
		strcpy(head_save.systemName, "FAT-Based File System");
		strcpy(head_save.producerName, "Dyllan & Taka");
		strcpy(head_save.vendorName, "UESTC");
		head_save.emptyFatBlockNumber = FAT_BLOCK_NUM;
		head_save.emptyBlockNumber = BLOCK_NUM;
		f.write((char*)&head_save, sizeof(struct head_data));
		// std::cout << f.tellg() << " " << f.tellp() << std::endl;
		int* buf = new int[(FAT1_SIZE + FAT2_SIZE + BLOCK_NUM * BLOCK_SIZE) * 2];
		memset(buf, 0, sizeof(FAT1_SIZE + FAT2_SIZE + BLOCK_NUM * BLOCK_SIZE) * sizeof(int) * 2);
		f.write((char*)buf, (FAT1_SIZE + FAT2_SIZE + BLOCK_NUM * BLOCK_SIZE) * sizeof(char) * 2);
		std::cout << f.tellg() << " " << f.tellp() << std::endl;
		f.seekp(0, std::ios::beg);
		f.seekg(0, std::ios::beg);
		delete[] buf;
	}
	// std::cout << f.tellg() << " " << f.tellp() << std::endl;
	std::cout << "----------------System Data----------------" << std::endl;
	f.read((char*)&HeadData_, sizeof(struct head_data));
	std::cout << "System Name: " << HeadData_.systemName << std::endl;
	std::cout << "Producers' Name: " << HeadData_.producerName << std::endl;
	std::cout << "Vendor Name: " << HeadData_.vendorName << std::endl;
	std::cout << "Fat Space (64B/64B): " << HeadData_.emptyFatBlockNumber << "/" << FAT_BLOCK_NUM << std::endl;
	std::cout << "Memory Space (64B/64B): " << HeadData_.emptyBlockNumber << "/" << BLOCK_NUM << std::endl;
	std::cout << "---------------System Running--------------" << std::endl;
	// std::cout << f.tellg() << " " << f.tellp() << std::endl;
	move2offset_short(0);
	// std::cout << f.tellg() << " " << f.tellp() << std::endl;
}
FileOptor::~FileOptor() {
	// std::cout << f.tellg() << " " << f.tellp() << std::endl;
	f.seekp(0, std::ios::beg);
	f.seekg(0, std::ios::beg);
	f.write((char*)&HeadData_, sizeof(struct head_data));
	// std::cout << f.tellg() << " " << f.tellp() << std::endl;
	f.close();
	std::cout << "----------------System Exit----------------" << std::endl;
}

inline Offset FileOptor::Fat1BlockBegin() {
	return HEAD_SIZE;
}
inline Offset FileOptor::BlockBegin() {
	return HEAD_SIZE + FAT1_SIZE + FAT2_SIZE;
}

inline bool FileOptor::getLastFindBool() {
	if (lastFindResult) {
		lastFindResult = false;
		return true;
	}
	return false;
}

inline void FileOptor::move2offset_short(Offset o) {
	// std::cout << f.tellg() << " " << f.tellp() << " " << o << std::endl;
	f.seekg(o * sizeof(char), std::ios::beg);
	f.seekp(o * sizeof(char), std::ios::beg);
	// std::cout << f.tellg() << " " << f.tellp() << std::endl;
}
inline void FileOptor::move2offset(Offset o) {
	f_offset = o * sizeof(char);
	f.seekg(f_offset, std::ios::beg);
	f.seekp(f_offset, std::ios::beg);
}
inline Offset FileOptor::getOffset() {
	return f_offset;
}

bool FileOptor::readData(char* b, int len) {
	f.read(b, len);
	move2offset(f_offset);
	return true;
}
bool FileOptor::saveData(char* b, int len) {
	// std::cout << f.tellg() << " " << f.tellp() << " " << len << std::endl;
	f.write(b, len);
	move2offset(f_offset);
	return true;
}

Offset FileOptor::findEmptyBlock(int times) {
	if (lastFindResult)
		getLastFindBool();
	Offset blockOffset = HEAD_SIZE + FAT1_SIZE + FAT2_SIZE;
	move2offset_short(blockOffset);
	if (HeadData_.emptyBlockNumber == 0)
		return -1;
	for (int i = 0; i < BLOCK_NUM; ++i) {
		char blockData[BLOCK_SIZE];
		f.read(blockData, BLOCK_SIZE * sizeof(char));
		if (((DirData*)blockData)->st.dev == 0) {
			if (times > 0) {
				times--;
			} else {
				blockOffset += BLOCK_SIZE * i;
				lastFindResult = true;
				// std::cout << "find" << std::endl;
				break;
			}
		}
	}
	move2offset_short(f_offset);
	if (lastFindResult)
		HeadData_.emptyBlockNumber--;
	return blockOffset;
}
Offset FileOptor::findEmptyFatBlock() {
	if (lastFindResult)
		getLastFindBool();
	Offset blockOffset = HEAD_SIZE;
	move2offset_short(blockOffset);
	if (HeadData_.emptyFatBlockNumber == 0)
		return -1;
	for (int i = 0; i < FAT_BLOCK_NUM; ++i) {
		char blockData[BLOCK_SIZE];
		f.read(blockData, BLOCK_SIZE * sizeof(char));
		if (((DirData*)blockData)->st.dev == 0) {
			blockOffset += BLOCK_SIZE * i;
			lastFindResult = true;
			// std::cout << "find" << std::endl;
			break;
		}
	}
	move2offset_short(f_offset);
	if (lastFindResult)
		HeadData_.emptyFatBlockNumber--;

	return blockOffset;
}

// Offset FileOptor::findBlock(S_type type, string name) {
// 	if (lastFindResult)
// 		getLastFindBool();
// 	Offset blockOffset = HEAD_SIZE + FAT1_SIZE + FAT2_SIZE;
// 	move2offset_short(blockOffset);
// 	if (HeadData_.emptyBlockNumber == 0)
// 		return -1;
// 	for (int i = 0; i < BLOCK_NUM; ++i) {
// 		char blockData[BLOCK_SIZE];
// 		f.read(blockData, BLOCK_SIZE);
// 		if (((DirData*)blockData)->st.type == type) {
// 			if (name.compare(string(((DirData*)blockData)->name)) == 0) {
// 				blockOffset += BLOCK_SIZE * i;
// 				lastFindResult = true;
// 				break;
// 			}
// 		}
// 	}
// 	move2offset_short(f_offset);
// 	return blockOffset;
// }
Offset FileOptor::findFat1Block(S_type type, string name) {
	if (lastFindResult)
		getLastFindBool();
	Offset blockOffset = HEAD_SIZE;
	move2offset_short(blockOffset);
	for (int i = 0; i < FAT_BLOCK_NUM; ++i) {
		char blockData[BLOCK_SIZE_DOU];
		f.read(blockData, BLOCK_SIZE * sizeof(char));
		// std::cout << f.tellg() << " " << f.tellp() << std::endl;
		// std::cout << ((DirData*)blockData)->st.dev << std::endl;
		if (((DirData*)blockData)->st.type == type) {
			// std::cout << string(((DirData*)blockData)->name) << std::endl;
			if (name.compare(string(((DirData*)blockData)->name)) == 0) {
				Offset Fat1_off = blockOffset + BLOCK_SIZE * i;
				Offset Fat2_off = Fat1_off + FAT1_SIZE;
				move2offset_short(Fat2_off);
				f.read(blockData, BLOCK_SIZE * sizeof(char));
				if (((DirData*)blockData)->st.type == type) {
					if (name.compare(string(((DirData*)blockData)->name)) == 0) {
						blockOffset = Fat1_off;
						lastFindResult = true;
						break;
					}
				}
				cleanFatBlock(Fat1_off);
				move2offset_short(Fat1_off + BLOCK_SIZE);
			}
		}
	}
	move2offset_short(f_offset);
	return blockOffset;
}

bool FileOptor::cleanBlock(Offset o) {
	Offset blockOffset = o;
	move2offset_short(blockOffset);
	DirData blockData;
	f.write((char*)&blockData, sizeof(DirData));
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