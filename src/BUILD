cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [
        ":ast_printer",
        ":expr",
        ":parser",
        ":scanner",
        ":token",
    ],
)

cc_library(
    name = "scanner",
    srcs = ["scanner.cc"],
    hdrs = ["scanner.h"],
    deps = [":token"],
)

cc_library(
    name = "token",
    srcs = ["token.cc"],
    hdrs = ["token.h"],
)

cc_library(
    name = "expr",
    srcs = ["expr.cc"],
    hdrs = ["expr.h"],
    deps = [":token"],
)

cc_library(
    name = "ast_printer",
    srcs = ["ast_printer.cc"],
    hdrs = ["ast_printer.h"],
    deps = [
        ":expr",
        ":variadic",
    ],
)

cc_library(
    name = "variadic",
    hdrs = ["variadic.h"],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    deps = [
        ":expr",
        ":token",
        ":variadic",
    ],
)
