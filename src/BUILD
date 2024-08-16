package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        ":ast_printer",
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
        ":variadic",
        "//src/syntactics:expr",
    ],
)

cc_library(
    name = "variadic",
    hdrs = ["variadic.h"],
)

cc_library(
    name = "logging",
    srcs = ["logging.cc"],
    hdrs = ["logging.h"],
    deps = [
        ":variadic",
        "//src/syntactics:expr",
        "//src/syntactics:token",
        "@fmt",
    ],
)
