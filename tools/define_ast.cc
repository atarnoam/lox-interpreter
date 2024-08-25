#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <locale>
#include <optional>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace fs = std::filesystem;

std::string to_lower(std::string s) {
    for (char &c : s) {
        c = std::tolower(c);
    }
    return s;
}

// trim from start (in place)
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

using SubclassName = std::string;
using FieldType = std::string;
using FieldName = std::string;
using Field = std::pair<FieldType, FieldName>;
using SubclassData = std::pair<SubclassName, std::vector<Field>>;

SubclassData parse_string(const std::string &subclass_str) {
    std::stringstream subclass_stream(subclass_str);
    SubclassName subclass_name;
    std::getline(subclass_stream, subclass_name, ':');
    trim(subclass_name);
    std::string field_str;
    std::vector<Field> fields;
    while (std::getline(subclass_stream, field_str, ',')) {
        trim(field_str);
        size_t delim_idx = field_str.find(' ');
        FieldType field_type = field_str.substr(0, delim_idx);
        FieldName field_name = field_str.substr(delim_idx + 1);
        fields.emplace_back(field_type, field_name);
    };
    return {subclass_name, fields};
}

std::vector<SubclassData>
parse_subclasses(const std::vector<std::string> &subclass_strs) {
    std::vector<SubclassData> subclasses;
    std::transform(subclass_strs.begin(), subclass_strs.end(),
                   std::back_inserter(subclasses), parse_string);
    return subclasses;
}

struct AstData {
    AstData(const std::string &base_name,
            std::vector<std::string> subclass_strs)
        : base_name(base_name),
          subclasses(parse_subclasses(std::move(subclass_strs))),
          visitor_name(fmt::format("{}Visitor", base_name)) {}

    std::string base_name;
    std::vector<SubclassData> subclasses;
    std::string visitor_name;
};

void define_base_class(std::ostream &os, const AstData &ast_data) {
    os << fmt::format(
        R"###(struct {0} {{ 
    virtual void accept({1} &visitor) const = 0;
    
    {0}() = default;
    {0}(const {0} &) = default;
    {0} &operator=(const {0} &) = default;
    {0}({0} &&) = default;
    {0} &operator=({0} &&) = default;
    
    virtual ~{0}() = default;

)###",
        ast_data.base_name, ast_data.visitor_name);
    for (const auto &[subclass_name, _] : ast_data.subclasses) {
        os << fmt::format("    struct {};", subclass_name) << std::endl;
    }
    os << "};" << std::endl << std::endl;
}

void define_subclass(std::ostream &os_h, std::ostream &os_cc,
                     const std::string &base_class,
                     const std::string &visitor_name,
                     const SubclassData subclass_data) {
    auto [subclass_name, fields] = subclass_data;

    const std::string &scoped_name =
        fmt::format("{}::{}", base_class, subclass_name);

    for (auto &[field_type, field_name] : fields) {
        if (field_type == base_class) {
            field_type = fmt::format("std::shared_ptr<{}>", field_type);
        }
    }
    os_h << fmt::format("struct {} : {} {{", scoped_name, base_class)
         << std::endl;

    // Declare constructor
    os_h << fmt::format("    {}(", subclass_name);

    bool first_iter = true;
    for (const auto &field : fields) {
        if (!first_iter) {
            os_h << ", ";
        }
        os_h << field.first << " " << field.second;
        first_iter = false;
    }
    os_h << ");" << std::endl;

    // Define constructor
    os_cc << fmt::format("{}::{}(", scoped_name, subclass_name);
    first_iter = true;
    for (const auto &[field_type, field_name] : fields) {
        if (!first_iter) {
            os_cc << ", ";
        }
        os_cc << field_type << " " << field_name;
        first_iter = false;
    }
    os_cc << ") : ";

    first_iter = true;
    for (const auto &[field_type, field_name] : fields) {
        if (!first_iter) {
            os_cc << ", ";
        }
        os_cc << fmt::format("{0}(std::move({0}))", field_name);
        first_iter = false;
    }
    os_cc << "{}" << std::endl << std::endl;

    // Declare visitor accept
    os_h << fmt::format("    virtual void accept({}& visitor) const override;",
                        visitor_name)
         << std::endl;

    // Define visitor accept
    os_cc << fmt::format(R"###(void {}::accept({}& visitor) const {{
    visitor.visit_{}_{}(*this);
}})###",
                         scoped_name, visitor_name, to_lower(subclass_name),
                         to_lower(base_class))
          << std::endl
          << std::endl;

    // Define subclass fields
    for (const auto &[field_type, field_name] : fields) {
        os_h << "    " << field_type << " " << field_name << ";" << std::endl;
    }
    os_h << "};" << std::endl << std::endl;
}

void define_subclasses(std::ostream &os_h, std::ostream &os_cc,
                       const AstData &ast_data) {
    for (const SubclassData &subclass_data : ast_data.subclasses) {
        define_subclass(os_h, os_cc, ast_data.base_name, ast_data.visitor_name,
                        subclass_data);
    }
}

void define_visitor(std::ostream &os_h, std::ostream &os_cc,
                    const AstData &ast_data) {
    os_h << fmt::format("struct {} {{", ast_data.visitor_name) << std::endl;
    for (const auto &[subclass_name, _] : ast_data.subclasses) {
        os_h << fmt::format(
                    "    virtual void visit_{0}_{2}(const {3}::{1} &{2}) = 0;",
                    to_lower(subclass_name), subclass_name,
                    to_lower(ast_data.base_name), ast_data.base_name)
             << std::endl;
    }
    os_h << "};" << std::endl << std::endl;
}

void define_ast(const fs::path &file_h, const fs::path &file_cc,
                const std::string &base_name,
                const std::vector<std::string> &subclass_strs,
                const std::vector<std::string> &includes = {},
                const std::optional<fs::path> &file_fwd_h = std::nullopt) {
    std::ofstream ofile_h(file_h);
    std::ofstream ofile_cc(file_cc);

    AstData ast_data(base_name, subclass_strs);

    // Define .fwd.h
    if (file_fwd_h.has_value()) {
        std::ofstream ofile_fwd_h(file_fwd_h.value());
        ofile_fwd_h << "#pragma once" << std::endl << std::endl;
        ofile_fwd_h << fmt::format("struct {};", base_name) << std::endl;
    }

    // Headers of .h
    ofile_h << "#pragma once" << std::endl;
    if (file_fwd_h.has_value()) {
        ofile_h << fmt::format("#include \"src/syntactics/{}.fwd.h\"",
                               to_lower(ast_data.base_name))
                << std::endl;
    }
    for (const auto &include : includes) {
        ofile_h << fmt::format("#include {}", include) << std::endl;
    }
    ofile_h << "#include <memory>" << std::endl << std::endl;

    // Headers of .cc
    ofile_cc << fmt::format("#include \"src/syntactics/{}.h\"",
                            to_lower(ast_data.base_name))
             << std::endl
             << std::endl;

    // Declare visitor class
    ofile_h << fmt::format("struct {};", ast_data.visitor_name) << std::endl
            << std::endl;

    define_base_class(ofile_h, ast_data);

    define_subclasses(ofile_h, ofile_cc, ast_data);

    define_visitor(ofile_h, ofile_cc, ast_data);
}

void define_and_format_ast(fs::path output_dir, const std::string &base_name,
                           const std::vector<std::string> &subclass_strs,
                           const std::vector<std::string> &includes = {},
                           bool define_fwd = false) {
    output_dir = output_dir / "syntactics";
    std::cout << output_dir << std::endl;
    fs::path file_h = output_dir / (to_lower(base_name) + ".h");
    fs::path file_cc = output_dir / (to_lower(base_name) + ".cc");
    std::optional<fs::path> file_fwd_h =
        define_fwd ? std::optional<fs::path>(
                         (output_dir / (to_lower(base_name) + ".fwd.h")))
                   : std::nullopt;

    define_ast(file_h, file_cc, base_name, subclass_strs, includes, file_fwd_h);

    // Format files
    std::system(fmt::format("clang-format -i {}", file_h.string()).c_str());
    std::system(fmt::format("clang-format -i {}", file_cc.string()).c_str());
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: define_ast [output dir]" << std::endl;
        return 1;
    }
    std::string output_dir = argv[1];
    define_and_format_ast(
        output_dir, "Expr",
        {
            "Assign   : Token name, Expr value",
            "Binary   : Expr left, Token op, Expr right",
            "Call     : Expr callee, Token paren, "
            "std::vector<std::shared_ptr<Expr>> arguments",
            "Get        : Expr object, Token name",
            "Grouping : Expr expression",
            "Lambda   : Token keyword, std::vector<Token> params, "
            "std::vector<std::shared_ptr<Stmt>> body",
            "Literal  : Token value",
            "Logical  : Expr left, Token op, Expr right",
            "Set      : Expr object, Token name, Expr value",
            "Super    : Token keyword, Token method",
            "This     : Token keyword",
            "Unary    : Token op, Expr right",
            "Variable : Token name",
        },
        {
            "\"src/syntactics/token.h\"",
            "\"src/syntactics/stmt.fwd.h\"",
            "<vector>",
        });

    define_and_format_ast(
        output_dir, "Stmt",
        {
            "Block      : std::vector<std::shared_ptr<Stmt>> statements",
            "Class      : Token name, std::shared_ptr<Expr::Variable> "
            "superclass, std::vector<std::shared_ptr<Stmt::Function>> methods",
            "Expression : std::shared_ptr<Expr> expression",
            "If         : std::shared_ptr<Expr> condition, Stmt then_branch,"
            "   Stmt else_branch",
            "Function   : Token name, std::vector<Token> params, "
            "std::vector<std::shared_ptr<Stmt>> body",
            "Print      : std::shared_ptr<Expr> expression",
            "Return     : Token keyword, std::shared_ptr<Expr> value",
            "Var        : Token name, std::shared_ptr<Expr> initializer",
            "While      : std::shared_ptr<Expr> condition, Stmt body",
        },
        {
            "\"src/syntactics/expr.h\"",
            "<vector>",
        },
        true);
}