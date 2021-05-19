#include "./FileSystem.h"

class User {
 public:
  User() = default;

  // input dirName
  void mkdir(string dirName);

  // input dirName
  void rmdir(string dirName);

  // input fileName
  void rm(string fileName);

  // without input
  void ls();

  // input cdPath
  void cd(string cdPath);

  // input fileName
  void cat(string fileName);
  // input inFileName and outFileName
  void cat(string inFileName, string outFileName);

  // input data
  void echo(string data);
  // input data, filePathName
  void echo(string data, string filePathName);

  // input dirName, fileName
  void find(string dirName, string fileName);

  // input tree level
  void tree(int level = -1);
  // input tree level, current level
  void tree(int level, int currentLevel);

  // without input
  void exit();

  // without input
  void pwd();

  // input file/dir name
  string fmtName(string name);

  // input string, split char
  std::list<string> split(string str);

 private:
  FileSystem fs;
};