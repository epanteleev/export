#pragma once

#include <windows.h>
#include "Exceptions.h"

/**
 * Represent class for access to environment variables.
 */
class Environment {
public:
    explicit Environment(HKEY key) : m_hkey(key) {}

public:
    virtual ~Environment() {
        RegCloseKey(m_hkey);
    }

public:
    /**
     * Create new instance.
     */
    static Environment open() {
        HKEY hkey;
        if (utils::checkAccess()) { // if admin
            if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"),
                             0, KEY_ALL_ACCESS, &hkey)) {
                throw WindowsError();
            }
        } else {
            if (RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Environment"), 0, KEY_ALL_ACCESS, &hkey)) {
                throw WindowsError();
            }
        }
        return Environment(hkey);
    }

public:
    /**
     * Set new variable with value.
     * If variable already exist, value will be rewrote.
     */
    bool set(const char *variable, const char *value) {
        std::string val = std::string(value).append(";");
        if (RegSetValueEx(m_hkey, variable, 0, REG_SZ,
                          reinterpret_cast<const BYTE *>(val.c_str()), val.length())) {
            throw WindowsError();
        }
        return false;
    }

    /**
     * Set new variable with given values.
     * If variable already exist, values will be rewrote.
     */
    bool set(const char *variable, const std::list<std::string> &values) {
        std::string valuesStr;
        for (auto &i: values) {
            valuesStr.append(i + ';');
        }
        if (RegSetValueEx(m_hkey, variable, 0, REG_SZ,
                          reinterpret_cast<const BYTE *>(valuesStr.c_str()), valuesStr.length())) {
            throw WindowsError();
        }
        return false;
    }

    /**
     * Get all values from given variable if they are exist.
     */
    std::string get(const char *variable) {     //Variable Name
        DWORD len = 4096;
        std::string value(len, ' ');
        LSTATUS st = RegQueryValueExA(m_hkey, variable, nullptr, nullptr,
                                      reinterpret_cast<BYTE * >(value.data()), &len);
        if (st == ERROR_FILE_NOT_FOUND) {
            throw VariableNotFound();
        } else if (st == ERROR_MORE_DATA) {
            throw DataError();
        }
        value.resize(len ? len - 1 : 0);
        return value;
    }

    /**
     * Delete given variable from the global environments with all values.
     */
    bool del(const char *variable) noexcept {
        if (RegDeleteValueA(m_hkey, variable)) {
            return true;
        }
        return false;
    }

    /**
     * Append the specified value to the variable list.
     */
    bool append(const char *variable, const char *value) {
        std::string env = get(variable);
        env.append(value).append(";");

        if (RegSetValueEx(m_hkey, variable, 0, REG_EXPAND_SZ, reinterpret_cast<const BYTE *>(env.c_str()),
                          env.length())) {
            return true;
        }
        return false;
    }

private:
    HKEY m_hkey{};
};