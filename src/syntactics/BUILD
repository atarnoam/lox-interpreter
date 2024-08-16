package(default_visibility = ["//visibility:public"])

cc_library(
    name = "expr",
    srcs = ["expr.cc"],
    hdrs = ["expr.h"],
    deps = [":token"],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    deps = [
        ":token",
        "//src:logging",
        "//src:variadic",
        "//src/syntactics:expr",
    ],
)

cc_library(
    name = "scanner",
    srcs = ["scanner.cc"],
    hdrs = ["scanner.h"],
    deps = [
        ":token",
        "//src:logging",
    ],
)

cc_library(
    name = "token",
    srcs = ["token.cc"],
    hdrs = ["token.h"],
)
