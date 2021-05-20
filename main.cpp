#include <iostream>

// #include "function.h"
#include "FileSystem.h"

int main(int argc, char const* argv[]) {
	FileSystem fs;
	std::list<string> l;
	l.push_back("/a");
	l.push_back("/a");
	l.push_back("/aa");
	l.push_back("/b");
	l.push_back("/b");
	l.push_back("/a/b");
	l.push_back("/a/b");
	l.push_back("/aa/a");
	l.push_back("/aa/a");
	l.push_back("/aa/bb");
	l.push_back("/aa/bb");
	l.push_back("/a/b/c");
	l.push_back("/a/b/c");
	l.push_back("/a/b/d");
	l.push_back("/a/b/d");
	l.push_back("/aa/b/d");
	l.push_back("/aa/b/d");
	l.push_back("/aa/bb/d");
	l.push_back("/aa/bb/d");
	l.push_back("/a/b/e");
	l.push_back("/a/b/c");
	l.push_back("/a/b/d");
	l.push_back("/a/b/c/d");
	l.push_back("/a/b/c/d");
	l.push_back("/a/b/c/d/f");
	l.push_back("/a/b/c/d/f");
	l.push_back("/a/b/d/d");
	l.push_back("/a/b/d/d");
	l.push_back("/a/b/c/d/f/b/a");

	for (auto i : l) {
		std::cout << i << ": " << (fs.createDir(i) ? "true" : "false") << std::endl
				  << std::endl;
	}

	std::list<string> l2;
	l2.push_back("/a");
	l2.push_back("/b");
	l2.push_back("/c");
	l2.push_back("/a/b");
	l2.push_back("/a/a");
	l2.push_back("/a/b/c");
	l2.push_back("/a/b/d");
	l2.push_back("/a/b/e");
	l2.push_back("/a/b/c");
	l2.push_back("/a/b/d");
	l2.push_back("/a/b/g");
	l2.push_back("/a/c/c");
	l2.push_back("/a/b/c/d");
	l2.push_back("/a/b/c/d");
	l2.push_back("/a/b/c/d/f");
	l2.push_back("/a/b/d/d");
	l2.push_back("/a/b/c/d/f/b/a");
	l2.push_back("/aa");
	l2.push_back("/aa/bb");

	std::cout << "first pwd: " << fs.getDirName() << std::endl
			  << std::endl;

	for (auto i : l2) {
		bool result;
		std::cout << i << ": " << ((result = fs.openDir(i)) ? "true" : "false")
				  << std::endl;
		std::cout << "pwd: " << fs.getDirName() << std::endl;
		std::cout << "ls: ";
		for (auto i : (fs.getName().nameL)) {
			switch (i.type) {
			case T_dir:
				std::cout << "DIR-" << i.name
						  << " ";
				break;
			case T_file:
				std::cout << "FILE-" << i.name
						  << " ";
				break;
			default:
				std::cout << "error"
						  << " ";
			}
		}
		std::cout << std::endl;
		std::cout << std::endl;
	}
	fs.openDir("/");
	std::cout << "final pwd: " << fs.getDirName() << std::endl
			  << std::endl;
	std::cout << "ls: ";
	for (auto i : (fs.getName().nameL)) {
		switch (i.type) {
		case T_dir:
			std::cout << "DIR-" << i.name
					  << " ";
			break;
		case T_file:
			std::cout << "FILE-" << i.name
					  << " ";
			break;
		default:
			std::cout << "error"
					  << " ";
		}
	}
	std::cout << std::endl;
	return 0;
}
