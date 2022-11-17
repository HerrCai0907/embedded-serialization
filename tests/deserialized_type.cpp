#include "serialize.h"
#include "span.h"
#include <gtest/gtest.h>

using namespace embedded_serialization;

TEST(deserialized_type, integer) {
  using Input = std::tuple<u8>;
  using Expect = std::tuple<u8>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}

TEST(deserialized_type, static_array) {
  using Input = std::tuple<SerializedSpan<u8, 4U, 4U>>;
  using Expect = std::tuple<SerializedSpan<u8, 4U, 4U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, static_array_static_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<u8, 1U, 1U>, 2U, 2U>>;
  using Expect = std::tuple<std::array<SerializedSpan<u8, 1U, 1U>, 2U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, dynamic_array) {
  using Input = std::tuple<SerializedSpan<u8, 4U, 5U>>;
  using Expect = std::tuple<SerializedSpan<u8, 4U, 5U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, dynamic_array_dynamic_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<u8, 1U, 2U>, 3U, 4U>>;
  using Expect = std::tuple<std::array<SerializedSpan<u8, 1U, 2U>, 4U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, dynamic_array_dynamic_array_dynamic_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<SerializedSpan<u8, 1U, 2U>, 3U, 4U>, 5U, 6U>>;
  using Expect = std::tuple<std::array<std::array<SerializedSpan<u8, 1U, 2U>, 4U>, 6U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, static_array_dynamic_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<u8, 0U, 16U>, 4U, 4U>>;
  using Expect = std::tuple<std::array<SerializedSpan<u8, 0U, 16U>, 4U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, dynamic_array_static_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<u8, 1U, 1U>, 2U, 3U>>;
  using Expect = std::tuple<std::array<SerializedSpan<u8, 1U, 1U>, 3U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}

TEST(deserialized_type, tuple) {
  using Input = std::tuple<u8, uint16_t, uint32_t, SerializedSpan<u8, 1U, 1U>>;
  using Expect = std::tuple<u8, uint16_t, uint32_t, SerializedSpan<u8, 1U, 1U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}

TEST(deserialized_type, dynamic_array_tuple) {
  using Input = std::tuple<SerializedSpan<std::tuple<u8, uint16_t, uint32_t, SerializedSpan<u8, 1U, 1U>>, 1U, 2U>>;
  using Expect = std::tuple<std::array<std::tuple<u8, uint16_t, uint32_t, SerializedSpan<u8, 1U, 1U>>, 2U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}