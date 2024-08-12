#include "src/ast_printer.h"
#include "src/expr.h"
#include "src/parser.h"
#include "src/scanner.h"
#include "src/token.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

int interpret_stream(std::istream &&is) {
    Scanner scanner(std::move(is));
    if (scanner.had_error) {
        std::cerr << "Scanner had error" << std::endl;
        return 65;
    }
    auto tokens = scanner.scan_tokens();

    for (const Token &t : tokens) {
        std::cout << t << std::endl;
    }
    Parser parser(std::move(tokens));

    std::unique_ptr<Expr> expr = parser.expression();
    if (parser.had_error) {
        std::cerr << "Parser had error" << std::endl;
        return 64;
    }
    AstPrinter printer{};
    expr->accept(printer);
    std::cout << printer.get_string() << std::endl;

    return 0;
}

int run_file(char *filename) {
    std::ifstream ifs(filename);
    return interpret_stream(std::move(ifs));
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: jlox [script]" << std::endl;
        return 1;
    }
    return run_file(argv[1]);
}