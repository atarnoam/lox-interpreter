package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        ":ast_printer",
        "//src/semantics:interpreter",
        "//src/semantics/object:lox_callable",
        "//src/semantics/object:lox_function",
        "//src/syntactics:expr",
        "//src/syntactics:parser",
        "//src/syntactics:scanner",
        "//src/syntactics:token",
    ],
)

cc_library(
    name = "ast_printer",
    srcs = ["ast_printer.cc"],
    hdrs = ["ast_printer.h"],
    deps = [
        ":tp_utils",
        "//src/syntactics:expr",
    ],
)

cc_library(
    name = "tp_utils",
    hdrs = ["tp_utils.h"],
)

cc_library(
    name = "logging",
    srcs = ["logging.cc"],
    hdrs = ["logging.h"],
    deps = [
        ":tp_utils",
        "//src/syntactics:expr",
        "//src/syntactics:token",
        "@fmt",
    ],
)
