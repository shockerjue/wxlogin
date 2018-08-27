# Convention:
# A cc_proto_library that wraps a proto_library named foo_proto
# should be called foo_cc_proto.

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cc_proto_library")

cc_proto_library(
  name = "protolib",
  protos = [
    "wxloginpro.proto"
  ],
  
  with_grpc = True,
  verbose = 1, # 0=no output, 1=show protoc command, 2+ more...
    visibility = ["//:__pkg__"],
)