#include "endian.h"
#include "serialize.h"
#include "span.h"
#include <array>
#include <gtest/gtest.h>
#include <tuple>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

TEST(serialize_static_array, u32) {
  std::array<u8, 32> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::array<u32, 4> raw_data{1, 2, 3, 4};
  auto data = std::make_tuple(embedded_serialization::SerializedSpan<u32, 4U, 4U>{raw_data.data(), raw_data.size()});

  auto serialize_length = embedded_serialization::serialization(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 32> expect_data_area{};
  u32 offset = 0U;
  // element 1
  expect_data_area[offset++] = 0x01;
  expect_data_area[offset++] = 0x00;
  expect_data_area[offset++] = 0x00;
  expect_data_area[offset++] = 0x00;
  // element 2
  expect_data_area[offset++] = 0x02;
  expect_data_area[offset++] = 0x00;
  expect_data_area[offset++] = 0x00;
  expect_data_area[offset++] = 0x00;
  // element 3
  expect_data_area[offset++] = 0x03;
  expect_data_area[offset++] = 0x00;
  expect_data_area[offset++] = 0x00;
  expect_data_area[offset++] = 0x00;
  // element 4
  expect_data_area[offset++] = 0x04;
  expect_data_area[offset++] = 0x00;
  expect_data_area[offset++] = 0x00;
  expect_data_area[offset++] = 0x00;

  EXPECT_EQ(serialize_length, offset);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  embedded_serialization::DeserializedType<decltype(data)>::type out = {};
  auto deserialize_length = embedded_serialization::deserialization<decltype(data)>(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_static_array, u8) {
  std::array<u8, 32> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::array<u8, 4> raw_data{1, 2, 3, 4};
  auto data = std::make_tuple(embedded_serialization::SerializedSpan<u8, 4U, 4U>{raw_data.data(), raw_data.size()});

  auto serialize_length = embedded_serialization::serialization(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 32> expect_data_area{};
  u32 offset = 0U;
  // element 1
  expect_data_area[offset++] = 0x01;
  // element 2
  expect_data_area[offset++] = 0x02;
  // element 3
  expect_data_area[offset++] = 0x03;
  // element 4
  expect_data_area[offset++] = 0x04;

  EXPECT_EQ(serialize_length, offset);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  embedded_serialization::DeserializedType<decltype(data)>::type out = {};
  auto deserialize_length = embedded_serialization::deserialization<decltype(data)>(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}
