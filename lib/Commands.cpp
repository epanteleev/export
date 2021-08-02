#include <iostream>

#include "Commands.h"
#include "Env.h"

void ShowValues::apply() {
    auto env = Environment::open();
    auto [value, type] = env.get(m_variable);
    std::cout << m_variable << " : " << utils::typeToStr(type) << std::endl;
    utils::printList(value);
}

void AddVariable::apply() {
    auto env = Environment::open();
    env.append(m_variable, m_value);
}

void CreateVariable::apply() {
    auto env = Environment::open();
    env.set(m_variable, m_value);
}

void DeleteValue::apply() {
    auto env = Environment::open();
    auto [value, _] = env.get(m_variable);

    filter(value);
    if (value.empty()) {
        env.del(m_variable);
    } else {
        env.set(m_variable, value);
    }
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

void ShowAllEnv::apply() {
    auto env = Environment::open();
    PrintAll visitor;
    env.foreach(visitor);
}
