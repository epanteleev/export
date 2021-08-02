#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include "Commands.h"

/**
 * Command line parser representation.
 */
class CommandLineParser final {
public:
    explicit CommandLineParser(int argc, char **argv) : m_argc(argc), m_argv(argv) {}

    CommandLineParser(CommandLineParser&) = delete;

    CommandLineParser(CommandLineParser&&) = delete;

    CommandLineParser operator=(CommandLineParser&) = delete;
public:
    /**
     * Start parsing of arguments.
     */
    std::unique_ptr<AbstractCommand> apply();

private:
    void checkKeyValue() const;

private:
    int m_argc;
    char **m_argv;
};

