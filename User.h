#include "./FileSystem.h"

class User {
 public:
  User() = default;
  void mkdir(string dirName);
  void rmdir(string dirName);
  void ls();
  void cd(string dirName);
  void cat(string fileName);
  void echo(string data);
  void find(string fileName);
  void tree(int level = 1);
  void exit();

 private:
  FileSystem fileSystem;
};