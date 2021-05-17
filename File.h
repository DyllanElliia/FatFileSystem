#include "Block.h"

class File : public Block {
private:
	FileData data;

public:
	File();
	~File();
};