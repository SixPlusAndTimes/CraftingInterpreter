
#include <iostream>
#include <fstream>
#include <format>
#include <vector>
#include <string.h>
#include "utils.h"

static const double EPSILON = 1e-9;
std::ostream& operator<<(std::ostream& os, const std::string_view& sv) {
    return os.write(sv.data(), sv.size());
}

char* operator+(const char* left, const std::string_view right) {
    size_t new_size = strlen(left) + right.size() + 1;
    char* res = new char[new_size];
    memcpy(res, left, strlen(left));
    memcpy(res + strlen(left), right.data(), right.size());
    res[new_size] = '\0';
    return res;
}


std::string replace_all(std::string_view input, std::string_view pattern, std::string_view replacement) {
    std::string result;
    size_t pos = 0;
    while (pos < input.size()) {
        size_t found = input.find(pattern, pos);
        if (found == std::string_view::npos) {
            result.append(input.substr(pos));
            break;
        }
        result.append(input.substr(pos, found - pos));
        result.append(replacement);
        pos = found + pattern.size();
    }

    return result;
}

CLoxResult DumpBuffer(const std::string& buffer) {
    if (buffer.size() == 0) {
        std::cerr << "DumpBuffer Fail: buffersize is 0!\n";
        return CLoxResult::FAILED;
    }

    return CLoxResult::SUCCESS;
}
// std::string& buffer : out param 
CLoxResult ReadAllBytesFromFile(std::string_view fileName, std::string& buffer) {
    CLoxResult result{CLoxResult::SUCCESS};
    std::ifstream in(fileName.data(), std::ios::in | std::ios::binary);
    if (in.is_open()) {
        in.seekg(0, std::ios::end);
        buffer.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(buffer.data(), buffer.size());
        in.close();
    }else {
       std::cerr << std::format("Fail to open file[{}] \n", fileName);
       result = CLoxResult::FAILED;
    }
    return result;
}

bool ObjectEquals(const Object& left, const Object& right) {
    LOG_DEBUG("ObjectEqual: start");
    if (std::holds_alternative<nullptr_t>(left) && std::holds_alternative<nullptr_t>(right)) {
        return true;
    }

    if (std::holds_alternative<nullptr_t>(left) 
        || std::holds_alternative<nullptr_t>(right)) {
        return false;
    }

    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
        LOG_DEBUG("ObjectEqual: compare double");
        return std::abs(std::get<double>(left) - std::get<double>(right)) < EPSILON;
    } else if (std::holds_alternative<bool>(left) && std::holds_alternative<bool>(right)) {
        LOG_DEBUG("ObjectEqual: compare boolean left:{} right:{}", std::get<bool>(left), std::get<bool>(right));
        return std::get<bool>(left) == std::get<bool>(right);
    } else if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
        LOG_DEBUG("ObjectEqual: compare compare string");
        return std::get<std::string>(left) == std::get<std::string>(left);
    } else {
    }
    LOG_DEBUG("ObjectEqual: END, Compare values that can not compare, return false");
    return false;
}

void setLogLevel(LogLevel level) {
    globalLogLevel = level;
}