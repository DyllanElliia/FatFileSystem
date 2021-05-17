#include "Block.h"

class FileContent : public Block {
private:
	FileDataData data;

public:
	FileContent();
	~FileContent();
};