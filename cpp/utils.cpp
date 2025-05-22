
#include <iostream>
#include <fstream>
#include <format>
#include "utils.h"

CLoxResult DumpBuffer(const std::string& buffer) {
    if (buffer.size() == 0) {
        std::cerr << "DumpBuffer Fail: buffersize is 0!\n";
        return CLoxResult::FAILED;
    }

    std::cout << "------buffer content:\n" << buffer << "\n ----content End\n";
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
