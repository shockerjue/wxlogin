cc_binary (
    name = "main",
    srcs = ["main.cc"],
    deps = [
        "//md5:md5",
        "//json:json",
        "//util:util",
        "//uuid:uuid",
        "//cache:cache",
        "//model:model",
        "//hiredis:hiredis",
        "//protocol:protolib",
    ],
    linkstatic = 0,
)


cc_binary (
    name = "server",
    srcs = ["wxlogin_server.cc"],
    deps = [
        "//md5:md5",
        "//util:util",
        "//json:json",
        "//uuid:uuid",
        "//cache:cache",
        "//model:model",
        "//hiredis:hiredis",
        "//protocol:protolib",
    ],
    linkstatic = 0,
)

cc_binary (
    name = "client",
    srcs = ["wxlogin_client.cc"],
    deps = [
        "//md5:md5",
        "//util:util",
        "//json:json",
        "//model:model",
        "//hiredis:hiredis",
        "//protocol:protolib",
    ],
    linkstatic = 0,
)