#include "src/semantics/return.h"

Return::Return(LoxObject obj) : return_value(std::move(obj)) {}