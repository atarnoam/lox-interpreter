#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>
#include <locale>
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

void define_base_class(std::ostream &os, const std::string &base_name,
                       const std::string &visitor_name) {
    os << fmt::format(
        R"###(struct {0} {{ 
    virtual void accept({1} &visitor) const = 0;
    
    {0}() = default;
    {0}(const {0} &) = default;
    {0} &operator=(const {0} &) = default;
    {0}({0} &&) = default;
    {0} &operator=({0} &&) = default;
    
    virtual ~{0}() = default;
}};)###",
        base_name, visitor_name);
    // os << fmt::format(BASE_CLASS_FMT, base_name);
    os << std::endl << std::endl;
}

using SubclassName = std::string;
using FieldType = std::string;
using FieldName = std::string;
using Field = std::pair<FieldType, FieldName>;

void define_subclass(std::ostream &os_h, std::ostream &os_cc,
                     const std::string &base_class,
                     const std::string &visitor_name,
                     const SubclassName &subclass_name,
                     std::vector<Field> fields) {
    for (auto &[field_type, field_name] : fields) {
        if (field_type == base_class) {
            field_type = std::string("std::unique_ptr<") + field_type + ">";
        }
    }
    os_h << fmt::format("struct {} : {} {{", subclass_name, base_class)
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
    os_cc << fmt::format("{0}::{0}(", subclass_name);
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
    os_h << fmt::format("    virtual void accept({}& visitor) const;",
                        visitor_name)
         << std::endl;

    // Define visitor accept
    os_cc << fmt::format(R"###(void {0}::accept({2}& visitor) const {{
    visitor.visit_{1}(*this);
}})###",
                         subclass_name, to_lower(subclass_name), visitor_name)
          << std::endl
          << std::endl;

    // Define subclass fields
    for (const auto &[field_type, field_name] : fields) {
        os_h << "    " << field_type << " " << field_name << ";" << std::endl;
    }
    os_h << "};" << std::endl << std::endl;
}

std::pair<SubclassName, std::vector<Field>>
parse_string(const std::string &subclass_str) {
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

std::vector<std::pair<SubclassName, std::vector<Field>>>
parse_subclasses(const std::vector<std::string> &subclass_strs) {
    std::vector<std::pair<SubclassName, std::vector<Field>>> subclasses;
    std::transform(subclass_strs.begin(), subclass_strs.end(),
                   std::back_inserter(subclasses), parse_string);
    return subclasses;
}

void define_visitor(std::ostream &os_h, std::ostream &os_cc,
                    const std::string &visitor_name,
                    const std::vector<SubclassName> &subclasses) {
    os_h << fmt::format("struct {} {{", visitor_name) << std::endl;
    for (const auto &subclass_name : subclasses) {
        os_h << fmt::format("    virtual void visit_{0}(const {1} &{0}) = 0;",
                            to_lower(subclass_name), subclass_name)
             << std::endl;
        ;
    }
    os_h << "};" << std::endl << std::endl;
}

void define_ast(fs::path output_dir, const std::string &base_name,
                const std::vector<std::string> &subclass_strs,
                const std::vector<std::string> &includes = {}) {
    auto subclasses = parse_subclasses(subclass_strs);
    output_dir = output_dir / "syntactics";
    std::cout << output_dir << std::endl;
    std::ofstream ofile_h(output_dir / (to_lower(base_name) + ".h"));
    std::ofstream ofile_cc(output_dir / (to_lower(base_name) + ".cc"));

    std::string visitor_name = fmt::format("{}Visitor", base_name);

    // Headers of .h
    ofile_h << "#pragma once" << std::endl;
    for (const auto &include : includes) {
        ofile_h << fmt::format("#include \"src/{}\"", include) << std::endl;
    }
    ofile_h << "#include <memory>" << std::endl << std::endl;

    // Headers of .cc
    ofile_cc << fmt::format("#include \"src/syntactics/{}.h\"",
                            to_lower(base_name))
             << std::endl
             << std::endl;

    // Declare base class
    ofile_h << fmt::format("struct {};", base_name) << std::endl << std::endl;

    std::vector<SubclassName> subclass_names;
    std::transform(subclasses.begin(), subclasses.end(),
                   std::back_inserter(subclass_names),
                   [](const auto &x) { return x.first; });

    // Declare subclasses
    for (const auto &subclass_name : subclass_names) {
        ofile_h << fmt::format("struct {};", subclass_name) << std::endl;
    }
    ofile_h << std::endl;

    define_visitor(ofile_h, ofile_cc, visitor_name, subclass_names);

    define_base_class(ofile_h, base_name, visitor_name);

    for (const auto &[subclass_name, fields] : subclasses) {
        define_subclass(ofile_h, ofile_cc, base_name, visitor_name,
                        subclass_name, fields);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: define_ast [output dir]" << std::endl;
        return 1;
    }
    std::string output_dir = argv[1];
    define_ast(output_dir, "Expr",
               {
                   "Binary : Expr left, Token op, Expr right",
                   "Grouping : Expr expression",
                   "Literal  : Token value",
                   "Unary    : Token op, Expr right",

               },
               {"syntactics/token.h"});

    define_ast(output_dir, "Stmt",
               {
                   "Expression : std::unique_ptr<Expr> expression",
                   "Print : std::unique_ptr<Expr> expression",
               },
               {"syntactics/expr.h"});
}