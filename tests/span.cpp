#include "span.h"
#include <array>
#include <gtest/gtest.h>

// NOLINTNEXTLINE(cert-err58-cpp, cppcoreguidelines-special-member-functions)
TEST(span, eq) {
  std::array<int, 8U> data{1, 2, 3, 4, 1, 2, 3, 4};
  embedded_serialization::SerializedSpan<int, 4U, 4U> span1{&data[0U], 4U};
  embedded_serialization::SerializedSpan<int, 4U, 4U> span2{&data[4U], 4U};
  EXPECT_EQ(span1, span2);
}

// NOLINTNEXTLINE(cert-err58-cpp, cppcoreguidelines-special-member-functions)
TEST(span, ne_data) {
  std::array<int, 8U> data{1, 2, 3, 4, 1, 3, 3, 4};
  embedded_serialization::SerializedSpan<int, 4U, 4U> span1{&data[0U], 4U};
  embedded_serialization::SerializedSpan<int, 4U, 4U> span2{&data[4U], 4U};
  EXPECT_NE(span1, span2);
}

// NOLINTNEXTLINE(cert-err58-cpp, cppcoreguidelines-special-member-functions)
TEST(span, ne_size) {
  std::array<int, 8U> data{1, 2, 3, 4, 1, 2, 3, 4};
  embedded_serialization::SerializedSpan<int, 2U, 4U> span1{&data[0U], 4U};
  embedded_serialization::SerializedSpan<int, 2U, 4U> span2{&data[4U], 3U};
  EXPECT_NE(span1, span2);
}

// NOLINTNEXTLINE(cert-err58-cpp, cppcoreguidelines-special-member-functions)
TEST(span, cast_from_non_const) {
  std::array<int, 8U> data{1, 2, 3, 4};
  embedded_serialization::SerializedSpan<int, 2U, 4U> span1{&data[0U], 4U};
  embedded_serialization::SerializedSpan<const int, 2U, 4U> span2{span1};
  span2 = span1;
}
