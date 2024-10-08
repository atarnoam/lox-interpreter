#include "src/ast_printer.h"
#include "src/semantics/interpreter.h"
#include "src/semantics/object/lox_callable.h"
#include "src/semantics/object/lox_function.h"
#include "src/semantics/resolver.h"
#include "src/syntactics/expr.h"
#include "src/syntactics/parser.h"
#include "src/syntactics/scanner.h"
#include "src/syntactics/token.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>

std::optional<std::vector<std::shared_ptr<Stmt>>>
parse_stream(std::istream &&is) {
    Scanner scanner(std::move(is));

    auto tokens = scanner.scan_tokens();
    if (scanner.had_error) {
        return std::nullopt;
    }

    Parser parser(std::move(tokens));

    auto stmts = parser.parse();
    if (parser.had_error()) {
        return std::nullopt;
    }

    return stmts;
}

int interpret_stream(Interpreter &interpreter, std::istream &&is,
                     InterpreterMode mode) {
    auto stmts_opt = parse_stream(std::move(is));
    if (!stmts_opt.has_value()) {
        return 65;
    }

    auto stmts = stmts_opt.value();
    Resolver resolver{interpreter};
    resolver.resolve(stmts);

    if (resolver.had_error()) {
        return 66;
    }

    interpreter.interpret(stmts, mode);
    if (interpreter.had_runtime_error()) {
        return 70;
    }

    return 0;
}

int run_interpreter() {
    Interpreter interpreter{};

    std::string s;
    std::cout << "> ";
    while (std::getline(std::cin, s)) {
        interpret_stream(interpreter, std::stringstream(s),
                         InterpreterMode::INTERACTIVE);
        std::cout << "> ";
        interpreter.reset_runtime_error();
    }
    std::cout << std::endl;
    return 0;
}

int run_file(char *filename) {
    Interpreter interpreter;
    std::ifstream ifs(filename);
    return interpret_stream(interpreter, std::move(ifs), InterpreterMode::FILE);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        return run_interpreter();
    } else if (argc == 2) {
        return run_file(argv[1]);
    } else {
        std::cout << "Usage: cpplox [script]" << std::endl;
        return 1;
    }
}