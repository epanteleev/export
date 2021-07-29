#include <stdexcept>
#include <iostream>

#include "CommandLineParser.h"
#include "utils.h"


void CommandLineParser::checkKeyValue() const {
    if (m_argc == 2) {
        throw std::invalid_argument("expect variable name");
    } else if (m_argc == 3) {
        throw std::invalid_argument("expect value");
    }
}

std::unique_ptr<AbstractCommand> CommandLineParser::apply() {
    ASSERT(m_argc >= 2, "internal error");
    if (strcmp(m_argv[1], "--add") == 0 ||
        strcmp(m_argv[1], "-a") == 0) { // <program name> add <key> <value>

        checkKeyValue();
        return std::make_unique<AddVariable>(m_argv[2], m_argv[3]);
    } else if (strcmp(m_argv[1], "--list") == 0 ||
               strcmp(m_argv[1], "-l") == 0) {

        if (m_argc == 2) {
            throw std::invalid_argument("expect variable name");
        }
        return std::make_unique<ShowValues>(m_argv[2]);
    } else if (strcmp(m_argv[1], "--make") == 0 ||
               strcmp(m_argv[1], "-m") == 0) {

        checkKeyValue();
        return std::make_unique<CreateVariable>(m_argv[2], m_argv[3]);
    } else if (strcmp(m_argv[1], "--remove") == 0 ||
               strcmp(m_argv[1], "-r") == 0) {

        checkKeyValue();
        return std::make_unique<DeleteValue>(m_argv[2], m_argv[3]);
    }
    throw std::invalid_argument("no match");
}
