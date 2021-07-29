#include <iostream>

#include "Commands.h"
#include "Env.h"

void printEnv(const std::string &value) {
    auto vars = utils::split(value, ';');
    std::size_t c = 0;
    for (auto &i: vars) {
        std::cout << "[" << c << "]\t" << i << std::endl;
        c += 1;
    }
}

bool ShowValues::apply() {
    auto env = Environment::open();
    const auto value = env.get(m_variable);

    printEnv(value);
    return true;
}

bool AddVariable::apply() {
    auto env = Environment::open();
    env.append(m_variable, m_value);
    return true;
}

bool CreateVariable::apply() {
    auto env = Environment::open();
    env.set(m_variable, m_value);
    return true;
}

bool DeleteValue::apply() {
    auto env = Environment::open();
    const auto value = env.get(m_variable);
    auto vars = utils::split(value, ';');

    filter(vars);
    if (vars.empty()) {
        env.del(m_variable);
        return true;
    }

    env.set(m_variable, vars);
    return false;
}

bool isPattern(const std::string& s) {
    return s[0] == '%';
}

int getPattern(const char* s) {
    try {
        return std::stoi(s + 1);
    } catch (std::invalid_argument& ex) {
        throw BadIndex(s);
    }
}

void DeleteValue::filter(std::list<std::string>& vars) {
    if (!isPattern(m_value)) {
        vars.remove_if([&](auto &i) { return i == m_value; });
        return;
    }
    auto idx = getPattern(m_value);
    int count = -1;
    vars.remove_if([&](auto &i) {
        count += 1;
        return count == idx;
    });
}

