#include <iostream>
#include <string>
#include <string.h>
#include <string_view>
#include <format>
#include <fstream>
#include <vector>
#include "../utils.h"

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
    // override accept
    astFile << "\n\t\tstd::any accept (Visitor& visitor) override{\n";
    astFile << "\t\t\treturn visitor.visit" << className << baseName << "(*this);\n";
    astFile << "\t\t}\n";
    // Fields.
    astFile << "\n";
    for (const auto& field : fieldvec) {
      LOG_INFO(std::string(field));
      astFile << "    " << field << ";\n";
    }

    astFile << ("\n};");
}

void defineVisitor(std::ofstream& astFile, const std::string& baseName, const std::vector<std::string>& types) {
    astFile << "class Visitor {\n" ;
    astFile << "public:";
    for (const auto& type : types) {
        auto view_vec = spiltString(type, ":");
        std::string_view typeName = trim(view_vec[0]);
        astFile << "\tvirtual std::any visit" << typeName << baseName << "(" << typeName <<"&"
                << " " << baseName << ");\n";
    }
    astFile << "};\n" ;
}
void deFineAst(const std::string& outputDir, const std::string& baseName, const std::vector<std::string>& types) {
    std::string pathName = outputDir + "/" + baseName + ".cpp";

    std::ofstream astFile (pathName, std::ios::out | std::ios::trunc | std::ios::binary);
    if(astFile.is_open()) {
        astFile << "#include <vector>\n#include <any>\n#include\"token.h\"\n";
        // namespace
        astFile << "namespace Expr {\n";
        // Visitor
        defineVisitor(astFile, baseName, types);
        // base class
        astFile << "class " << baseName << " {\n";
        astFile << "\tvirtual std::any accept(Visitor& visitor);\n" ;
        astFile << "};\n";

        for (const auto& type : types) {
            auto view_vec = spiltString(type, ":");
            std::string_view className = trim(view_vec[0]);
            std::string_view fields = trim(view_vec[1]);
            defineType(astFile, baseName, className, fields);
        }
        astFile << "} //namespace Expr";
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