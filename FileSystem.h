#include "FileOptor.h"
#include "stat.h"

class FileSystem {
 public:
  /* used for File & Dir
Input:  Relative path
Output: FILE_DESCRIPTOR
*/
  F_D open(const string FilePathName);

  /* used for File & Dir
Input:  FILE_DESCRIPTOR
Output: Boolen
*/
  bool close(const F_D fd);

  /* used for File & Dir
Input:  FILE_DESCRIPTOR
      Stat_ptr
Output: Boolen
*/
  bool fstat(const F_D fd, const Stat* st);

  /* used for Dir
Input:  DIR_NAME
Output: Boolen
*/
  bool createDir(const string DirName);

  /* used for Dir
Input:  DIR_NAME
Output: Boolen
*/
  bool deleteDir(const string DirName);

  /* used for Dir_path
Input:  path
Output: Boolen
*/
  bool openDir(const string cdPath);

  /* used for Dir_path
Output: Boolen
*/
  string getDirName();

  /* used for File
Input:  File_NAME
Output: Boolen
*/
  bool createFile(const string DirName);

  /* used for File
Input:  File_NAME
Output: Boolen
*/
  bool deleteFile(const string DirName);

  /* used for File
Input:  FILE_DESCRIPTOR
      Data
      Write_len
Output: Write_Return
*/
  W_RET write(const F_D fd, const string data, W_LEN len);
  W_RET write(const F_D fd, const string data);

  /* used for File
Input:  FILE_DESCRIPTOR
      Data
      Read_len
Output: Read_Return
*/
  R_RET read(const F_D fd, const string& data, R_LEN len);
  R_RET read(const F_D fd, const string& data);
};
