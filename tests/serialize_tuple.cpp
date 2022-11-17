#include "serialize.h"
#include <gtest/gtest.h>
#include <tuple>

using namespace embedded_serialization;

TEST(serialize_integer, tuple) {
  std::array<u8, 16> data_area{};
  Span<u8> data_span{data_area.data(), data_area.size()};
  auto data = std::make_tuple(static_cast<u8>(1), static_cast<u16>(1), static_cast<u32>(1));

  auto serialize_length = serialization(data, data_span);
  EXPECT_EQ(get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x01;

  expect_data_area[1U] = 0x01;
  expect_data_area[2U] = 0x00;

  expect_data_area[3U] = 0x01;
  expect_data_area[4U] = 0x00;
  expect_data_area[5U] = 0x00;
  expect_data_area[6U] = 0x00;

  EXPECT_EQ(serialize_length, 7U);
  EXPECT_EQ(data_area, expect_data_area);

  Span<u8> const data_span_const{data_area.data(), data_area.size()};
  DeserializedType<decltype(data)>::type out = {};
  auto deserialize_length = deserialization<decltype(data)>(data_span_const, out);
  EXPECT_EQ(get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}
