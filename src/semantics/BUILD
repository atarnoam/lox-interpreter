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
    name = "interpreter",
    srcs = ["interpreter.cc"],
    hdrs = ["interpreter.h"],
    deps = [
        ":environment",
        ":lox_object",
        ":runtime_error",
        "//src:logging",
        "//src:tp_utils",
        "//src/syntactics:expr",
        "//src/syntactics:stmt",
    ],
)
