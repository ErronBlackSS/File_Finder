#pragma once
#include<string>
#include<regex>

struct arguments_list{
    std::string DIR;
    std::string EXPR;
    std::regex REGEXPR;
    int Depth = -1;
    bool Exact = false;
    bool Full_path = false;
    bool Relative_path = false;
    char Type = ' ';
    bool Case_insensitive = false;
    bool regex = false;
    int Size_gt = -1;
    int Size_lt = -1;
    bool Info = false;
    bool Huge_files = false;
    bool Huge_files_noext = false;
};