package(default_visibility = ["//visibility:public"])

cc_library(
    name = "runtime_error",
    srcs = ["runtime_error.cc"],
    hdrs = ["runtime_error.h"],
    deps = [
        "//src/semantics/object:lox_object",
    ],
)

cc_library(
    name = "environment",
    srcs = ["environment.cc"],
    hdrs = ["environment.h"],
    deps = [
        ":runtime_error",
        "//src/semantics/object:lox_object",
    ],
)

cc_test(
    name = "environment_test",
    srcs = ["environment_test.cc"],
    deps = [
        ":environment",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "return",
    srcs = ["return.cc"],
    hdrs = ["return.h"],
    deps = [
        "//src/semantics/object:lox_object",
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
        ":natives",
        ":return",
        ":runtime_error",
        "//src:logging",
        "//src:tp_utils",
        "//src/semantics/object:lox_callable",
        "//src/semantics/object:lox_function",
        "//src/semantics/object:lox_object",
        "//src/semantics/object:print_lox_object",
        "//src/syntactics:expr",
        "//src/syntactics:stmt",
    ],
)

cc_library(
    name = "natives",
    srcs = ["natives.cc"],
    hdrs = ["natives.h"],
    deps = [
        "//src/semantics/object:lox_callable",
        "//src/semantics/object:lox_object",
    ],
)

cc_library(
    name = "resolver",
    srcs = ["resolver.cc"],
    hdrs = ["resolver.h"],
    deps = [
        "//src:logging",
        "//src/semantics:abstract_interpreter",
        "//src/syntactics:expr",
        "//src/syntactics:stmt",
    ],
)
