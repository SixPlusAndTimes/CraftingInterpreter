#ifndef UTILLS_H
#define UTILLS_H

#include <string>
#include <string_view>
enum class CLoxResult {
    SUCCESS,
    FAILED,
};
CLoxResult DumpBuffer(const std::string& buffer);
CLoxResult ReadAllBytesFromFile(std::string_view fileName, std::string& buffer);

namespace TerminalColor {
    constexpr const char* RESET   = "\033[0m";
    constexpr const char* RED     = "\033[31m";
    constexpr const char* GREEN   = "\033[32m";
    constexpr const char* YELLOW  = "\033[33m";
    constexpr const char* BLUE    = "\033[34m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* CYAN    = "\033[36m";
    constexpr const char* WHITE   = "\033[37m";
    constexpr const char* BOLD    = "\033[1m";
}

inline void Log(const std::string& levelColor, const std::string& levelName,
         const std::string& msg, const char* file, int line) {
    std::cout << levelColor << "[" << levelName << "]" << TerminalColor::RESET
              << " "
              << TerminalColor::CYAN << msg << TerminalColor::RESET
              << "  (" << file << ":" << line << ")" << std::endl;
}

// 宏封装
#define LOG_INFO(msg)    Log(TerminalColor::GREEN,  "INFO",    msg, __FILE__, __LINE__)
#define LOG_WARN(msg)    Log(TerminalColor::YELLOW, "WARNING", msg, __FILE__, __LINE__)
#define LOG_ERROR(msg)   Log(TerminalColor::RED,    "ERROR",   msg, __FILE__, __LINE__)
#define LOG_DEBUG(msg)   Log(TerminalColor::CYAN,   "DEBUG",   msg, __FILE__, __LINE__)
#endif