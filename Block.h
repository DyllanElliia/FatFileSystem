#include <string>

#include "AboutData.h"
#include "stat.h"

#define BLOCK_SIZE 64

class Block {
public:
	Block() {}
	~Block() {}
	virtual bool changeSonFileHead(Offset o) = 0;
	virtual bool changeSonDirHead(Offset o) = 0;
	virtual bool getSonFileHead(Offset o) = 0;
	virtual bool getSonDirHead(Offset o) = 0;
	virtual bool Data2Block(void* data) = 0;
	virtual void* Block2Data() = 0;
	virtual int getDataSize() = 0;
	virtual bool changeBrother(Offset o) = 0;
	virtual Offset getFatherOff() = 0;
	virtual Offset getBrotherOff() = 0;
	virtual std::string getName() = 0;
	virtual bool changeStSize(int size) = 0;
	virtual std::string getFileData() = 0;
};