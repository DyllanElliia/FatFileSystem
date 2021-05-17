using Offset = int;
using S_type = int;

#define T_empty_block 0
#define T_file 1
#define T_dir 2

struct stat {
	S_type type;
	Offset dev;
	int size;
};

using Stat = struct stat;