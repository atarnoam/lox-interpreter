#include "src/ast_printer.h"
#include "src/semantics/interpreter.h"
#include "src/syntactics/expr.h"
#include "src/syntactics/parser.h"
#include "src/syntactics/scanner.h"
#include "src/syntactics/token.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

int interpret_stream(Interpreter &interpreter, std::istream &&is) {
    Scanner scanner(std::move(is));

    auto tokens = scanner.scan_tokens();
    if (scanner.had_error) {
        return 64;
    }

    Parser parser(std::move(tokens));

    auto stmts = parser.parse();
    if (parser.had_error()) {
        return 65;
    }

    interpreter.interpret(stmts);
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
        interpret_stream(interpreter, std::stringstream(s));
        std::cout << "> ";
        interpreter.reset_runtime_error();
    }
    std::cout << std::endl;
    return 0;
}

int run_file(char *filename) {
    Interpreter interpreter;
    std::ifstream ifs(filename);
    return interpret_stream(interpreter, std::move(ifs));
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