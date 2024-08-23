package(default_visibility = ["//visibility:public"])

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
    name = "lox_callable_fwd",
    hdrs = ["lox_callable.fwd.h"],
)

cc_library(
    name = "lox_callable",
    srcs = ["lox_callable.cc"],
    hdrs = ["lox_callable.h"],
    deps = [
        ":lox_callable_fwd",
        "//src/semantics:abstract_interpreter",
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
    name = "lox_function",
    srcs = ["lox_function.cc"],
    hdrs = ["lox_function.h"],
    deps = [
        ":lox_callable",
        ":lox_object",
        "//src/semantics:abstract_interpreter",
        "//src/semantics:environment",
        "//src/semantics:return",
    ],
)
