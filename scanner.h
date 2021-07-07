#pragma once
#include <string>
#include <memory>
#include <filesystem>
#include <list>
#include <algorithm>
#include <regex>
#include "arguments_list.h"

using namespace std;
namespace fs = filesystem;

class Scanner {
protected:
    arguments_list args;
public:
    explicit Scanner(arguments_list prop): args(prop){}

    virtual list<string> scanner() = 0;
};

class SearchFileOrDirectory: public Scanner{
    void Find_pathes(list<string>& FileOrDirection, const string& path, int i = 0);

public:
    explicit SearchFileOrDirectory(const arguments_list& pr): Scanner(pr){}

    list<string> scanner() final;
};

class ScannerFileSystem: public Scanner {
    struct Direction {
        int depth;
        int size;
        string NameOfDirection;
        string newName;
        list<Direction> listDir;
        list<string> listFile;

        explicit Direction(int d = 0, int s = 0,const string& n = ""): depth(d), size(s), NameOfDirection(n){
            transform(NameOfDirection.begin(), NameOfDirection.end(), newName.begin(), ::tolower);
        }

        void AddName(const string& N){
            NameOfDirection = N;
            newName = NameOfDirection;
            transform(newName.begin(), newName.end(), newName.begin(), ::tolower);
        }

        int RecursiveFinder(list<string>& recout);
    };

    int RecursiveDirSize(list<Direction>& directions, const string& s, int i = 0 );

public:
    explicit ScannerFileSystem(const arguments_list& pr): Scanner(pr){}

    list<string> scanner() override;
};