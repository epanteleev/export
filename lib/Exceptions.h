#pragma once

#include "utils.h"


class WindowsError : public std::exception {
public:
    WindowsError() : m_error(utils::getLastError()) {}

    [[nodiscard]]
    const char *what() const noexcept override {
        return m_error.c_str();
    }

private:
    std::string m_error;
};

class DataError : public std::exception {
public:
    [[nodiscard]]
    const char *what() const noexcept override {
        return "variable is so long size";
    }
};

class VariableNotFound : public std::exception {
public:
    [[nodiscard]]
    const char *what() const noexcept override {
        return "variable not found";
    }
};


class BadIndex : public std::exception {
public:
    explicit BadIndex(std::string&& idx) : message("bad index \"" + idx + "\"") {}

    [[nodiscard]]
    const char *what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};