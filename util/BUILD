cc_library(
    name = "util",
    srcs = [
        "datetime.cc",
        "dbmodule.cc",
        "log.cc",
        "redisutil.cc",
        "value.cc",
        "imodule.cc",
        "configmodule.cc",
        "mysqlutil.cc"
        ],
    hdrs = [
        "datetime.h",
        "dbmodule.h",
        "log.h",
        "redisutil.h",
        "value.h",
        "imodule.h",
        "configmodule.h",
        "mysqlutil.h",
        "validate.h",
        "const.h"
        ],
    deps = ["//hiredis:hiredis"],
    linkopts = ["-lmysqlclient"],
    visibility = ["//visibility:public"],
)