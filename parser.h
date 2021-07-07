#pragma once
#include<QCommandLineParser>
#include<QScopedPointer>
#include"arguments_list.h"

arguments_list parse_args(int argc, char *argv[]);