#include "Env.h"

void Environment::foreach(AbstractVisitor &visitor) {
    DWORD valueCount{}, maxValueLength{}, maxNameLength{};
    if (RegQueryInfoKey(m_hkey, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                        &valueCount, &maxNameLength, &maxValueLength, nullptr, nullptr)) {
        throw WindowsError();
    }

    auto valueName = std::vector<char>(maxNameLength + 1);
    auto dataBuffer = std::vector<BYTE>(maxValueLength);

    for (DWORD index = 0; index < valueCount; index++) {

        auto charCountValueName = static_cast<DWORD>(valueName.size());
        auto charBytesData = static_cast<DWORD>(dataBuffer.size());
        DWORD type{};

        if (RegEnumValue(m_hkey, index, reinterpret_cast<LPSTR>(valueName.data()), &charCountValueName,
                         nullptr, &type, dataBuffer.data(), &charBytesData)) {
            throw WindowsError();
        }

        const auto regString = reinterpret_cast<char *>(dataBuffer.data());
        if (type == REG_SZ) {

            visitor.visitRegSz(valueName.data(), utils::split(regString, ';'));
        } else if (type == REG_EXPAND_SZ) {

            TCHAR buffer[32000];
            ExpandEnvironmentStrings(reinterpret_cast<LPCSTR>(regString), buffer, 32000); //Todo
            visitor.visitRegExpandSz(valueName.data(), utils::split(buffer, ';'));
        } else {
           ASSERT(false, "undefined type");
        }
    }
}

bool Environment::append(const char *variable, const char *value) {
    auto [env, _] = internalGet(variable);
    env.append(value).append(";");

    if (RegSetValueEx(m_hkey, variable, 0, REG_EXPAND_SZ, reinterpret_cast<const BYTE *>(env.c_str()),
                      env.length())) {
        return true;
    }
    return false;
}

bool Environment::del(const char *variable) noexcept {
    if (RegDeleteValueA(m_hkey, variable)) {
        return true;
    }
    return false;
}

std::tuple<std::string, DWORD> Environment::internalGet(const char *variable) {
    DWORD len = 4096;
    std::string value(len, ' ');
    DWORD type;
    LSTATUS st = RegQueryValueExA(m_hkey, variable, nullptr, &type,
                                  reinterpret_cast<BYTE * >(value.data()), &len);
    if (st == ERROR_FILE_NOT_FOUND) {
        throw VariableNotFound();
    } else if (st == ERROR_MORE_DATA) {
        throw DataError();
    }
    value.resize(len ? len - 1 : 0);
    return std::make_tuple(value, type);
}

std::tuple<std::list<std::string>, DWORD> Environment::get(const char *variable) {
    auto [value, type] = internalGet(variable);
    return std::make_tuple(utils::split(value, ';'), type);
}

bool Environment::set(const char *variable, const std::list<std::string> &values) {
    std::string valuesStr;
    for (auto &i: values) {
        valuesStr.append(i + ';');
    }
    if (RegSetValueEx(m_hkey, variable, 0, REG_EXPAND_SZ,
                      reinterpret_cast<const BYTE *>(valuesStr.c_str()), valuesStr.length())) {
        throw WindowsError();
    }
    return false;
}

bool Environment::set(const char *variable, const char *value) {
    std::string val = std::string(value).append(";");
    if (RegSetValueEx(m_hkey, variable, 0, REG_SZ,
                      reinterpret_cast<const BYTE *>(val.c_str()), val.length())) {
        throw WindowsError();
    }
    return false;
}

Environment Environment::open() {
    HKEY hkey;
    if (utils::checkAccess()) { // if admin
        if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                         TEXT("SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment"),
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

Environment::~Environment() {
    RegCloseKey(m_hkey);
}