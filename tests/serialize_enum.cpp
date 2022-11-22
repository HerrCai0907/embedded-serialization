#include "serialize.h"
#include <cstdint>
#include <gtest/gtest.h>

using namespace embedded_serialization;

enum EA { E1 = 2, E2 };
enum class EB { E1 = 4, E2 };
enum class EC : u16 { E1 = 6, E2 };

// NOLINTNEXTLINE(cert-err58-cpp, cppcoreguidelines-special-member-functions)
TEST(serialize_integer, enum) {
  std::array<u8, 16> data_area{};
  Span<u8> data_span{data_area.data(), data_area.size()};

  EA data{EA::E1};

  auto serialize_length = serialization(data, data_span);
  EXPECT_EQ(get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = static_cast<u8>(data);

  EXPECT_EQ(serialize_length, sizeof(EA));
  EXPECT_EQ(data_area, expect_data_area);

  decltype(data) out;
  auto deserialize_length = deserialization<decltype(data)>(data_span, out);
  EXPECT_EQ(serialize_length, deserialize_length);
  EXPECT_EQ(data, out);
}

// NOLINTNEXTLINE(cert-err58-cpp, cppcoreguidelines-special-member-functions)
TEST(serialize_integer, class_enum) {
  std::array<u8, 16> data_area{};
  Span<u8> data_span{data_area.data(), data_area.size()};

  EB data{EB::E1};

  auto serialize_length = serialization(data, data_span);
  EXPECT_EQ(get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = static_cast<u8>(data);

  EXPECT_EQ(serialize_length, sizeof(EB));
  EXPECT_EQ(data_area, expect_data_area);
  decltype(data) out;
  auto deserialize_length = deserialization<decltype(data)>(data_span, out);
  EXPECT_EQ(serialize_length, deserialize_length);
  EXPECT_EQ(data, out);
}

// NOLINTNEXTLINE(cert-err58-cpp, cppcoreguidelines-special-member-functions)
TEST(serialize_integer, u16_enum) {
  std::array<u8, 16> data_area{};
  Span<u8> data_span{data_area.data(), data_area.size()};

  EC data{EC::E1};

  auto serialize_length = serialization(data, data_span);
  EXPECT_EQ(get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = static_cast<u8>(data);

  EXPECT_EQ(serialize_length, sizeof(EC));
  EXPECT_EQ(data_area, expect_data_area);

  decltype(data) out;
  auto deserialize_length = deserialization<decltype(data)>(data_span, out);
  EXPECT_EQ(serialize_length, deserialize_length);
  EXPECT_EQ(data, out);
}
