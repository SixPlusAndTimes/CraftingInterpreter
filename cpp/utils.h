#include <string>
#include <string_view>
#ifndef UTILLS_H
#define UTILLS_H
enum class CLoxResult {
    SUCCESS,
    FAILED,
};
CLoxResult DumpBuffer(const std::string& buffer);
CLoxResult ReadAllBytesFromFile(std::string_view fileName, std::string& buffer);
#endif