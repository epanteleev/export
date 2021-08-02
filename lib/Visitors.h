#pragma once
#include <list>
#include <string>
#include <evr.h>

/**
 * Class for each visit of variable in the Environment class @see{lib/Env.h}.
 */
class AbstractVisitor {
public:
    AbstractVisitor() = default;
    virtual ~AbstractVisitor() = default;

public:
    virtual void visitRegSz(const char *variable, std::list<std::string> && values) = 0;
    virtual void visitRegExpandSz(const char* variable, std::list<std::string>&& values) = 0;
};

/**
 * Print all variables and their values them.
 */
class PrintAll : public AbstractVisitor {
public:
    PrintAll() = default;

public:
    void visitRegSz(const char *variable, std::list<std::string> && values) override;
    void visitRegExpandSz(const char *variable, std::list<std::string> &&values) override;
};