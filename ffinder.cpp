#include <exception>
#include <iostream>
#include <string>
#include "scanner.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    cin.tie(nullptr);
    try {
        arguments_list parsed_args = parse_args(argc, argv);
        Scanner* scanner = nullptr;
        if(parsed_args.Info) {
            scanner = new ScannerFileSystem(parsed_args);
        }
        else {
            scanner = new SearchFileOrDirectory(parsed_args);
        }
        auto listResult = scanner->scanner();
        for(auto &par : listResult) {
            cout << par << '\n';
        }
    }
    catch (exception &e) {
        cout << "Error: " << e.what() << '\n';
    }
    return 0;
}
