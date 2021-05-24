#include "./User.h"

#include <cstring>
#include <iostream>
#include <regex>

FileSystem& User::getFs() { return this->fs; }

string User::fmtName(string name) {
  string fmtName;
  if (name[0] == '/') {
    std::list<string> ls = User::split(name);
    std::list<string> st;
    for (auto&& i : ls) {
      if (i != ".." && i != ".") {
        st.push_back(i);
      } else if (i == ".." && !st.empty()) {
        st.pop_back();
      }
    }
    if (st.empty()) {
      fmtName.push_back('/');
      return fmtName;
    }
    for (auto&& i : st) {
      fmtName.push_back('/');
      fmtName += i;
    }
    return fmtName;
  } else {
    string n = fs.getDirName();
    if (n != "/") {
      n.push_back('/');
    }
    n += name;
    fmtName = User::fmtName(n);
    return fmtName;
  }
}

std::list<string> User::split(string str) {
  std::regex ws_re("/");
  std::list<string> v(
      std::sregex_token_iterator(str.begin(), str.end(), ws_re, -1),
      std::sregex_token_iterator());

  v.pop_front();
  return v;
}

void User::mkdir(string dirName) {
  string dir = this->fmtName(dirName);
  F_D fd = fs.open(dir);
  if (fd != -1) {
    std::cout << "mkdir: " << dir << ": File exists" << std::endl;
    fs.close(fd);
    return;
  }
  bool result = fs.createDir(dir);
  if (!result) {
    std::cout << "Error: Cannot create dir " << dir << std::endl;
  }
}

void User::rmdir(string dirName) {
  F_D fd;

  string dir = this->fmtName(dirName);
  if ((fd = fs.open(dir)) < 0) {
    std::cout << "rmdir: Cannot find dir " << dir << std::endl;
  }
  Stat stat;
  if (!fs.fstat(fd, &stat)) {
    std::cout << "rmdir: Cannot stat dir " << dir << std::endl;
    fs.close(fd);
    return;
  }
  if (stat.type != T_dir) {
    std::cout << "rmdir: " << dir << " is not a dir" << std::endl;
    fs.close(fd);
    return;
  }
  fs.close(fd);
  if (!fs.deleteDir(dir)) {
    std::cout << "rmdir: error: delete dir " << dir << " failed" << std::endl;
  }
}

void User::rm(string fileName) {
  string file = this->fmtName(fileName);
  int fd = fs.open(file);
  if (fd < 0) {
    std::cout << "rm: file " << file << " not exists" << std::endl;
    return;
  }
  fs.close(fd);
  bool result = fs.deleteFile(file);
  if (!result) {
    std::cout << "rm: delete file " << file << "failed" << std::endl;
  }
}

void User::ls() {
  FileSystem::nameList nl = fs.getName();
  std::cout << "name\ttype\tsize" << std::endl;
  for (auto i : nl.nameL) {
    string name = this->fmtName(i.name);
    F_D fd = fs.open(name);
    if (fd < 0) {
      std::cout << "ls: Cannot open file " << name << std::endl;
      continue;
    }
    Stat stat;
    if (!fs.fstat(fd, &stat)) {
      std::cout << "ls: Cannot stat " << name << std::endl;
      fs.close(fd);
      continue;
    }
    fs.close(fd);
    std::cout << i.name << "\t";
    switch (stat.type) {
      case T_dir:
        std::cout << "dir\t\\";
        break;
      case T_file:
        std::cout << "file\t" << stat.size << "byte";
    }

    std::cout << std::endl;
  }
}

void User::cd(string cdPath) {
  string path = this->fmtName(cdPath);
  if (!fs.openDir(path)) {
    std::cout << "cd: Cannot change dir " << path << std::endl;
  }
}

void User::cat(string fileName) {
  string file = this->fmtName(fileName);
  F_D fd = fs.open(file);
  if (fd == -1) {
    std::cout << "cat: Cannot open file " << file << std::endl;
    return;
  }
  string buf;
  int len = fs.read(fd, buf);
  fs.write(1, buf, len);
  fs.close(fd);
}
void User::cat(string inFileName, string outFileName) {
  string inFile = this->fmtName(inFileName);
  string outFile = this->fmtName(outFileName);
  F_D fd = fs.open(inFile);
  if (fd == -1) {
    std::cout << "cat: Cannot open file " << inFile << std::endl;
    return;
  }
  string buf;
  int len = fs.read(fd, buf);
  fs.close(fd);
  fd = fs.open(outFile);
  if (fd < 0) {
    if (!fs.createFile(outFile)) {
      std::cout << "cat: Cannot create file " << outFile << std::endl;
      return;
    }
    if ((fd = fs.open(outFile)) < 0) {
      std::cout << "cat: Cannot open file " << outFile << std::endl;
      return;
    }
  }
  Stat stat;
  fs.fstat(fd, &stat);
  if (stat.type == T_file) {
    fs.write(fd, buf);
  } else {
    std::cout << "cat: " << outFile << " is a directory." << std::endl;
  }
  fs.close(fd);
}

void User::echo(string data) { fs.write(1, data); }
void User::echo(string data, string filePathName) {
  string filePath = this->fmtName(filePathName);
  F_D fd = fs.open(filePath);
  if (fd > 0) {
    Stat stat;
    fs.fstat(fd, &stat);
    if (stat.type == T_file) {
      std::cout << "echo: " << filePath << " is exists, rewriting..."
                << std::endl;
    } else {
      std::cout << "echo: " << filePath << " is a directory, cannot rewrite."
                << std::endl;
      fs.close(fd);
      return;
    }
    fs.close(fd);
  }
  if (!fs.createFile(filePath)) {
    std::cout << "echo: Cannot create file " << filePath << std::endl;
    return;
  }
  if ((fd = fs.open(filePath)) < 0) {
    std::cout << "echo: Cannot open file " << filePath << std::endl;
    return;
  }
  if (fs.write(fd, data) < 0) {
    std::cout << "echo: Cannot write into file " << filePath << std::endl;
  }
  fs.close(fd);
}

void User::find(string dirName, string fileName) {
  string current = fs.getDirName();
  string dir = this->fmtName(dirName);
  string file = this->fmtName(fileName);
  cd(dir);
  nameList nl = fs.getName();
  for (auto i : nl.nameL) {
    if (i.name == file) {
      string f = dir + "/" + file;
      std::cout << f << std::endl;
    }
    if (i.type == T_dir) {
      string f = dir + "/" + file;
      find(f, file);
    }
  }
  cd(current);
}

void User::tree(int level) {
  string current = fs.getDirName();
  nameList nl = fs.getName();
  std::cout << "." << std::endl;
  int num = nl.nameL.size();
  for (auto i = nl.nameL.begin(); i != nl.nameL.end(); ++i) {
    if (i != --nl.nameL.end()) {
      std::cout << "├── " << i->name << std::endl;
    } else {
      std::cout << "└── " << i->name << std::endl;
    }
    if (i->type == T_dir && level != 1) {
      cd(i->name);
      tree(level - 1, 1);
      cd(current);
    }
  }
}
void User::tree(int level, int currentLevel) {
  string current = fs.getDirName();
  nameList nl = fs.getName();
  for (auto i = nl.nameL.begin(); i != nl.nameL.end(); ++i) {
    for (int i = 0; i < currentLevel; ++i) {
      std::cout << "│  ";
    }
    if (i != --nl.nameL.end()) {
      std::cout << "├── " << i->name << std::endl;
    } else {
      std::cout << "└── " << i->name << std::endl;
    }
    if (i->type == T_dir && level != 1) {
      cd(i->name);
      tree(level - 1, currentLevel + 1);
      cd(current);
    }
  }
}

void User::exit() { std::exit(EXIT_SUCCESS); }

void User::pwd() {
  string pwd = fs.getDirName();
  fs.write(1, pwd);
}