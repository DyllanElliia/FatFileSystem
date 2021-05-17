#include "Block.h"

class Dir : public Block {
private:
	DirData data;

public:
	Dir();
	~Dir();
};