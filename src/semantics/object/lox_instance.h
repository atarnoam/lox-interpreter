#pragma once

#include "src/semantics/object/lox_instance.fwd.h"

#include "src/semantics/object/lox_class.h"

#include <string>

struct LoxInstance {
    LoxInstance(const LoxClass *lclass);

    std::string to_string() const;

  private:
    const LoxClass *lclass;
};

std::ostream &operator<<(std::ostream &os, const LoxInstance &callable);
