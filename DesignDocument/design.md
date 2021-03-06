# user interface

-   mkdir
    -   dirName
-   rmdir
    -   dirName
-   ls
-   cd
    -   dirName
-   cat
    -   fileName
-   echo
    -   string
    -   (optional) >
        -   fileName
-   find
    -   fileName
-   tree
    -   (optional) -L level=1
-   exit

# Block (class)

-   extend tree:
    -   File
    -   Dir
    -   Data

# Stat (class)

-   var:
    -   Offset dev
    -   S_type type:
        -   T_DIR = 0
        -   T_FILE = 1
    -   int Size

# fileSystem interface

-   File&Dir:
    -   F_D open(string FilePathName);
        -   ps: -1 Not Found, please create Dir/File.
    -   bool close(F_D n);
    -   bool fstat(F_D, Stat* st);

-   Dir:
    -   bool createDir(string DirName);
    
    -   bool deleteDir(string DirName);
    
    -   Path:
        -   bool openDir(string cdPath);
        -   string getDirName();
        
    -   NameList getName();
    
        -   ```c++
            NameList{
            	data{
            		S_type type;
            		string name;
            	}
            	list<data> nameL;
            }
            ```
-   FILE: 
    -   bool createFile(string DirName);
    -   bool deleteFile(string DirName);
    -   W_RET write(F_D n, string data);
    -   W_RET write(F_D n, string data, W_LEN len);
    -   R_RET read(F_D n, string& data, R_LEN len);

- [x] mkdir
- [x] rmdir
- [x] rm
- [x] ls
- [x] cd
- [x] cat
- [x] echo
- [x] find
- [x] tree
- [x] exit
- [x] pwd
- [x] fmtName
- [x] split