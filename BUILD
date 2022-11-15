cc_library(
    name = "embedded_serialization_inner",
    hdrs = glob(["src/*.h"]),
    strip_include_prefix = "src",
    visibility = ["//:__subpackages__"],
)

cc_library(
    name = "embedded_serialization",
    hdrs = glob(["src/*.h"]),
    include_prefix = "embedded_serialization",
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
)
