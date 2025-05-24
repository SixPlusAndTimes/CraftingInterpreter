#include <iostream>
#include <string>
#include <string.h>
#include <string_view>
#include <format>
#include <fstream>
#include <vector>
#include "../utils.h"
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

template<typename T>
concept StringLike = std::same_as<T, std::string> || std::same_as<T, std::string_view>;

template<StringLike T>
std::vector<std::string_view> spiltString(const T& input, const std::string& delimiter) {
    std::vector<std::string_view> result;
    std::string_view view = input;

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
std::string_view trim(const T& stringlike) {
    std::string_view view = stringlike; 
    size_t start = view.find_first_not_of(" ");
    size_t end = view.find_last_not_of(" ");
    return {view.begin() + start, view.begin() + end + 1};
}
std::string replace_all(std::string_view input, std::string_view pattern, std::string_view replacement) {
    std::string result;
    size_t pos = 0;

    while (pos < input.size()) {
        size_t found = input.find(pattern, pos);
        if (found == std::string_view::npos) {
            // 余下部分追加到结果
            result.append(input.substr(pos));
            break;
        }

        // 追加匹配前的部分
        result.append(input.substr(pos, found - pos));

        // 追加替换内容
        result.append(replacement);

        // 移动位置到匹配后
        pos = found + pattern.size();
    }

    return result;
}
void defineType(std::ofstream& astFile, std::string_view baseName, std::string_view className, std::string_view fields) {
    astFile << "\nclass " << className << " : public " << baseName << " {";
    // Constructor.
    astFile << "\n    " << className << "(" << fields << ") {";

    // Store parameters in fields.
    auto fieldvec = spiltString(fields, ", ");
    for (const auto& field : fieldvec) {
      auto name = spiltString(field, " ")[1];
      astFile << "\n      this->" << name << " = " << name << ";";
    }

    astFile << "\n    }";

    // Fields.
    astFile << "\n";
    for (const auto& field : fieldvec) {
      LOG_INFO(std::string(field));
      astFile << "    " << field << ";\n";
    }

    astFile << ("\n};");
}
void deFineAst(const std::string& outputDir, const std::string& baseName, const std::vector<std::string>& types) {
    std::string pathName = outputDir + "/" + baseName + ".cpp";
    std::string a = std::format("pathName[{}]", pathName);
    LOG_INFO(a);
    std::ofstream astFile (pathName, std::ios::out | std::ios::trunc | std::ios::binary);
    if(astFile.is_open()) {
        astFile << "#include <vector>\n#include \"token.h\"\n";
        astFile << "class " << baseName << " {\n";
        

        
        astFile << "};\n";
        for (const auto& type : types) {
            auto view_vec = spiltString(type, ":");
            std::string_view className = trim(view_vec[0]);
            std::string_view fields = trim(view_vec[1]);
            defineType(astFile, baseName, className, fields);
        }
        astFile.close();
    }else {
        LOG_ERROR("astFile Not Opened!");
    }
}
int main(int argc, char** argv) {
    if (argc != 2) {
        LOG_ERROR("Usage : GenerateAST <output directory");
        exit(1);
    }
    std::vector<std::string> types{"Binary   : Expr left, Token operater, Expr right",
                                    "Grouping : Expr expression",
                                    "Literal  : Object value",
                                    "Unary    : Token operater, Expr right"}; // operator => operater, cause "operator" is a keyword in cpp, we can not use it as parameter
    deFineAst(argv[1], "Expr", types);
    
    std::string view1 = "  whfrj :  asdasd  :cupq  asijab  ";
    auto view_vec = spiltString(view1, ":");
    std::string_view className = trim(view_vec[0]);
    std::string_view typeName = trim(view_vec[1]);
    LOG_INFO(std::string(className) + "_" + std::string(typeName));
}