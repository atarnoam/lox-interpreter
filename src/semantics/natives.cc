#include "src/semantics/natives.h"

#include "src/semantics/object/lox_object.h"
#include <chrono>

const std::vector<std::pair<std::string, LoxObject>> natives{
    {"clock", LoxObject(std::make_shared<ClockFun>())},
};

std::string ClockFun::to_string() const { return "<native fn>"; }

LoxObject ClockFun::call(AbstractInterpreter &interpreter,
                         const std::vector<LoxObject> &arguments) {
    auto now = std::chrono::system_clock::now();
    double seconds_since_epoch =
        std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch())
            .count();
    return seconds_since_epoch;
}

int ClockFun::arity() const { return 0; }