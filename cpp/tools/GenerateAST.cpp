#include <iostream>
#include <string>
#include <string.h>
#include <string_view>
#include <format>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cassert>
#include "../utils.h"

void defineType(std::ofstream& astFile, std::string baseName, std::string className, std::string fields) {
    astFile << "\nclass " << className << " : public " << baseName;
    astFile << ", public std::enable_shared_from_this<" << className << ">" ;
    astFile << " {";
    astFile << "\npublic:";
    std::cout << "defineType " << baseName << className << fields << std::endl;
    std::cout << std::format("defineType: baseName{} className{} field{}", baseName, className, fields) << std::endl;
    // spilt fieldtype and filedname
    std::vector<std::string> fieldvec = spiltString(fields, ", ");
    std::vector<std::pair<std::string, std::string>> fieldTypeNames{};
    for (const auto& field : fieldvec) {
        auto fieldSplit = spiltString(field, " ");
        fieldTypeNames.emplace_back(fieldSplit[0], fieldSplit[1]);
    }

    {
        // Constructor.
        // Constructor parameters 
        astFile << "\n    " << className << "(";
        for (size_t i = 0; i < fieldTypeNames.size(); i++) {
            astFile << "std::shared_ptr<";
            astFile << fieldTypeNames[i].first;
            astFile << "> ";
            astFile << fieldTypeNames[i].second;
            if (i != fieldTypeNames.size() - 1) {
                astFile << ", ";
            }
        }
    
        astFile << ") {";
        // Consstructor assign 
        for (auto&[fieldType, fieldName] : fieldTypeNames) {
            astFile << "\n      this->m_" << fieldName << " = " << fieldName << ";";
        }
        astFile << "\n    }";
    }

    {
        // override accept
        astFile << "\n\t\tstd::any accept (std::shared_ptr<Visitor> visitor) override{\n";
        astFile << "\t\t\treturn visitor->visit" << className << baseName << "(shared_from_this());\n";
        astFile << "\t\t}\n";
    }
    
    {
        // Field definitions
        astFile << "\n";
        for (auto&[fieldType, fieldName] : fieldTypeNames) {
            astFile << "    " << "std::shared_ptr<" << fieldType << ">" << " m_" << fieldName << ";\n";
        }
    }
    astFile << ("};");
}

void defineVisitor(std::ofstream& astFile, const std::string& baseName, const std::vector<std::string>& types) {
    astFile << "\tclass Visitor {\n" ;
    astFile << "\tpublic:\n";
    for (const auto& type : types) {
        auto view_vec = spiltString(type, "-");
        std::string typeName = trim(view_vec[0]);
        astFile << "\t\tvirtual std::any visit" << typeName << baseName << "(" 
                << "std::shared_ptr<" << typeName << ">"
                << " " << ToLowerCase(baseName) << ") = 0;\n";
    }
    // define virtual destructor
    astFile << "\t\tvirtual ~Visitor() {};\n";
    astFile << "};\n" ;
}
void deFineAst(const std::string& outputDir, const std::string& baseName, const std::vector<std::string>& types) {
    std::string pathName = outputDir + "/" + baseName + ".h";

    std::cout << outputDir << "define\n";
    
    std::ofstream astFile (pathName, std::ios::out | std::ios::trunc | std::ios::binary);
    if(astFile.is_open()) {
        const std::string headerName = ToUperCase(baseName) + "_H";
        astFile << "#ifndef "<< headerName << "\n";
        astFile << "#define "<< headerName << "\n";
        astFile << "#include <vector>\n#include <any>\n#include <memory>\n#include\"Token.h\"\n";
        // forward declare
        for (const auto& type : types) {
            auto view_vec = spiltString(type, "-");
            std::string_view className = trim(view_vec[0]);
            astFile << "class " << className << ";\n";
            std::cout << "forward declare " << className << std::endl;
        }
        if (baseName != "Expr") 
        {
            astFile << "class Expr;\n";
        }

        // base class
        astFile << "\nclass " << baseName << " {";
        astFile << "\npublic:\n";

        // define Visitor
        defineVisitor(astFile, baseName, types);

        astFile << "\tvirtual std::any accept(std::shared_ptr<Visitor> visitor) = 0;\n" ;
        astFile << "};\n";
        astFile << "\n";
        for (const auto& type : types) {
            auto view_vec = spiltString(type, "-" );
            std::string className = trim(view_vec[0]);
            std::string fields = trim(view_vec[1]);
            defineType(astFile, baseName, className, fields);
            astFile << "\n";
        }
        astFile << "\n#endif\n";
        astFile.close();
    }else {
        LOG_ERROR("astFile:{} Not Opened!", pathName);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        LOG_ERROR("Usage : GenerateAST <output directory");
        exit(1);
    }
    // std::vector<std::string> types{"Binary   : Expr left, Token operater, Expr right",
    //                                 "Grouping : Expr expression",
    //                                 "Literal  : Object value",
    //                                 "Unary    : Token operater, Expr right"}; // operator => operater, cause "operator" is a keyword in cpp, we can not use it as parameter
    deFineAst(argv[1], "Expr", {"Assign     - Token name, Expr value",
                                "Binary     - Expr left, Token operater, Expr right",
                                "Grouping   - Expr expression",
                                "Literal    - Object value",
                                "Logical    - Expr left, Token operater, Expr right",
                                "Unary      - Token operater, Expr right", // operator => operater, cause "operator" is a keyword in cpp, we can not use it as parameter
                                "Variable   - Token name"});
    deFineAst(argv[1], "Stmt", {"Block      - std::vector<std::shared_ptr<Stmt>> statements",
                                "Expression - Expr expression",
                                "If         - Expr condition, Stmt thenBranch, Stmt elseBranch",
                                "Print      - Expr expression",
                                "Var        - Token name, Expr initializer"});
    // std::string view1 = "  whfrj :  asdasd  :cupq  asijab  ";
    // auto view_vec = spiltString(view1, ":");
    // std::string_view className = trim(view_vec[0]);
    // std::string_view typeName = trim(view_vec[1]);
    // LOG_INFO(std::string(className) + "_" + std::string(typeName));
}