#include <iostream>

#include "Visitors.h"
#include "utils.h"

void PrintAll::visitRegExpandSz(const char *variable, std::list<std::string> &&values) {
    std::cout << variable << " : " << utils::typeToStr(REG_EXPAND_SZ) << std::endl;
    utils::printList(values);
    std::cout << std::endl;
}

void PrintAll::visitRegSz(const char *variable, std::list<std::string>&& values) {
    std::cout << variable << " : " << utils::typeToStr(REG_SZ) << std::endl;
    utils::printList(values);
    std::cout << std::endl;
}