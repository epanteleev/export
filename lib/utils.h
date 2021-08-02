#pragma once

#include <string>
#include <list>
#include <cassert>
#include <minwindef.h>

#ifdef NDEBUG
#define ASSERT(condition, message)
#else
#define ASSERT(condition, message)                                             \
      do                                                                       \
      {                                                                        \
        if (!(condition))                                                      \
        {                                                                      \
          fprintf(stderr, "[%s:%d] Assert failed in %s(): %s\n",               \
              __FILE__, __LINE__, __func__, message);                          \
          abort();                                                             \
        }                                                                      \
      } while (false)
#endif

namespace utils {
    /**
    * Getting current last windows internal error as std::string.
    */
    std::string getLastError() noexcept;
    /**
     * @return true if current user is administrator.
     */
    bool checkAccess();

    /**
     * Split given string to list of substrings with delimiter.
     */
    std::list<std::string> split(const std::string &s, char delimiter);

    /**
     * Formatted print list of strings.
     */
    void printList(const std::list<std::string> &values);

    std::list<std::string> toListOfStrings(char* data, int size);

    constexpr const char *typeToStr(DWORD type) {
        switch (type) {
            case REG_SZ: return "REG_SZ";
            case REG_EXPAND_SZ: return "REG_EXPAND_SZ";
            case REG_MULTI_SZ: return "REG_MULTI_SZ";
            case REG_DWORD: return "REG_DWORD";
            case REG_QWORD: return "REG_QWORD";
            default: ASSERT(false, "unknown type");
        }
    }
}