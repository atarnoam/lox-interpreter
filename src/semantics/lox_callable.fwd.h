#pragma once

#include <iostream>

struct LoxCallable;

std::ostream &operator<<(std::ostream &os, const LoxCallable &callable);
