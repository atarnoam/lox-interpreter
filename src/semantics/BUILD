package(default_visibility = ["//visibility:public"])

cc_library(
    name = "runtime_error",
    srcs = ["runtime_error.cc"],
    hdrs = ["runtime_error.h"],
    deps = [
        ":lox_object",
    ],
)

cc_library(
    name = "lox_object",
    srcs = ["lox_object.cc"],
    hdrs = ["lox_object.h"],
    deps = [
        ":lox_callable_fwd",
        "//src:tp_utils",
        "//src/syntactics:expr",
        "//src/syntactics:token",
    ],
)

cc_library(
    name = "environment",
    srcs = ["environment.cc"],
    hdrs = ["environment.h"],
    deps = [
        ":lox_object",
        ":runtime_error",
    ],
)

cc_library(
    name = "interpreter_function",
    srcs = ["interpreter_function.cc"],
    hdrs = ["interpreter_function.h"],
    deps = [
        ":interpreter",
        ":lox_function",
    ],
)

cc_library(
    name = "return",
    srcs = ["return.cc"],
    hdrs = ["return.h"],
    deps = [
        ":lox_object",
    ],
)

cc_library(
    name = "abstract_interpreter",
    srcs = ["abstract_interpreter.cc"],
    hdrs = ["abstract_interpreter.h"],
    deps = [
        ":environment",
        "//src/syntactics:stmt",
    ],
)

cc_library(
    name = "interpreter",
    srcs = ["interpreter.cc"],
    hdrs = ["interpreter.h"],
    deps = [
        ":environment",
        ":lox_callable",
        ":lox_function",
        ":lox_object",
        ":natives",
        ":return",
        ":runtime_error",
        "//src:logging",
        "//src:tp_utils",
        "//src/syntactics:expr",
        "//src/syntactics:stmt",
    ],
)

cc_library(
    name = "lox_callable_fwd",
    hdrs = ["lox_callable.fwd.h"],
)

cc_library(
    name = "lox_callable",
    srcs = ["lox_callable.cc"],
    hdrs = ["lox_callable.h"],
    deps = [
        ":abstract_interpreter",
        ":lox_callable_fwd",
    ],
)

cc_library(
    name = "print_lox_object",
    srcs = ["print_lox_object.cc"],
    hdrs = ["print_lox_object.h"],
    deps = [
        ":lox_callable",
        ":lox_object",
    ],
)

cc_library(
    name = "natives",
    srcs = ["natives.cc"],
    hdrs = ["natives.h"],
    deps = [
        ":lox_callable",
        ":lox_object",
    ],
)

cc_library(
    name = "lox_function",
    srcs = ["lox_function.cc"],
    hdrs = ["lox_function.h"],
    deps = [
        ":abstract_interpreter",
        ":environment",
        ":lox_callable",
        ":lox_object",
        ":print_lox_object",
        ":return",
    ],
)
