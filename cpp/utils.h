#ifndef UTILLS_H
#define UTILLS_H

#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <source_location>
#include "Token.h"
template<typename T>
concept StringLike = std::same_as<T, std::string> || std::same_as<T, std::string_view>;

template<StringLike T>
std::vector<std::string> spiltString(const T& input, const std::string& delimiter) {
    std::vector<std::string> result;
    std::string view = input;

    size_t start = 0;
    while (true) {
        size_t pos = view.find(delimiter, start);
        if (pos == std::string_view::npos) {
            result.emplace_back(view.substr(start));
            break;
        }
        result.emplace_back(view.substr(start, pos - start));
        start = pos + delimiter.size();
    }
    return result;
}

template<StringLike T>
std::string trim(const T& stringlike) {
    std::string view = stringlike; 
    size_t start = view.find_first_not_of(" ");
    size_t end = view.find_last_not_of(" ");
    return {view.begin() + start, view.begin() + end + 1};
}

template<StringLike T>
std::string ToLowerCase(const T& origin) {
    std::string res{};
    for (auto c: origin) {
        res.push_back(tolower(c));
    }
    return res;
}
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

bool ObjectEquals(const Object& left, const Object& right);

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

// 全局日志等级（默认为 INFO）
inline LogLevel globalLogLevel = LogLevel::INFO;
void setLogLevel(LogLevel level);


template<typename... Args>
void Log(const std::string& levelColor,
         const LogLevel&    curLoglevel,
         const std::string& levelName,
         const std::source_location& location,
         const std::string& format,
         Args&&... args) {
    if (curLoglevel < globalLogLevel) return;
    std::string msg = std::vformat(format, std::make_format_args(args...));

    // 输出日志
    std::cout << levelColor << "[" << levelName << "]" << TerminalColor::RESET
              << " "
              << TerminalColor::CYAN << msg << TerminalColor::RESET
              << " (" << location.file_name() << ":" << location.line() << ")"
              << std::endl;
}

#define LOG_DEBUG(...)    Log(TerminalColor::CYAN,    LogLevel::DEBUG, "DEBUG",   std::source_location::current(), __VA_ARGS__)
#define LOG_INFO(...)     Log(TerminalColor::GREEN,   LogLevel::INFO, "INFO",    std::source_location::current(), __VA_ARGS__)
#define LOG_WARN(...)     Log(TerminalColor::YELLOW,  LogLevel::WARN,  "WARNING", std::source_location::current(), __VA_ARGS__)
#define LOG_ERROR(...)    Log(TerminalColor::RED,     LogLevel::ERROR,  "ERROR",   std::source_location::current(), __VA_ARGS__)

#endif