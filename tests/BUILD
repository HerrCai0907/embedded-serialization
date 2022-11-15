test_suite(
    name = "tests",
    tags = ["small"],
)

cc_test(
    name = "serialize_dyn_array",
    size = "small",
    srcs = ["serialize_dyn_array.cpp"],
    deps = [
        "//:embedded_serialization_inner",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "serialize_integer",
    size = "small",
    srcs = ["serialize_integer.cpp"],
    deps = [
        "//:embedded_serialization_inner",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "serialize_static_array",
    size = "small",
    srcs = ["serialize_static_array.cpp"],
    deps = [
        "//:embedded_serialization_inner",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "size_traits",
    size = "small",
    srcs = ["size_traits.cpp"],
    deps = [
        "//:embedded_serialization_inner",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "span",
    size = "small",
    srcs = ["span.cpp"],
    deps = [
        "//:embedded_serialization_inner",
        "@com_google_googletest//:gtest_main",
    ],
)
