#pragma once
using Offset = int;
using S_type = int;

#define T_file S_type(1)
#define T_dir S_type(2)
#define T_file_content S_type(3)

class Stat {
public:
	Offset dev;
	S_type type;
	int size;
};
