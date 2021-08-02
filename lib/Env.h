#pragma once

#include <minwindef.h>
#include <vector>
#include <tuple>
#include "Exceptions.h"
#include "Visitors.h"

/**
 * Represent class for access to environment variables.
 */
class Environment final {
public:
    Environment(Environment &) = delete;

    Environment(Environment &&other) noexcept:
            m_hkey(std::exchange(other.m_hkey, nullptr)) {}

    Environment operator=(Environment &) = delete;

private:
    explicit Environment(HKEY key) : m_hkey(key) {}

public:
    ~Environment();

public:
    /**
     * Create new instance.
     */
    static Environment open();

public:
    /**
     * Set new variable with value.
     * If variable already exist, value will be rewrote.
     */
    bool set(const char *variable, const char *value);

    /**
     * Set new variable with given values.
     * If variable already exist, values will be rewrote.
     */
    bool set(const char *variable, const std::list<std::string> &values);

    /**
     * Get all values from given variable if they are exist.
     * @return pair of value's list and type of variable.
     */
    std::tuple<std::list<std::string>, DWORD> get(const char *variable);

    /**
     * Delete given variable from the global environments with all values.
     */
    bool del(const char *variable) noexcept;

    /**
     * Append the specified value to the variable list.
     */
    bool append(const char *variable, const char *value);

    /**
     * Traversal of the whole data structure.
     * @param visitor is derived class of AbstractVisitor.
     */
    void foreach(AbstractVisitor &visitor);

private:
    std::tuple<std::string, DWORD> internalGet(const char *variable);

private:
    HKEY m_hkey{};
};