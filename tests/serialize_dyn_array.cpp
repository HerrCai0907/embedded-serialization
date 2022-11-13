#include "endian.h"
#include "serialize.h"
#include "span.h"
#include <array>
#include <gtest/gtest.h>
#include <tuple>

using u32 = uint32_t;
using u8 = uint8_t;

TEST(serialize_dyn_array, u32) {
  std::array<u8, 32> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::array<u32, 4> rawdata{1, 2, 3, 4};
  auto data = std::make_tuple(embedded_serialization::SerializedSpan<u32, 0U, 256U>{rawdata.data(), rawdata.size()});

  auto length = embedded_serialization::serialization(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), length);

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

  EXPECT_EQ(length, 2U + 4U * 4U);
  EXPECT_EQ(data_area, expect_data_area);
}
