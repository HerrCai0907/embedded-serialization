#include "endian.h"
#include "serialize.h"
#include <array>
#include <gtest/gtest.h>

using u64 = uint64_t;
using u32 = uint32_t;
using u16 = uint16_t;
using u8 = uint8_t;

TEST(serialize_integer, const_type) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<const u8> data{0x0f};

  auto serialize_length = embedded_serialization::serialization(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x0f;

  EXPECT_EQ(serialize_length, 1U);
  EXPECT_EQ(data_area, expect_data_area);
}

// ██    ██  ██████  ██   ██
// ██    ██ ██       ██   ██
// ██    ██ ███████  ███████
// ██    ██ ██    ██      ██
//  ██████   ██████       ██

TEST(serialize_integer, u64) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u64> data{};
  std::get<0>(data) = 0x0102030405060708;

  auto serialize_length = embedded_serialization::serialization(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x08;
  expect_data_area[1U] = 0x07;
  expect_data_area[2U] = 0x06;
  expect_data_area[3U] = 0x05;
  expect_data_area[4U] = 0x04;
  expect_data_area[5U] = 0x03;
  expect_data_area[6U] = 0x02;
  expect_data_area[7U] = 0x01;

  EXPECT_EQ(serialize_length, 8U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length = embedded_serialization::deserialization(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_integer, big_endian_u64) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u64> data{};
  std::get<0>(data) = 0x0102030405060708;

  auto serialize_length = embedded_serialization::serialization<embedded_serialization::BigEndian>(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x08;
  expect_data_area[1U] = 0x07;
  expect_data_area[2U] = 0x06;
  expect_data_area[3U] = 0x05;
  expect_data_area[4U] = 0x04;
  expect_data_area[5U] = 0x03;
  expect_data_area[6U] = 0x02;
  expect_data_area[7U] = 0x01;

  EXPECT_EQ(serialize_length, 8U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length =
      embedded_serialization::deserialization<embedded_serialization::BigEndian>(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_integer, little_endian_u64) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u64> data{};
  std::get<0>(data) = 0x0102030405060708;

  auto serialize_length = embedded_serialization::serialization<embedded_serialization::LittleEndian>(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x08;
  expect_data_area[1U] = 0x07;
  expect_data_area[2U] = 0x06;
  expect_data_area[3U] = 0x05;
  expect_data_area[4U] = 0x04;
  expect_data_area[5U] = 0x03;
  expect_data_area[6U] = 0x02;
  expect_data_area[7U] = 0x01;

  EXPECT_EQ(serialize_length, 8);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length =
      embedded_serialization::deserialization<embedded_serialization::LittleEndian>(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

// ██    ██ ██████  ██████
// ██    ██      ██      ██
// ██    ██  █████   █████
// ██    ██      ██ ██
//  ██████  ██████  ███████

TEST(serialize_integer, u32) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u32> data{};
  std::get<0>(data) = 0x01020304;

  auto serialize_length = embedded_serialization::serialization(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x04;
  expect_data_area[1U] = 0x03;
  expect_data_area[2U] = 0x02;
  expect_data_area[3U] = 0x01;

  EXPECT_EQ(serialize_length, 4U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length =
      embedded_serialization::deserialization<embedded_serialization::LittleEndian>(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_integer, u32_u32) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u32, u32> data{};
  std::get<0>(data) = 0x01020304;
  std::get<1>(data) = 0x05060708;

  auto serialize_length = embedded_serialization::serialization(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x04;
  expect_data_area[1U] = 0x03;
  expect_data_area[2U] = 0x02;
  expect_data_area[3U] = 0x01;

  expect_data_area[4U] = 0x08;
  expect_data_area[5U] = 0x07;
  expect_data_area[6U] = 0x06;
  expect_data_area[7U] = 0x05;

  EXPECT_EQ(serialize_length, 8U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length = embedded_serialization::deserialization(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_integer, big_endian_u32) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u32> data{};
  std::get<0>(data) = 0x01020304;

  auto serialize_length = embedded_serialization::serialization<embedded_serialization::BigEndian>(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x04;
  expect_data_area[1U] = 0x03;
  expect_data_area[2U] = 0x02;
  expect_data_area[3U] = 0x01;

  EXPECT_EQ(serialize_length, 4U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length =
      embedded_serialization::deserialization<embedded_serialization::BigEndian>(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_integer, little_endian_u32) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u32> data{};
  std::get<0>(data) = 0x01020304;

  auto serialize_length = embedded_serialization::serialization<embedded_serialization::LittleEndian>(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x04;
  expect_data_area[1U] = 0x03;
  expect_data_area[2U] = 0x02;
  expect_data_area[3U] = 0x01;

  EXPECT_EQ(serialize_length, 4U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length =
      embedded_serialization::deserialization<embedded_serialization::LittleEndian>(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

// ██    ██  ██  ██████
// ██    ██ ███ ██
// ██    ██  ██ ███████
// ██    ██  ██ ██    ██
//  ██████   ██  ██████

TEST(serialize_integer, u16) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u16> data{};
  std::get<0>(data) = 0x0102;

  auto serialize_length = embedded_serialization::serialization(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x02;
  expect_data_area[1U] = 0x01;

  EXPECT_EQ(serialize_length, 2U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length = embedded_serialization::deserialization(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_integer, big_endian_u16) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u16> data{};
  std::get<0>(data) = 0x0102;

  auto serialize_length = embedded_serialization::serialization<embedded_serialization::BigEndian>(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x02;
  expect_data_area[1U] = 0x01;

  EXPECT_EQ(serialize_length, 2U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length =
      embedded_serialization::deserialization<embedded_serialization::BigEndian>(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

TEST(serialize_integer, little_endian_u16) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u16> data{};
  std::get<0>(data) = 0x0102;

  auto serialize_length = embedded_serialization::serialization<embedded_serialization::LittleEndian>(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x02;
  expect_data_area[1U] = 0x01;

  EXPECT_EQ(serialize_length, 2U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length =
      embedded_serialization::deserialization<embedded_serialization::LittleEndian>(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}

// ██    ██  █████
// ██    ██ ██   ██
// ██    ██  █████
// ██    ██ ██   ██
//  ██████   █████

TEST(serialize_integer, u8) {
  std::array<u8, 16> data_area{};
  embedded_serialization::Span<u8> data_span{data_area.data(), data_area.size()};

  std::tuple<u8> data{};
  std::get<0>(data) = 0x0f;

  auto serialize_length = embedded_serialization::serialization(data, data_span);
  EXPECT_EQ(embedded_serialization::get_size(data), serialize_length);

  std::array<u8, 16> expect_data_area{};
  expect_data_area[0U] = 0x0f;

  EXPECT_EQ(serialize_length, 1U);
  EXPECT_EQ(data_area, expect_data_area);

  embedded_serialization::Span<u8> const data_span_const{data_area.data(), data_area.size()};
  decltype(data) out{};
  auto deserialize_length = embedded_serialization::deserialization(data_span_const, out);
  EXPECT_EQ(embedded_serialization::get_size(data), deserialize_length);
  EXPECT_EQ(data, out);
}
