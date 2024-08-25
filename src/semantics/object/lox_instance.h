#pragma once

#include "src/semantics/object/lox_instance.fwd.h"

#include "src/semantics/object/lox_class.h"
#include "src/semantics/object/lox_object.h"
#include "src/syntactics/token.h"

#include <string>
#include <unordered_map>

struct LoxInstance {
    LoxInstance(const LoxClass *lclass);

    std::string to_string() const;

    LoxObject get(Token name, const std::shared_ptr<LoxInstance> &this_bind,
                  EnvironmentTree &envs) const;
    void set(Token name, const LoxObject &value);

  private:
    const LoxClass *lclass;
    std::unordered_map<std::string, LoxObject> fields;
};

std::ostream &operator<<(std::ostream &os, const LoxInstance &callable);
