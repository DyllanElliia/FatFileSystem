#include "FileSystem.h"

#include <sstream>

FileSystem::FileSystem() {}
FileSystem::~FileSystem() {}

bool FileSystem::PathSpliter(string path, const char split,
                             std::list<string>& result) {
  std::istringstream iss(path);       // 输入流
  string token;                       // 接收缓冲区
  while (getline(iss, token, split))  // 以split为分隔符
  {
    result.push_back(token);
  }
  for (auto i = result.begin(); i != result.end();) {
    if ((*i).size() == 0) {
      i = result.erase(i);
    } else
      ++i;
  }
  return true;
}

Offset FileSystem::findDirSonDir(Offset off_begin, string name) {
  Offset dir_off;
  file_opt.move2offset_short(off_begin);
  Dir lastDir;
  if (!file_opt.readData(lastDir.getDataCharPtr(), lastDir.getDataSize())) {
    std::cout << "read data failed. Path: " << name << std::endl;
    return 0;
  }
  Offset dir_begin = lastDir.getSonDirHead();
  if (dir_begin == 0) return 0;
  if (dir_begin < HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
    std::cout << "error path. " << name << " can not be found." << std::endl;
    return 0;
  }
  dir_off = dir_begin;
  while (dir_off >= HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
    file_opt.move2offset_short(dir_off);
    Dir blockData;
    if (!file_opt.readData(blockData.getDataCharPtr(),
                           blockData.getDataSize())) {
      std::cout << "read data failed. Path: " << name << std::endl;
      return 0;
    }
    if (blockData.getStat().dev != dir_off) {
      std::cout << "Block data errow! path: " << name << std::endl;
      return 0;
    }
    if (name.compare(blockData.getName()) == 0) {
      return dir_off;
    }
    dir_off = blockData.getBrotherOff();
  }
  return 0;
}

Offset FileSystem::findDirSonFile(Offset off_begin, string name) {
  Offset dir_off;
  file_opt.move2offset_short(off_begin);
  Dir lastDir;
  if (!file_opt.readData(lastDir.getDataCharPtr(), lastDir.getDataSize())) {
    std::cout << "read data failed. Path: " << name << std::endl;
    return 0;
  }
  Offset dir_begin = lastDir.getSonFileHead();
  if (dir_begin == 0) return 0;
  if (dir_begin < HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
    std::cout << "error path. " << name << " can not be found." << std::endl;
    return 0;
  }
  dir_off = dir_begin;
  while (dir_off >= HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
    file_opt.move2offset_short(dir_off);
    File blockData;
    if (!file_opt.readData(blockData.getDataCharPtr(),
                           blockData.getDataSize())) {
      std::cout << "read data failed. Path: " << name << std::endl;
      return 0;
    }
    if (blockData.getStat().dev != dir_off) {
      std::cout << "Block data errow! path: " << name << std::endl;
      return 0;
    }
    if (name.compare(blockData.getName()) == 0) {
      return dir_off;
    }
    dir_off = blockData.getBrotherOff();
  }
  return 0;
}

Offset FileSystem::findLongPath(Offset off_begin, std::list<string>& pathList) {
  if (pathList.size() == 0) return off_begin;
  Offset lastDir_offset = off_begin;
  Offset dir_off;
  for (auto path_name : pathList) {
    file_opt.move2offset_short(lastDir_offset);
    Dir lastDir;
    if (!file_opt.readData(lastDir.getDataCharPtr(), lastDir.getDataSize())) {
      std::cout << "read data failed. Path: " << path_name << std::endl;
      return 0;
    }
    Offset dir_begin = lastDir.getSonDirHead();
    if (dir_begin == 0) return 0;
    if (dir_begin < HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
      std::cout << "error path. " << path_name << " can not be found."
                << std::endl;
      return 0;
    }
    dir_off = dir_begin;
    while (dir_off >= HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
      file_opt.move2offset_short(dir_off);
      Dir blockData;
      if (!file_opt.readData(blockData.getDataCharPtr(),
                             blockData.getDataSize())) {
        std::cout << "read data failed. Path: " << path_name << std::endl;
        return 0;
      }
      if (blockData.getStat().dev != dir_off) {
        std::cout << "Block data errow! path: " << path_name << std::endl;
        return 0;
      }
      if (path_name.compare(blockData.getName()) == 0) {
        lastDir_offset = dir_off;
        break;
      }
      dir_off = blockData.getBrotherOff();
    }
  }
  if (dir_off < HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) return 0;
  return dir_off;
}

// used for File & Dir
// Input:     path
// Output:    FILE_DESCRIPTOR
F_D FileSystem::open(const string PathName) {
  std::list<string> pathList;
  PathSpliter(PathName, '/', pathList);
  Block* pathB = nullptr;
  if (pathList.size() == 0) {
    std::cout << "path is empty!" << std::endl;
    return -1;
  }
  if (pathList.size() == 1) {
    // create File in Fat1 AND Fat2
    Offset fat_offset = file_opt.findFat1Block(T_file, pathList.front());
    if (!file_opt.getLastFindBool()) {
      fat_offset = file_opt.findFat1Block(T_dir, pathList.front());
      if (!file_opt.getLastFindBool()) {
        std::cout << pathList.front() << " not found!" << std::endl;
        return -1;
      }
    }
    F_D fd = fdBuf.get();
    if (fdBuf.set(fd, fat_offset))
      return fd;
    else {
      std::cout << PathName << " has been opened!" << std::endl;
      return -1;
    }
  }
  // long path
  // FAT1
  Offset Fat1_offset = file_opt.findFat1Block(T_dir, pathList.front());
  if (!file_opt.getLastFindBool()) {
    std::cout << pathList.front() << " not found" << std::endl;
    return -1;
  }
  pathList.pop_front();
  string back_name = pathList.back();
  pathList.pop_back();
  Offset dir_off = findLongPath(Fat1_offset, pathList);
  // std::cout << "dir: " << dir_off << std::endl;
  if (dir_off >= HEAD_SIZE) {
    Offset fd_off = findDirSonFile(dir_off, back_name);
    if (fd_off < HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
      fd_off = findDirSonDir(dir_off, back_name);
      if (fd_off < HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
        std::cout << PathName << " already exists!" << std::endl;
        return -1;
      }
    }
    F_D fd = fdBuf.get();
    if (fd && fdBuf.set(fd, fd_off))
      return fd;
    else {
      std::cout << PathName << " has been opened!" << std::endl;
      return -1;
    }
  }
  std::cout << PathName << ": open file failed." << std::endl;
  return -1;
}

// used for File & Dir
// Input:     FILE_DESCRIPTOR
// Output:    Boolen
bool FileSystem::close(const F_D fd) {
  if (fd == -1) return false;
  return fdBuf.remove(fd);
}

// used for File & Dir
// Input:     FILE_DESCRIPTOR
//            Stat_ptr
// Output:    Boolen
bool FileSystem::fstat(const F_D fd, const Stat* st) {
  if (fd == -1) return false;
  Offset off = fdBuf.getOff(fd);
  if (off >= HEAD_SIZE) {
    if (!st) {
      std::cout << "stat_ptr must be new a space!" << std::endl;
      return false;
    }
    file_opt.move2offset_short(off);
    file_opt.readData((char*)st, sizeof(Stat));
    if (st->dev != off) {
      std::cout << "error, the F_D you input is damaged!" << std::endl;
      return false;
    }
    return true;
  }
  return false;
}

// used for Dir
// Input:     DIR_NAME
// Output:    Boolen
bool FileSystem::createDir(const string DirName) {
  std::list<string> pathList;
  PathSpliter(DirName, '/', pathList);
  Block* pathB = nullptr;
  if (pathList.size() == 0) {
    std::cout << "path is empty!" << std::endl;
    return false;
  }
  if (pathList.size() == 1) {
    // create Dir in Fat1 AND Fat2
    Offset fat_offset = file_opt.findFat1Block(T_dir, pathList.front());
    if (file_opt.getLastFindBool()) {
      std::cout << pathList.front() << " already exists!" << std::endl;
      return false;
    }
    fat_offset = file_opt.findEmptyFatBlock();
    if (!file_opt.getLastFindBool()) {
      std::cout << "failed to find empty fat block!" << std::endl;
      return false;
    }
    Stat st;
    st.type = T_dir;
    st.dev = fat_offset;
    st.size = 0;
    Dir fat;
    fat.setStat(st);
    fat.changeFatherOff(fat_offset);
    fat.setName(pathList.front());
    // std::cout << fat.getStat().dev << " of: " << fat_offset << std::endl;
    file_opt.move2offset_short(fat_offset);
    file_opt.saveData(fat.getDataCharPtr(), fat.getDataSize());

    fat_offset += FAT1_SIZE;
    st.dev = fat_offset;
    fat.setStat(st);
    fat.changeFatherOff(fat_offset);
    fat.setName(pathList.front());
    file_opt.move2offset_short(fat_offset);
    file_opt.saveData(fat.getDataCharPtr(), fat.getDataSize());
    return true;
  }
  // long path
  // FAT1
  Offset Fat1_offset = file_opt.findFat1Block(T_dir, pathList.front());
  if (!file_opt.getLastFindBool()) {
    std::cout << pathList.front() << " not found" << std::endl;
    return false;
  }
  pathList.pop_front();
  string back_name = pathList.back();
  pathList.pop_back();
  Offset dir_off = findLongPath(Fat1_offset, pathList);
  // std::cout << "dir: " << dir_off << std::endl;
  if (dir_off >= HEAD_SIZE) {
    if (findDirSonDir(dir_off, back_name) >=
        HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
      std::cout << DirName << " already exists!" << std::endl;
      return false;
    }
    Offset newOff = file_opt.findEmptyBlock();
    if (!file_opt.getLastFindBool()) {
      std::cout << "find Empty Block failed." << std::endl;
      return false;
    }
    Dir newDir;
    Dir fatherDir;
    file_opt.move2offset_short(dir_off);
    file_opt.readData(fatherDir.getDataCharPtr(), fatherDir.getDataSize());
    Stat st;
    st.type = T_dir;
    st.dev = newOff;
    st.size = 0;
    newDir.setStat(st);
    newDir.changeFatherOff(dir_off);
    newDir.setName(back_name);
    newDir.changeBrother(fatherDir.getSonDirHead());
    fatherDir.changeSonDirHead(newOff);
    file_opt.move2offset_short(dir_off);
    file_opt.saveData(fatherDir.getDataCharPtr(), fatherDir.getDataSize());
    file_opt.move2offset_short(newOff);
    file_opt.saveData(newDir.getDataCharPtr(), newDir.getDataSize());
    return true;
  }
  std::cout << DirName << ": create dir failed." << std::endl;
  return false;
}

// used for Dir
// Input:     DIR_NAME
// Output:    Boolen
bool FileSystem::deleteDir(const string DirName) {
  std::list<string> pathList;
  PathSpliter(DirName, '/', pathList);
  Block* pathB = nullptr;
  if (pathList.size() == 0) {
    std::cout << "path is empty!" << std::endl;
    return false;
  }
  if (pathList.size() == 1) {
    // create Dir in Fat1 AND Fat2
    Offset fat_offset = file_opt.findFat1Block(T_dir, pathList.front());
    if (!file_opt.getLastFindBool()) {
      std::cout << pathList.front() << " not found!" << std::endl;
      return false;
    }
    // delete
    Offset save = nowDir_off;
    nowDir_off = fat_offset;
    nameList nList = getName();
    nowDir_off = save;
    for (auto obj : nList.nameL) {
      string newPath = DirName + "/" + obj.name;
      switch (obj.type) {
        case T_dir:
          deleteDir(newPath);
          break;
        case T_file:
          deleteFile(newPath);
          break;
        default:
          std::cout << "error not path " << newPath << std::endl;
      }
    }

    file_opt.cleanFatBlock(fat_offset);
    return true;
  }
  // long path
  // FAT1
  Offset Fat1_offset = file_opt.findFat1Block(T_dir, pathList.front());
  if (!file_opt.getLastFindBool()) {
    std::cout << pathList.front() << " not found" << std::endl;
    return false;
  }
  pathList.pop_front();
  string back_name = pathList.back();
  pathList.pop_back();
  Offset dir_off = findLongPath(Fat1_offset, pathList);
  // std::cout << "dir: " << dir_off << std::endl;
  if (dir_off >= HEAD_SIZE) {
    Offset DirHere = findDirSonDir(dir_off, back_name);
    if (DirHere >= HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
      Offset save = nowDir_off;
      nowDir_off = DirHere;
      nameList nList = getName();
      nowDir_off = save;
      for (auto obj : nList.nameL) {
        string newPath = DirName + "/" + obj.name;
        switch (obj.type) {
          case T_dir:
            deleteDir(newPath);
            break;
          case T_file:
            deleteFile(newPath);
            break;
          default:
            std::cout << "error not path " << newPath << std::endl;
        }
      }
      // chang link
      // if father link you?
      Dir fatherDir;
      file_opt.move2offset_short(dir_off);
      file_opt.readData(fatherDir.getDataCharPtr(), fatherDir.getDataSize());
      Dir nowDir;
      file_opt.move2offset_short(DirHere);
      file_opt.readData(nowDir.getDataCharPtr(), nowDir.getDataSize());
      if (fatherDir.getSonDirHead() == DirHere) {
        fatherDir.changeSonDirHead(nowDir.getBrotherOff());
        file_opt.move2offset_short(dir_off);
        file_opt.saveData(fatherDir.getDataCharPtr(), fatherDir.getDataSize());
      } else {
        Offset lastDir_offset = fatherDir.getSonDirHead();
        while (true) {
          Dir dir_data;
          file_opt.move2offset_short(lastDir_offset);
          file_opt.readData(dir_data.getDataCharPtr(), dir_data.getDataSize());
          if (dir_data.getBrotherOff() == DirHere) {
            dir_data.changeBrother(nowDir.getBrotherOff());
            file_opt.move2offset_short(lastDir_offset);
            file_opt.saveData(dir_data.getDataCharPtr(),
                              dir_data.getDataSize());
            break;
          }
          lastDir_offset = dir_data.getBrotherOff();
        }
      }
      file_opt.cleanBlock(DirHere);
      return true;
    }
    std::cout << "failed to find " << back_name << std::endl;
    return false;
  }
  std::cout << DirName << ": delete dir failed." << std::endl;
  return false;
}

// used for Dir_path
// Input:     path
// Output:    Boolen
bool FileSystem::openDir(const string cdPath) {
  std::list<string> pathList;
  PathSpliter(cdPath, '/', pathList);
  if (pathList.size() == 0) {
    nowDir_off = 0;
    return true;
  }
  if (pathList.size() == 1) {
    Offset fat_off = file_opt.findFat1Block(T_dir, pathList.front());
    if (!file_opt.getLastFindBool()) {
      std::cout << "failed to find " << cdPath << std::endl;
      return false;
    }
    nowDir_off = fat_off;
    return true;
  }
  // long path
  // FAT1
  Offset Fat1_offset = file_opt.findFat1Block(T_dir, pathList.front());
  if (!file_opt.getLastFindBool()) {
    std::cout << pathList.front() << " not found" << std::endl;
    return false;
  }
  pathList.pop_front();
  Offset dir_off = findLongPath(Fat1_offset, pathList);
  if (dir_off < HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
    std::cout << "failed to find " << cdPath << std::endl;
    return false;
  }
  nowDir_off = dir_off;
  return true;
}

// used for Dir_path
// Output:    Boolen
string FileSystem::getDirName() {
  string path = "";
  Offset now_off = nowDir_off;
  if (now_off == 0) {
    return path + "/";
  }
  Dir nowDir;
  while (true) {
    file_opt.move2offset_short(now_off);
    file_opt.readData(nowDir.getDataCharPtr(), nowDir.getDataSize());
    path = "/" + nowDir.getName() + path;
    if (nowDir.getFatherOff() == now_off) {
      break;
    }
    now_off = nowDir.getFatherOff();
  }
  return path;
}

// used for Dir
// Input:     DIR_NAME
// Output:    Boolen
nameList FileSystem::getName() {
  nameList nList;
  // for FAT again. i hate fat. i think i shoule design a better fat file struct
  // at next time.
  if (nowDir_off == 0) {
    Offset fat_off = HEAD_SIZE;
    for (int i = 0; i < FAT_BLOCK_NUM; ++i) {
      file_opt.move2offset_short(fat_off + i * BLOCK_SIZE);
      char blockData[BLOCK_SIZE];
      file_opt.readData(blockData, BLOCK_SIZE);
      // std::cout << string(((DirData*)blockData)->name) << std::endl;
      switch (((DirData*)blockData)->st.type) {
        case T_dir:
          nList.nameL.push_back({T_dir, string(((DirData*)blockData)->name)});
          break;
        case T_file:
          nList.nameL.push_back({T_file, string(((DirData*)blockData)->name)});
          break;
      }
    }
    return nList;
  }

  Dir nowDir;
  file_opt.move2offset_short(nowDir_off);
  file_opt.readData(nowDir.getDataCharPtr(), nowDir.getDataSize());
  Offset sonDir_off = nowDir.getSonDirHead();
  Offset sonFile_off = nowDir.getSonFileHead();

  while (sonDir_off) {
    Dir sonDir;
    file_opt.move2offset_short(sonDir_off);
    file_opt.readData(sonDir.getDataCharPtr(), sonDir.getDataSize());
    nList.nameL.push_back({T_dir, sonDir.getName()});
    sonDir_off = sonDir.getBrotherOff();
  }

  while (sonFile_off) {
    File sonFile;
    file_opt.move2offset_short(sonFile_off);
    file_opt.readData(sonFile.getDataCharPtr(), sonFile.getDataSize());
    nList.nameL.push_back({T_file, sonFile.getName()});
    sonFile_off = sonFile.getBrotherOff();
  }
  return nList;
}

// used for File
// Input:     File_NAME
// Output:    Boolen
bool FileSystem::createFile(const string DirName) {
  std::list<string> pathList;
  PathSpliter(DirName, '/', pathList);
  Block* pathB = nullptr;
  if (pathList.size() == 0) {
    std::cout << "path is empty!" << std::endl;
    return false;
  }
  if (pathList.size() == 1) {
    // create File in Fat1 AND Fat2
    Offset fat_offset = file_opt.findFat1Block(T_file, pathList.front());
    if (file_opt.getLastFindBool()) {
      std::cout << pathList.front() << " already exists!" << std::endl;
      return false;
    }
    fat_offset = file_opt.findEmptyFatBlock();
    if (!file_opt.getLastFindBool()) {
      std::cout << "failed to find empty fat block!" << std::endl;
      return false;
    }
    Stat st;
    st.type = T_file;
    st.dev = fat_offset;
    st.size = 0;
    File fat;
    fat.setStat(st);
    fat.changeFatherOff(fat_offset);
    fat.setName(pathList.front());
    // std::cout << fat.getStat().dev << " of: " << fat_offset << std::endl;
    file_opt.move2offset_short(fat_offset);
    file_opt.saveData(fat.getDataCharPtr(), fat.getDataSize());

    fat_offset += FAT1_SIZE;
    st.dev = fat_offset;
    fat.setStat(st);
    fat.changeFatherOff(fat_offset);
    fat.setName(pathList.front());
    file_opt.move2offset_short(fat_offset);
    file_opt.saveData(fat.getDataCharPtr(), fat.getDataSize());
    return true;
  }
  // long path
  // FAT1
  Offset Fat1_offset = file_opt.findFat1Block(T_dir, pathList.front());
  if (!file_opt.getLastFindBool()) {
    std::cout << pathList.front() << " not found" << std::endl;
    return false;
  }
  pathList.pop_front();
  string back_name = pathList.back();
  pathList.pop_back();
  Offset dir_off = findLongPath(Fat1_offset, pathList);
  // std::cout << "dir: " << dir_off << std::endl;
  if (dir_off >= HEAD_SIZE) {
    if (findDirSonFile(dir_off, back_name) >=
        HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
      std::cout << DirName << " already exists!" << std::endl;
      return false;
    }
    Offset newOff = file_opt.findEmptyBlock();
    if (!file_opt.getLastFindBool()) {
      std::cout << "find Empty Block failed." << std::endl;
      return false;
    }
    File newFile;
    Dir fatherDir;
    file_opt.move2offset_short(dir_off);
    file_opt.readData(fatherDir.getDataCharPtr(), fatherDir.getDataSize());
    Stat st;
    st.type = T_file;
    st.dev = newOff;
    st.size = 0;
    newFile.setStat(st);
    newFile.changeFatherOff(dir_off);
    newFile.setName(back_name);
    newFile.changeBrother(fatherDir.getSonFileHead());
    fatherDir.changeSonFileHead(newOff);
    file_opt.move2offset_short(dir_off);
    file_opt.saveData(fatherDir.getDataCharPtr(), fatherDir.getDataSize());
    file_opt.move2offset_short(newOff);
    file_opt.saveData(newFile.getDataCharPtr(), newFile.getDataSize());
    return true;
  }
  std::cout << DirName << ": create file failed." << std::endl;
  return false;
}

bool FileSystem::cleanFileData(Offset FileData_Begin) {
  while (FileData_Begin >= HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
    FileContent file_c;
    file_opt.move2offset_short(FileData_Begin);
    file_opt.readData(file_c.getDataCharPtr(), file_c.getDataSize());
    // clean
    file_opt.cleanBlock(FileData_Begin);
    FileData_Begin = file_c.getBrotherOff();
  }
  return true;
}

// used for File
// Input:     File_NAME
// Output:    Boolen
bool FileSystem::deleteFile(const string DirName) {
  std::list<string> pathList;
  PathSpliter(DirName, '/', pathList);
  Block* pathB = nullptr;
  if (pathList.size() == 0) {
    std::cout << "path is empty!" << std::endl;
    return false;
  }
  if (pathList.size() == 1) {
    // create Dir in Fat1 AND Fat2
    Offset fat_offset = file_opt.findFat1Block(T_file, pathList.front());
    if (!file_opt.getLastFindBool()) {
      std::cout << pathList.front() << " not found!" << std::endl;
      return false;
    }
    // delete
    File file_d;
    file_opt.move2offset_short(fat_offset);
    file_opt.readData(file_d.getDataCharPtr(), file_d.getDataSize());
    if (!cleanFileData(file_d.getFileData())) {
      std::cout << "clean file failed!" << std::endl;
      return false;
    }
    file_opt.cleanFatBlock(fat_offset);
    return true;
  }
  // long path
  // FAT1
  Offset Fat1_offset = file_opt.findFat1Block(T_dir, pathList.front());
  if (!file_opt.getLastFindBool()) {
    std::cout << pathList.front() << " not found" << std::endl;
    return false;
  }
  pathList.pop_front();
  string back_name = pathList.back();
  pathList.pop_back();
  Offset dir_off = findLongPath(Fat1_offset, pathList);
  // std::cout << "dir: " << dir_off << std::endl;
  if (dir_off >= HEAD_SIZE) {
    Offset FileHere = findDirSonFile(dir_off, back_name);
    if (FileHere >= HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
      // delete
      File file_d;
      file_opt.move2offset_short(FileHere);
      file_opt.readData(file_d.getDataCharPtr(), file_d.getDataSize());
      if (!cleanFileData(file_d.getFileData())) {
        std::cout << "clean file failed!" << std::endl;
        return false;
      }

      // chang link
      // if father link you?
      Dir fatherDir;
      file_opt.move2offset_short(dir_off);
      file_opt.readData(fatherDir.getDataCharPtr(), fatherDir.getDataSize());
      // Dir nowDir;
      // file_opt.move2offset_short(FileHere);
      // file_opt.readData(file_d.getDataCharPtr(), file_d.getDataSize());
      if (fatherDir.getSonFileHead() == FileHere) {
        fatherDir.changeSonFileHead(file_d.getBrotherOff());
        file_opt.move2offset_short(dir_off);
        file_opt.saveData(fatherDir.getDataCharPtr(), fatherDir.getDataSize());
      } else {
        Offset lastFile_offset = fatherDir.getSonFileHead();
        while (true) {
          File file_data;
          file_opt.move2offset_short(lastFile_offset);
          file_opt.readData(file_data.getDataCharPtr(),
                            file_data.getDataSize());
          if (file_data.getBrotherOff() == FileHere) {
            file_data.changeBrother(file_d.getBrotherOff());
            file_opt.move2offset_short(lastFile_offset);
            file_opt.saveData(file_data.getDataCharPtr(),
                              file_data.getDataSize());
            break;
          }
          lastFile_offset = file_data.getBrotherOff();
        }
      }

      file_opt.cleanBlock(FileHere);
      return true;
    }
    std::cout << "failed to find " << back_name << std::endl;
    return false;
  }
  std::cout << DirName << ": delete dir failed." << std::endl;
  return false;
}

// used for File
// Input:     FILE_DESCRIPTOR
//            Data
//            Write_len
// Output:    Write_Return
W_RET FileSystem::write(const F_D fd, string data, W_LEN len) {
  if (fd == 1) {
    std::cout << data.substr(0, len) << std::endl;
    return len;
  }
  W_RET w_len = 0;
  // std::cout << w_len << " " << len << std::endl;
  Offset file_off = fdBuf.getOff(fd);
  if (file_off < HEAD_SIZE) {
    std::cout << "Please open the file before you write." << std::endl;
    return w_len;
  }
  if (len == -1 && len > data.size()) len = data.size();
  if (len < 0) {
    std::cout << "len must >= 0" << std::endl;
    return w_len;
  }

  Offset fileContent_off;
  File file_data;
  file_opt.move2offset_short(file_off);
  file_opt.readData(file_data.getDataCharPtr(), file_data.getDataSize());
  fileContent_off = file_data.getFileData();

  if (!cleanFileData(fileContent_off)) {
    std::cout << "failed to clean file!" << std::endl;
    return w_len;
  }

  if (len == 0) {
    file_data.changeFileData(0);
    file_opt.move2offset_short(file_off);
    file_opt.saveData(file_data.getDataCharPtr(), file_data.getDataSize());
    return w_len;
  }
  file_data.changeStSize(len);

  Offset fileC_off_begin, fileC_off_now;
  FileContent file_c;
  fileC_off_now = file_opt.findEmptyBlock();
  if (!file_opt.getLastFindBool()) {
    std::cout << "failed to find first block!" << std::endl;
    return w_len;
  }
  fileC_off_begin = fileC_off_now;
  Stat st;
  st.type = T_file_content;
  st.dev = fileC_off_now;
  st.size = 0;
  int count = 0;
  char* fc_data = file_c.getDataptr();
  data.push_back(' ');
  for (auto i : data) {
    if (count == F_DATA_LEN_LIMIT) {
      // std::cout << fileC_off_now << std::endl;
      st.size = count;
      file_c.setStat(st);
      Offset save_now = fileC_off_now;
      fileC_off_now = file_opt.findEmptyBlock(1);
      if (!file_opt.getLastFindBool()) {
        std::cout << "failed to find empty block!" << std::endl;
        return w_len;
      }
      file_c.changeBrother(fileC_off_now);
      file_opt.move2offset_short(save_now);
      file_opt.saveData(file_c.getDataCharPtr(), file_c.getDataSize());
      count = 0;
      st.dev = fileC_off_now;
    }
    if (len == 0) {
      // std::cout << fileC_off_now << std::endl;
      st.size = count;
      file_c.setStat(st);
      std::cout << file_c.getStat().size << std::endl;
      file_opt.move2offset_short(fileC_off_now);
      file_c.changeBrother(0);
      file_opt.saveData(file_c.getDataCharPtr(), file_c.getDataSize());
      break;
    }
    fc_data[count++] = i;
    len--;
    w_len++;
  }
  // std::cout << fileC_off_begin << std::endl;
  file_data.changeFileData(fileC_off_begin);
  file_opt.move2offset_short(file_off);
  file_opt.saveData(file_data.getDataCharPtr(), file_data.getDataSize());
  return w_len;
}

// used for File
// Input:     FILE_DESCRIPTOR
//            Data
//            Read_len
// Output:    Read_Return
R_RET FileSystem::read(const F_D fd, string& data, R_LEN len) {
  R_RET r_len = 0;
  // std::cout << r_len << " " << len << std::endl;
  Offset file_off = fdBuf.getOff(fd);
  if (file_off < HEAD_SIZE) {
    std::cout << "Please open the file before you write." << std::endl;
    return r_len;
  }
  if (len == -1) len = 1000000007;
  if (len < 0 && len != -1) {
    std::cout << "len must >= 0" << std::endl;
    return r_len;
  }

  Offset fileContent_off;
  File file_data;
  file_opt.move2offset_short(file_off);
  file_opt.readData(file_data.getDataCharPtr(), file_data.getDataSize());
  fileContent_off = file_data.getFileData();

  while (fileContent_off >= HEAD_SIZE + FAT1_SIZE + FAT2_SIZE) {
    // std::cout << fileContent_off << std::endl;
    if (r_len >= len) break;

    FileContent file_c;
    file_opt.move2offset_short(fileContent_off);
    file_opt.readData(file_c.getDataCharPtr(), file_c.getDataSize());
    // read
    char* fc_data = file_c.getDataptr();
    // std::cout << file_c.getStat().dev << std::endl;
    for (int i = 0; i < file_c.getStat().size; ++i) {
      data.push_back(fc_data[i]);
      r_len++;
    }
    fileContent_off = file_c.getBrotherOff();
  }
  return r_len;
}