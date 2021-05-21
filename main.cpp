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
	std::list<string> l3;
	l3.push_back("/a");
	l3.push_back("/zz");
	l3.push_back("/a.txt");
	l3.push_back("/a.txt");
	l3.push_back("/a/b/zzf");
	l3.push_back("/a/b/zzf");
	l3.push_back("/a/b/d/asdf");
	l3.push_back("/a/b/c/d/f/b/a");

	for (auto i : l3) {
		std::cout << i << ": " << (fs.createFile(i) ? "true" : "false") << std::endl
				  << std::endl;
	}

	// std::list<string> l4;
	// l4.push_back("/a");
	// // l4.push_back("/zz");
	// l4.push_back("/a.txt");
	// l4.push_back("/a.txt");
	// l4.push_back("/a/b/zzf");
	// l4.push_back("/a/b/zzf");
	// l4.push_back("/a/b/asdf");
	// // l4.push_back("/a/b/d/asdf");
	// l4.push_back("/a/b/c/d/f/b/a");

	// for (auto i : l4) {
	// 	std::cout << i << ": delete " << (fs.deleteFile(i) ? "true" : "false") << std::endl
	// 			  << std::endl;
	// }

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

	std::list<string> l5;
	l5.push_back("/a");
	l5.push_back("/a.txt");
	l5.push_back("/a/b/d/asdf");
	for (auto i : l5) {
		F_D fd = fs.open(i);
		Stat st;
		std::cout << i << ": fd: " << fd
				  << ": stat " << (fs.fstat(fd, &st) ? "true" : "false") << std::endl;
		fs.close(fd);
	}
	for (auto i : l5) {
		F_D fd = fs.open(i);
		Stat st;
		std::cout << i << ": fd: " << fd
				  << ": stat " << (fs.fstat(fd, &st) ? "true" : "false") << std::endl;
		fs.close(fd);
	}

	// std::cout << "/b"
	// 		  << ": delete " << (fs.deleteDir("/b") ? "true" : "false") << std::endl;
	// std::cout << "/a/b/c"
	// 		  << ": delete " << (fs.deleteDir("/a/b/c") ? "true" : "false") << std::endl;

	// for (auto i : l2) {
	// 	bool result;
	// 	std::cout << i << ": " << ((result = fs.openDir(i)) ? "true" : "false")
	// 			  << std::endl;
	// 	std::cout << "pwd: " << fs.getDirName() << std::endl;
	// 	std::cout << "ls: ";
	// 	for (auto i : (fs.getName().nameL)) {
	// 		switch (i.type) {
	// 		case T_dir:
	// 			std::cout << "DIR-" << i.name
	// 					  << " ";
	// 			break;
	// 		case T_file:
	// 			std::cout << "FILE-" << i.name
	// 					  << " ";
	// 			break;
	// 		default:
	// 			std::cout << "error"
	// 					  << " ";
	// 		}
	// 	}
	// 	std::cout << std::endl;
	// 	std::cout << std::endl;
	// }
	// fs.openDir("/");
	// std::cout << "final pwd: " << fs.getDirName() << std::endl
	// 		  << std::endl;
	// std::cout << "ls: ";
	// for (auto i : (fs.getName().nameL)) {
	// 	switch (i.type) {
	// 	case T_dir:
	// 		std::cout << "DIR-" << i.name
	// 				  << " ";
	// 		break;
	// 	case T_file:
	// 		std::cout << "FILE-" << i.name
	// 				  << " ";
	// 		break;
	// 	default:
	// 		std::cout << "error"
	// 				  << " ";
	// 	}
	// }
	// std::cout << std::endl;

	return 0;
}
