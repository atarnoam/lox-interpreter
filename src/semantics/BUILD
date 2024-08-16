package(default_visibility = ["//visibility:public"])

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
    name = "interpreter",
    srcs = ["interpreter.cc"],
    hdrs = ["interpreter.h"],
    deps = [
        ":lox_object",
        "//src:logging",
        "//src:tp_utils",
        "//src/syntactics:expr",
    ],
)
