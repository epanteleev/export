#pragma once

#include <stdexcept>
#include <list>

/**
 * Abstract command for execution.
 */
class AbstractCommand {
public:
    virtual ~AbstractCommand() = default;

    virtual void apply() = 0;
};

class CreateVariable : public AbstractCommand {
public:
    explicit CreateVariable(const char *variable, const char *value) :
            m_variable(variable),
            m_value(value) {}

    void apply() override;

private:
    const char *m_variable{};
    const char *m_value{};
};

class AddVariable : public AbstractCommand {
public:
    explicit AddVariable(const char *variable, const char *value) :
            m_variable(variable),
            m_value(value) {}

    void apply() override;

private:
    const char *m_variable{};
    const char *m_value{};
};

class ShowValues : public AbstractCommand {
public:
    explicit ShowValues(const char *variable) :
            m_variable(variable) {}

    void apply() override;

private:
    const char *m_variable{};
};


class DeleteValue : public AbstractCommand {
public:
    explicit DeleteValue(const char *variable, const char *value) :
            m_variable(variable),
            m_value(value) {}

public:
    void apply() override;

    void filter(std::list<std::string> &vars);

private:
    const char *m_variable{};
    const char *m_value{};
};

class ShowAllEnv: public AbstractCommand {
public:
    void apply() override;
};