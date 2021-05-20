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
}


void Console::run() {
  auto console = new Console();
  std::cout << "Fat File System Booting..." << std::endl;
  std::cout << "To get help about this system, please use \"useage\"."
            << std::endl;
  for (;;) {
    string con = fs.getDirName();
    std::cout << "[" << con << "/]"
              << "# ";
    std::getline(std::cin, con);
    Console::exec(con, *console);
  }
}

std::list<string> Console::split(string str) {
  std::regex ws_re("\\s+");
  std::list<string> v(
      std::sregex_token_iterator(str.begin(), str.end(), ws_re, -1),
      std::sregex_token_iterator());

  return v;
}

void Console::execFailed(string execName) {
  std::cout << execName << ": Call failed, please use \"usage\" to get help."
            << std::endl;
}

void Console::exec(string con, Console& console) {
  std::list<string> control = Console::split(con);
  auto&& i = control.begin();
  if (i->empty()) {
    control.pop_front();
    i = control.begin();
  }
  control.pop_front();
  if (*i == "ls" || *i == "exit" || *i == "pwd") {
    if (!control.empty()) {
      execFailed(*i);
      return;
    }
    (user.*(console.f0[*i]))();
  } else {
    if (control.empty() || control.size() > 2) {
      execFailed(*i);
      return;
    }
    if (control.size() == 1) {
      if (!console.f1.count(*i) || !console.fi.count(*i)) {
        execFailed(*i);
        return;
      }
      if (*i == "tree") {
        (user.*(console.fi[*i]))(std::stoi(*control.begin()));
      } else {
        (user.*(console.f1[*i]))(*control.begin());
      }
    } else if (control.size() == 2) {
      if (!console.f2.count(*i)) {
        execFailed(*i);
        return;
      }
      string para1 = *control.begin();
      control.pop_front();
      string para2 = *control.begin();
      (user.*(console.f2[*i]))(para1, para2);
    }
  }
}

void Console::useage() {
  std::cout << "COMMAND and SYNOPSIS:\n"
            << "\tls: list all files and directories;\n"
            << "\t\tuseage: ls\n"
            << "\tmkdir: create a directory;\n"
            << "\t\tuseage: mkdir dirName\n"
            << "\trmdir: remove a directory;\n"
            << "\t\tuseage: rmdir dirName\n"
            << "\trm: remove a file;\n"
            << "\t\tuseage: rm fileName\n"
            << "\tcd: change current path;\n"
            << "\t\tuseage: cd pathName\n"
            << "\tcat: concatenate and print file;\n"
            << "\t\tuseage: cat fileName [another fileName]\n"
            << "\techo: write arguments to the standard output or a file;\n"
            << "\t\tuseage: echo string [fileName]\n"
            << "\tfind: walk a file hierarchy;\n"
            << "\t\tuseage: find path fileName\n"
            << "\ttree: list contents of directories in a tree-like format;\n"
            << "\t\tuseage: tree [level(integer)]\n"
            << "\tpwd: return working directory name;\n"
            << "\t\tuseage: pwd\n"
            << "\texit: exit Fat File System.\n"
            << "\t\tuseage: exit\n";
}