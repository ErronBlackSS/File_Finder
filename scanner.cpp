#include "scanner.h"
using namespace std;

list<string> SearchFileOrDirectory::scanner(){
    list<string> listOfFileOrDir;
    Find_pathes(listOfFileOrDir, args.DIR);
    return listOfFileOrDir;
}


void SearchFileOrDirectory::Find_pathes(list<string> &FileOrDirection, const string &pathToDir, int i) {
    if(i > args.Depth && args.Depth != -1) {
        return;
    }
    for(const auto &cur_path: fs::directory_iterator(pathToDir)) {
        if(cur_path.is_directory()){
            Find_pathes(FileOrDirection, cur_path.path(), i + 1);
        }
        if(cur_path.is_directory() && args.Type != 'd' || !cur_path.is_directory() && args.Type != 't')
            continue;
        if(!cur_path.is_directory()){
            if(!(cur_path.file_size() >= args.Size_gt || args.Size_gt == -1) || !(cur_path.file_size() <= args.Size_lt || args.Size_lt == -1)){
                continue;
            }
        }
        if(regex_match((string)cur_path.path().filename(), args.REGEXPR)){
            if (!args.Full_path){
                string name = cur_path.path();
                auto findDirToPath = name.find(args.DIR);
                auto findSlash = args.DIR.rfind('/');
                if(findDirToPath != string::npos && findSlash != string::npos){
                    name.erase(0, findSlash);
                }
                name = "..." + name;
                FileOrDirection.push_back(name);
            }
            else {
                FileOrDirection.push_back(cur_path.path());
            }
        }
    }
}

list<string> ScannerFileSystem::scanner() {
    list<Direction> listDirInfo;
    RecursiveDirSize(listDirInfo, args.DIR, -1);
    listDirInfo.sort([](ScannerFileSystem::Direction &lhs, ScannerFileSystem::Direction &rhs){
        return lhs.newName < rhs.newName;
    });
    list<string> listDirAndFile;
    if (!listDirInfo.empty()) {
        int count = listDirInfo.front().RecursiveFinder(listDirAndFile);
        listDirAndFile.push_front("Количество файлов и папок: " + to_string(count));
    }
    else {
        listDirAndFile.push_front("Файлов не найдено!");
    }
    return listDirAndFile;
}

int ScannerFileSystem::RecursiveDirSize(list<Direction> &DirInfo,const string &dir, int depth){
    if(depth > args.Depth && args.Depth != -1)return 0;
    DirInfo.emplace_back(depth);
    Direction &dirInfo = DirInfo.back();
    auto find = dir.rfind('/');
    if(find != string::npos) {
        dirInfo.AddName(dir.substr(find + 1, dir.size()));
    }
    else {
        dirInfo.AddName(dir);
    }
    for (auto &cur_path : fs::directory_iterator(dir)){
        if (cur_path.is_directory()){
            dirInfo.size += RecursiveDirSize(dirInfo.listDir, cur_path.path(), depth + 1);
        }
        else {
            if ((args.Huge_files || args.Huge_files_noext) && cur_path.file_size() < 8 * 1024 * 1024) {
                continue;
            }
            if (args.Huge_files_noext || cur_path.path().filename().string().rfind('.') != string::npos) {
                dirInfo.listFile.push_back(cur_path.path().filename().string());
            }
            dirInfo.size += cur_path.file_size();
        }
    }
    return dirInfo.size;
}

int ScannerFileSystem::Direction::RecursiveFinder(list<string> &listOut){
    string path;
    for(int i = 0; i <= this->depth; i++) path += "-";
    listOut.push_back(path + NameOfDirection + " Размер: " + to_string(this->size));
    int countElement = this->listDir.size() + this->listFile.size();
    for(auto &dir : this->listDir) countElement += dir.RecursiveFinder(listOut);
    path += "-";
    for(auto &file : this->listFile) listOut.push_back(path + file);
    return countElement;
}

