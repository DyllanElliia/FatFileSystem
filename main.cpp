#include <iostream>

// #include "function.h"
#include "FileSystem.h"

int main(int argc, char const* argv[]) {
	FileSystem fs;
	std::list<string> l;
	l.push_back("/a");
	l.push_back("/a");
	l.push_back("/b");
	l.push_back("/b");
	l.push_back("/a/b");
	l.push_back("/a/b");
	l.push_back("/a/b/c");
	l.push_back("/a/b/c");
	l.push_back("/a/b/c");
	l.push_back("/a/b/c/d");
	l.push_back("/a/b/c/d");
	l.push_back("/a/b/c/d/f");
	l.push_back("/a/b/c/d/f");
	l.push_back("/a/b/c/d/f/b/a");

	for (auto i : l) {
		std::cout << i << ": " << (fs.createDir(i) ? "true" : "false") << std::endl
				  << std::endl;
	}
	return 0;
}
