#include <iostream>

#include "CommandLineParser.h"

void help() {
    std::cout << "\nExport environment variable\n\n"
                 "USAGE:\n"
                 "\texport [OPTIONS] <variable> <value>\n\n"
                 "OPTIONS:\n"
                 "\t-l, --list <variable>               List of values of variable\n"
                 "\t-a, --add <variable> <value>        Add value in variable\n"
                 "\t-r, --remove <variable> <value>     Remove value from variable\n"
                 "\t-m, --make <variable> <value>       Create new variable with initial value\n"
                 << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        help();
        return 0;
    }
    try {
        auto st = CommandLineParser(argc, argv).apply();
        st->apply();
    } catch (std::exception &error) {
        std::cout <<"Error: " << error.what() << std::endl;
    }
    return 0;
}