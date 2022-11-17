#include "endian.h"
#include "serialize.h"
#include "span.h"
#include <array>
#include <gtest/gtest.h>
#include <tuple>

using namespace embedded_serialization;

TEST(serialize_dyn_array, u32) {
  std::array<u8, 32> data_area{};
  Span<u8> data_span{data_area.data(), data_area.size()};

  std::array<u32, 4> raw_data{1, 2, 3, 4};
  auto data = std::make_tuple(SerializedSpan<u32, 0U, 256U>{raw_data.data(), raw_data.size()});

  auto length = serialization(data, data_span);
  EXPECT_EQ(get_size(data), length);

  std::array<u8, 32> expect_data_area{};
  u32 offset = 0U;
  // size
  expect_data_area[offset++] = 0x04;
  expect_data_area[offset++] = 0x00;
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

  EXPECT_EQ(length, offset);
  EXPECT_EQ(data_area, expect_data_area);

  Span<u8> const data_span_const{data_area.data(), data_area.size()};
  DeserializedType<decltype(data)>::type out = {};
  auto deserialize_length = deserialization<decltype(data)>(data_span_const, out);
  EXPECT_EQ(get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_dyn_array, u8) {
  std::array<u8, 32> data_area{};
  Span<u8> data_span{data_area.data(), data_area.size()};

  std::array<u8, 3> raw_data{1, 2, 3};
  auto data = std::make_tuple(SerializedSpan<u8, 0U, 6U>{raw_data.data(), raw_data.size()});

  auto length = serialization(data, data_span);
  EXPECT_EQ(get_size(data), length);

  std::array<u8, 32> expect_data_area{};
  u32 offset = 0U;
  // size
  expect_data_area[offset++] = 0x03;
  // element 1
  expect_data_area[offset++] = 0x01;
  // element 2
  expect_data_area[offset++] = 0x02;
  // element 3
  expect_data_area[offset++] = 0x03;

  EXPECT_EQ(length, offset);
  EXPECT_EQ(data_area, expect_data_area);

  Span<u8> const data_span_const{data_area.data(), data_area.size()};
  DeserializedType<decltype(data)>::type out = {};
  auto deserialize_length = deserialization<decltype(data)>(data_span_const, out);
  EXPECT_EQ(get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_dyn_array, static_array) {
  std::array<u8, 32> data_area{};
  Span<u8> data_span{data_area.data(), data_area.size()};

  std::array<u8, 3> raw_data{1, 2, 3};
  std::array<SerializedSpan<u8, 3U, 3U>, 2U> span_arr{};
  span_arr[0] = SerializedSpan<u8, 3U, 3U>{raw_data.data(), raw_data.size()};
  span_arr[1] = SerializedSpan<u8, 3U, 3U>{raw_data.data(), raw_data.size()};

  auto data = std::make_tuple(SerializedSpan<SerializedSpan<u8, 3U, 3U>, 0U, 6U>{span_arr.data(), span_arr.size()});

  auto length = serialization(data, data_span);
  EXPECT_EQ(get_size(data), length);

  std::array<u8, 32> expect_data_area{};
  u32 offset = 0U;
  // size
  expect_data_area[offset++] = 0x02;
  // 1
  expect_data_area[offset++] = 0x01;
  expect_data_area[offset++] = 0x02;
  expect_data_area[offset++] = 0x03;
  // 2
  expect_data_area[offset++] = 0x01;
  expect_data_area[offset++] = 0x02;
  expect_data_area[offset++] = 0x03;

  EXPECT_EQ(length, offset);
  EXPECT_EQ(data_area, expect_data_area);

  Span<u8> const data_span_const{data_area.data(), data_area.size()};
  DeserializedType<decltype(data)>::type out = {};
  auto deserialize_length = deserialization<decltype(data)>(data_span_const, out);
  EXPECT_EQ(get_size(data), deserialize_length);
  EXPECT_EQ(std::get<0>(out)[0][0], 1U);
  EXPECT_EQ(std::get<0>(out)[0][1], 2U);
  EXPECT_EQ(std::get<0>(out)[0][2], 3U);
  EXPECT_EQ(std::get<0>(out)[1][0], 1U);
  EXPECT_EQ(std::get<0>(out)[1][1], 2U);
  EXPECT_EQ(std::get<0>(out)[1][2], 3U);
  SerializedSpan<u8, 3U, 3U> nullspan{};
  EXPECT_EQ(std::get<0>(out)[3], nullspan);
  EXPECT_EQ(std::get<0>(out)[4], nullspan);
  EXPECT_EQ(std::get<0>(out)[5], nullspan);
}
