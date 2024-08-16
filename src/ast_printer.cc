#include "src/ast_printer.h"

AstPrinter::AstPrinter() {}

void AstPrinter::visit_binary(const Binary &binary) {
    parenthesize(binary.op.lexeme, binary.left.get(), binary.right.get());
}

void AstPrinter::visit_grouping(const Grouping &grouping) {
    parenthesize("group", grouping.expression.get());
}

void AstPrinter::visit_literal(const Literal &literal) {
    result << literal.value.literal_to_string();
}

void AstPrinter::visit_unary(const Unary &unary) {
    parenthesize(unary.op.lexeme, unary.right.get());
}

std::string AstPrinter::get_string() const { return result.str(); }
