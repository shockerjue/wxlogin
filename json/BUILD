cc_library(
    name = "json",
    srcs = [
        "json.hpp",
        ],
    hdrs = [
        "json.hpp"
        ],
    visibility = ["//visibility:public"],
)

cc_binary (
    name = "json_test",
    srcs = ["main.cpp"],
    deps = [
        ":json",
    ],
    linkstatic = 0,
)