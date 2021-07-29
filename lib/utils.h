#pragma once

#include <string>
#include <list>
#include "cassert"

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
}