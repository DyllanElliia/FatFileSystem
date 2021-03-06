#include "./Console.h"

#include <cstring>
#include <ostream>
#include <regex>

Console::Console() {
	f0["ls"] = &User::ls;
	f0["exit"] = &User::exit;
	f0["pwd"] = &User::pwd;
	f1["mkdir"] = &User::mkdir;
	f1["rmdir"] = &User::rmdir;
	f1["rm"] = &User::rm;
	f1["cd"] = &User::cd;
	f1["cat"] = &User::cat;
	f1["echo"] = &User::echo;
	fi["tree"] = &User::tree;
	f2["cat"] = &User::cat;
	f2["echo"] = &User::echo;
	f2["find"] = &User::find;
	std::cout << "Console init over\n";
}

void Console::run() {
	// auto console = new Console();

	std::cout << "Fat File System Booting..." << std::endl;
	std::cout << "To get help about this system, please use \"usage\"."
			  << std::endl;
	FileSystem& fs = user->getFs();
	for (;;) {
		string con = fs.getDirName();
		std::cout << "[" << con << "]"
				  << " # ";
		std::getline(std::cin, con);
		if (con == "usage") {
			this->usage();
		} else {
			Console::exec(con, *this);
		}
	}
}

std::list<string> Console::split(string str) {
	// std::regex ws_re("\\s+");
	// std::list<string> v(
	//     std::sregex_token_iterator(str.begin(), str.end(), ws_re, -1),
	//     std::sregex_token_iterator());
	std::list<string> v;
	string temp;
	int size = str.size();
	bool quo = false;
	for (int i = 0; i < size; ++i) {
		if (str[i] == '"' && quo == true) {
			quo = false;
			v.push_back(temp);
			temp.clear();
			continue;
		} else if (str[i] == '"' && quo == false) {
			quo = true;
			continue;
		} else if ((str[i] != '"' && quo == true) || str[i] != ' ') {
			temp += str[i];
		} else {
			v.push_back(temp);
			temp.clear();
		}
	}
	v.push_back(temp);

	for (auto i = v.begin(); i != v.end();) {
		if ((*i).size() == 0) {
			i = v.erase(i);
		} else {
			++i;
		}
	}
	if (quo == true) {
		std::cout << "there is a single quote, please check your input."
				  << std::endl;
		v.clear();
	}
	return v;
}

void Console::execFailed(string execName) {
	std::cout << execName << ": Call failed, please use \"usage\" to get help."
			  << std::endl;
}

void Console::exec(string con, Console& console) {
	std::list<string> control = Console::split(con);
	if (control.empty()) {
		return;
	}
	string j = *control.begin();
	control.pop_front();
	if (j == "ls" || j == "exit" || j == "pwd") {
		if (!control.empty()) {
			execFailed(j);
			return;
		}
		(user->*(console.f0[j]))();
		// std::cout << "exec 0 over;\n";
	} else {
		if (control.empty() || control.size() > 2) {
			execFailed(j);
			return;
		}
		if (control.size() == 1) {
			if (!console.f1.count(j) && !console.fi.count(j)) {
				execFailed(j);
				return;
			}
			if (j == "tree") {
				(user->*(console.fi[j]))(std::stoi(*control.begin()));
				// std::cout << "exec i over;\n";
			} else {
				(user->*(console.f1[j]))(*control.begin());
				// std::cout << "exec 1 over;\n";
			}
		} else if (control.size() == 2) {
			if (!console.f2.count(j)) {
				execFailed(j);
				return;
			}
			string para1 = *control.begin();
			control.pop_front();
			string para2 = *control.begin();
			// std::cout << para1.size() << "\t" << para2.size() << std::endl;
			//   std::cout << j << std::endl;
			(user->*(console.f2[j]))(para1, para2);
			// std::cout << "exec 2 over;\n";
		}
	}
}

void Console::usage() {
	std::cout << "COMMAND and SYNOPSIS:\n"
			  << "\tls: list all files and directories;\n"
			  << "\t\tusage: ls\n"
			  << "\tmkdir: create a directory;\n"
			  << "\t\tusage: mkdir dirName\n"
			  << "\trmdir: remove a directory;\n"
			  << "\t\tusage: rmdir dirName\n"
			  << "\trm: remove a file;\n"
			  << "\t\tusage: rm fileName\n"
			  << "\tcd: change current path;\n"
			  << "\t\tusage: cd pathName\n"
			  << "\tcat: concatenate and print file;\n"
			  << "\t\tusage: cat fileName [another fileName]\n"
			  << "\techo: write arguments to the standard output or a file;\n"
			  << "\t\tusage: echo string [fileName]\n"
			  << "\tfind: walk a file hierarchy;\n"
			  << "\t\tusage: find path fileName\n"
			  << "\ttree: list contents of directories in a tree-like format;\n"
			  << "\t\tusage: tree [level(integer)]\n"
			  << "\tpwd: return working directory name;\n"
			  << "\t\tusage: pwd\n"
			  << "\texit: exit Fat File System.\n"
			  << "\t\tusage: exit\n";
}