#include "serialize.h"
#include <array>
#include <gtest/gtest.h>

using u32 = uint32_t;
using u8 = uint8_t;

TEST(serialize_integer, u32) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u32> data{};
  std::get<0>(data) = 0x01020304;

  auto length = embedded_serialization::serialization(data, data_span);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0] = 0x04;
  expect_data_area[1] = 0x03;
  expect_data_area[2] = 0x02;
  expect_data_area[3] = 0x01;

  EXPECT_EQ(length, 4);
  EXPECT_EQ(data_area, expect_data_area);
}

TEST(serialize_integer, u32_u32) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u32, u32> data{};
  std::get<0>(data) = 0x01020304;
  std::get<1>(data) = 0x05060708;

  auto length = embedded_serialization::serialization(data, data_span);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0] = 0x04;
  expect_data_area[1] = 0x03;
  expect_data_area[2] = 0x02;
  expect_data_area[3] = 0x01;

  expect_data_area[4] = 0x08;
  expect_data_area[5] = 0x07;
  expect_data_area[6] = 0x06;
  expect_data_area[7] = 0x05;

  EXPECT_EQ(length, 8);
  EXPECT_EQ(data_area, expect_data_area);
}