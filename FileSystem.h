#pragma once
#include <list>

#include "F_D_saver.h"
#include "FileOptor.h"

class FileSystem {
private:
	FileOptor file_opt;
	Offset nowDir_off = 0;
	F_D_saver fdBuf;

	Offset findLongPath(Offset off_begin, std::list<string>& pathList);
	Offset findDirSonDir(Offset off_begin, string name);
	Offset findDirSonFile(Offset off_begin, string name);
	bool cleanFileData(Offset FileData_Begin);

public:
	FileSystem();
	~FileSystem();
	bool PathSpliter(string path, const char split, std::list<string>& result);
	// used for File & Dir
	// Input:     path
	// Output:    FILE_DESCRIPTOR
	F_D open(const string FilePathName);

	// used for File & Dir
	// Input:     FILE_DESCRIPTOR
	// Output:    Boolen
	bool close(const F_D fd);

	// used for File & Dir
	// Input:     FILE_DESCRIPTOR
	//            Stat_ptr
	// Output:    Boolen
	bool fstat(const F_D fd, Stat* st);

	// used for Dir
	// Input:     DIR_NAME
	// Output:    Boolen
	bool createDir(const string DirName);

	// used for Dir
	// Input:     DIR_NAME
	// Output:    Boolen
	bool deleteDir(const string DirName);

	// used for Dir_path
	// Input:     path
	// Output:    Boolen
	bool openDir(const string cdPath);

	// used for Dir_path
	// Output:    Boolen
	string getDirName();

	struct nameList {
		struct l_data {
			S_type type;
			string name;
			// bool operator<(l_data b) { return this->type > b.type ? true : (this->name < b.name); }
			bool operator<(l_data b) const {
				if (type == b.type)
					return name < b.name;
				return type > b.type;
			}
		};
		using l_data = struct l_data;
		std::list<l_data> nameL;
	};
	using nameList = struct nameList;
	using l_data = nameList::l_data;
	// used for Dir
	// Input:     DIR_NAME
	// Output:    Boolen
	nameList getName();

	// used for File
	// Input:     File_NAME
	// Output:    Boolen
	bool createFile(const string DirName);

	// used for File
	// Input:     File_NAME
	// Output:    Boolen
	bool deleteFile(const string DirName);

	// used for File
	// Input:     FILE_DESCRIPTOR
	//            Data
	//            Write_len
	// Output:    Write_Return
	W_RET write(const F_D fd, string data, W_LEN len = -1);

	// used for File
	// Input:     FILE_DESCRIPTOR
	//            Data
	//            Read_len
	// Output:    Read_Return
	R_RET read(const F_D fd, string& data, R_LEN len = -1);

	bool fsExit();
};
using nameList = FileSystem::nameList;
