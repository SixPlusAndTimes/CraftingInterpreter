#ifndef UTILLS_H
#define UTILLS_H

#include <string>
#include <string_view>
#include <vector>

template<typename T>
concept StringLike = std::same_as<T, std::string> || std::same_as<T, std::string_view>;

// 如果传入一个临时 std::string 怎么办？
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
              << "(" << file << ":" << line << ")" << std::endl;
}

#define LOG_INFO(msg)    Log(TerminalColor::GREEN,  "INFO",    msg, __FILE__, __LINE__)
#define LOG_WARN(msg)    Log(TerminalColor::YELLOW, "WARNING", msg, __FILE__, __LINE__)
#define LOG_ERROR(msg)   Log(TerminalColor::RED,    "ERROR",   msg, __FILE__, __LINE__)
#define LOG_DEBUG(msg)   Log(TerminalColor::CYAN,   "DEBUG",   msg, __FILE__, __LINE__)
#endif