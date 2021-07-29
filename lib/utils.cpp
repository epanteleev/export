#include <windows.h>
#include <sstream>
#include "utils.h"
#include "Exceptions.h"

namespace utils {
    std::string getLastError() noexcept {
//Get the error message ID, if any.
        DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0) {
            return std::string(); //No error message has been recorded
        }

        LPSTR messageBuffer = nullptr;

        size_t size = FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr, errorMessageID,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR) &messageBuffer, 0, nullptr);

        std::string message(messageBuffer, size);

        LocalFree(messageBuffer);

        return message;
    }

    bool checkAccess() {
        PSID administratorsGroup = nullptr;
        SID_IDENTIFIER_AUTHORITY SIDAuthNT =
                SECURITY_NT_AUTHORITY;

        if (!AllocateAndInitializeSid(&SIDAuthNT, 2,
                                      SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
                                      0, 0, 0, 0, 0, 0,
                                      &administratorsGroup)) {
            throw WindowsError();
        }

        BOOL isMember;
        if (!CheckTokenMembership(nullptr, administratorsGroup, &isMember)) {
            throw WindowsError();
        }

        return isMember;
    }

    std::list<std::string> split(const std::string &s, char delimiter) {
        std::list<std::string> result;
        std::stringstream ss(s);
        std::string item;

        while (getline(ss, item, delimiter)) {
            if (item.empty()) continue;
            result.push_back(item);
        }

        return result;
    }
}