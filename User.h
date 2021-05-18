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

  // input dirName
  void cd(string dirName);

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
  void tree(int level = 1);

  // without input
  void exit();

  // without input
  void pwd();

 private:
  FileSystem fs;
};