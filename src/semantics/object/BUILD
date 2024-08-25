package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lox_object",
    srcs = ["lox_object.cc"],
    hdrs = ["lox_object.h"],
    deps = [
        ":lox_callable_fwd",
        ":lox_instance_fwd",
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
        ":lox_instance",
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

cc_library(
    name = "lox_class_fwd",
    hdrs = ["lox_class.fwd.h"],
    deps = [
    ],
)

cc_library(
    name = "lox_class_h",
    hdrs = ["lox_class.h"],
    deps = [
        ":lox_callable",
        ":lox_class_fwd",
        ":lox_function",
        ":lox_object",
        "//src/semantics:abstract_interpreter",
        "//src/semantics:environment",
    ],
)

cc_library(
    name = "lox_class",
    srcs = ["lox_class.cc"],
    deps = [
        ":lox_class_h",
        ":lox_instance_h",
    ],
)

cc_library(
    name = "lox_instance_fwd",
    hdrs = ["lox_instance.fwd.h"],
    deps = [],
)

cc_library(
    name = "lox_instance_h",
    hdrs = ["lox_instance.h"],
    deps = [
        ":lox_callable",
        ":lox_class_h",
        ":lox_function",
        ":lox_instance_fwd",
        ":lox_object",
    ],
)

cc_library(
    name = "lox_instance",
    srcs = ["lox_instance.cc"],
    deps = [
        ":lox_instance_h",
        "//src/semantics:runtime_error",
    ],
)
