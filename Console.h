#pragma once
#include "./User.h"

class Console {
public:
	Console();
	void run();

private:
	User* user = new User();
	std::map<string, func_ptr_0> f0;
	std::map<string, func_ptr_str> f1;
	std::map<string, func_ptr_str_str> f2;
	std::map<string, func_ptr_int> fi;

	void exec(string con, Console& console);
	void execFailed(string execName);
	void usage();
	std::list<string> split(string str);
};
