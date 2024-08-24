#include "src/semantics/natives.h"

#include "natives.h"
#include "src/semantics/object/lox_object.h"
#include "src/semantics/object/print_lox_object.h"
#include <chrono>
#include <sstream>

const std::vector<std::pair<std::string, LoxObject>> natives{
    {"clock", LoxObject(std::make_shared<ClockFun>())},
    {"string", LoxObject(std::make_shared<ToStringFun>())},
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

std::string ToStringFun::to_string() const { return "<native fn>"; }

LoxObject ToStringFun::call(AbstractInterpreter &interpreter,
                            const std::vector<LoxObject> &arguments) {
    std::stringstream ss;
    ss << arguments[0];
    return ss.str();
}
int ToStringFun::arity() const { return 1; }
