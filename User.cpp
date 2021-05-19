#include "./User.h"

#include <cstring>
#include <iostream>

string User::fmtName(string name) {
  if (name[0] == '/') {
    return name;
  }
}
std::list<string> User::split(string str, char s) {}

void User::mkdir(string dirName) {
  F_D fd = fs.open(dirName);
  if (fd != -1) {
    std::cout << "mkdir: " << dirName << ": File exists" << std::endl;
    fs.close(fd);
    return;
  }
  bool result = fs.createDir(dirName);
  if (!result) {
    std::cout << "Error: Cannot create dir " << dirName << std::endl;
  }
}

void User::rmdir(string dirName) {
  F_D fd;
  if ((fd = fs.open(dirName)) < 0) {
    std::cout << "rmdir: Cannot find dir " << dirName << std::endl;
  }
  Stat stat;
  if (!fs.fstat(fd, &stat)) {
    std::cout << "rmdir: Cannot stat dir " << dirName << std::endl;
    fs.close(fd);
    return;
  }
  if (stat.type != T_dir) {
    std::cout << "rmdir: " << dirName << " is not a dir" << std::endl;
    fs.close(fd);
    return;
  }
  fs.close(fd);
  if (!fs.deleteDir(dirName)) {
    std::cout << "rmdir: error: delete dir " << dirName << " failed"
              << std::endl;
  }
}

void User::rm(string fileName) {
  int fd = fs.open(fileName);
  if (fd < 0) {
    std::cout << "rm: file " << fileName << " not exists" << std::endl;
    return;
  }
  fs.close(fd);
  bool result = fs.deleteFile(fileName);
  if (!result) {
    std::cout << "rm: delete file " << fileName << "failed" << std::endl;
  }
}

void User::ls() {
  FileSystem::nameList nl = fs.getName();
  for (auto i : nl.nameL) {
    F_D fd = fs.open(i.name);
    if (fd < 0) {
      std::cout << "ls: Cannot open file " << i.name << std::endl;
      continue;
    }
    Stat stat;
    if (!fs.fstat(fd, &stat)) {
      std::cout << "ls: Cannot stat " << i.name << std::endl;
      fs.close(fd);
      continue;
    }
    fs.close(fd);
    std::cout << i.name << " " << stat.type << " " << stat.size << std::endl;
  }
}

void User::cd(string cdPath) {
  if (!fs.openDir(cdPath)) {
    std::cout << "cd: Cannot change dir " << cdPath << std::endl;
  }
}

void User::cat(string fileName) {
  F_D fd = fs.open(fileName);
  if (fd == -1) {
    std::cout << "Cannot open file " << fileName << std::endl;
    return;
  }
  string buf;
  int len = fs.read(fd, buf);
  fs.write(1, buf, len);
  fs.close(fd);
}
void User::cat(string inFileName, string outFileName) {
  F_D fd = fs.open(inFileName);
  if (fd == -1) {
    std::cout << "cat: Cannot open file " << inFileName << std::endl;
    return;
  }
  string buf;
  int len = fs.read(fd, buf);
  fs.close(fd);
  fd = fs.open(outFileName);
  if (fd < 0) {
    if (fs.createFile(outFileName)) {
      std::cout << "cat: Cannot create file " << outFileName << std::endl;
      return;
    }
    if ((fd = fs.open(outFileName)) < 0) {
      std::cout << "cat: Cannot open file " << outFileName << std::endl;
      return;
    }
  }
  fs.write(fd, buf);
  fs.close(fd);
}

void User::echo(string data) { fs.write(1, data); }
void User::echo(string data, string filePathName) {
  F_D fd = fs.open(filePathName);
  if (fd > 0) {
    std::cout << "echo: " << filePathName << " is exists, rewriting..."
              << std::endl;
    fs.close(fd);
    fs.deleteFile(filePathName);
  }
  if (!fs.createFile(filePathName)) {
    std::cout << "echo: Cannot create file " << filePathName << std::endl;
    return;
  }
  if ((fd = fs.open(filePathName)) < 0) {
    std::cout << "echo: Cannot open file " << filePathName << std::endl;
    return;
  }
  if (fs.write(fd, data) < 0) {
    std::cout << "echo: Cannot write into file " << filePathName << std::endl;
  }
  fs.close(fd);
}

void User::find(string dirName, string fileName) {
  string current = fs.getDirName();
  cd(dirName);
  nameList nl = fs.getName();
  for (auto i : nl.nameL) {
    if (i.name == fileName) {
      string f = dirName + "/" + fileName;
      std::cout << f << std::endl;
    }
    if (i->S_type == T_dir) {
      string f = dirName + "/" + fileName;
      find(f, fileName);
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
    if (i != nl.nameL.end() - 1) {
      std::cout << "├── " << i.name << std::endl;
    } else {
      std::cout << "└── " << i.name << std::endl;
    }
    if (i->S_type == T_dir && level != 1) {
      cd(i.name);
      tree(level - 1, 1);
      cd(current);
    }
  }
}
void User::tree(int level, int currentLevel) { nameList }

void User::exit() {}

void User::pwd() {}