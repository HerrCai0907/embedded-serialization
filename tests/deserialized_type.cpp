#include "serialize.h"
#include "span.h"
#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>
#include <gtest/gtest.h>

using namespace embedded_serialization;

TEST(deserialized_type, integer) {
  using Input = std::tuple<uint8_t>;
  using Expect = std::tuple<uint8_t>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}

TEST(deserialized_type, static_array) {
  using Input = std::tuple<SerializedSpan<uint8_t, 4U, 4U>>;
  using Expect = std::tuple<SerializedSpan<uint8_t, 4U, 4U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, static_array_static_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<uint8_t, 1U, 1U>, 2U, 2U>>;
  using Expect = std::tuple<std::array<SerializedSpan<uint8_t, 1U, 1U>, 2U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, dynamic_array) {
  using Input = std::tuple<SerializedSpan<uint8_t, 4U, 5U>>;
  using Expect = std::tuple<SerializedSpan<uint8_t, 4U, 5U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, dynamic_array_dynamic_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<uint8_t, 1U, 2U>, 3U, 4U>>;
  using Expect = std::tuple<std::array<SerializedSpan<uint8_t, 1U, 2U>, 4U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, dynamic_array_dynamic_array_dynamic_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<SerializedSpan<uint8_t, 1U, 2U>, 3U, 4U>, 5U, 6U>>;
  using Expect = std::tuple<std::array<std::array<SerializedSpan<uint8_t, 1U, 2U>, 4U>, 6U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, static_array_dynamic_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<uint8_t, 0U, 16U>, 4U, 4U>>;
  using Expect = std::tuple<std::array<SerializedSpan<uint8_t, 0U, 16U>, 4U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}
TEST(deserialized_type, dynamic_array_static_array) {
  using Input = std::tuple<SerializedSpan<SerializedSpan<uint8_t, 1U, 1U>, 2U, 3U>>;
  using Expect = std::tuple<std::array<SerializedSpan<uint8_t, 1U, 1U>, 3U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}

TEST(deserialized_type, tuple) {
  using Input = std::tuple<uint8_t, uint16_t, uint32_t, SerializedSpan<uint8_t, 1U, 1U>>;
  using Expect = std::tuple<uint8_t, uint16_t, uint32_t, SerializedSpan<uint8_t, 1U, 1U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}

TEST(deserialized_type, dynamic_array_tuple) {
  using Input =
      std::tuple<SerializedSpan<std::tuple<uint8_t, uint16_t, uint32_t, SerializedSpan<uint8_t, 1U, 1U>>, 1U, 2U>>;
  using Expect = std::tuple<std::array<std::tuple<uint8_t, uint16_t, uint32_t, SerializedSpan<uint8_t, 1U, 1U>>, 2U>>;
  EXPECT_EQ(typeid(DeserializedType<Input>::type), typeid(Expect));
}