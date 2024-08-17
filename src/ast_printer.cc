#include "src/ast_printer.h"

AstPrinter::AstPrinter() {}

void AstPrinter::visit_binary_expr(const Expr::Binary &binary) {
    parenthesize(binary.op.lexeme, binary.left.get(), binary.right.get());
}

void AstPrinter::visit_grouping_expr(const Expr::Grouping &grouping) {
    parenthesize("group", grouping.expression.get());
}

void AstPrinter::visit_literal_expr(const Expr::Literal &literal) {
    result << literal.value.literal_to_string();
}

void AstPrinter::visit_unary_expr(const Expr::Unary &unary) {
    parenthesize(unary.op.lexeme, unary.right.get());
}

std::string AstPrinter::get_string() const { return result.str(); }
