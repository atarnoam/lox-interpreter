#pragma once

#include "src/semantics/lox_object.h"

struct Return {
    Return(LoxObject return_value);

    LoxObject return_value;
};