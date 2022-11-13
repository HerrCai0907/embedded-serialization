#include "size_traits.h"
#include <gtest/gtest.h>

TEST(size_traits, u8) {
  EXPECT_EQ(sizeof(embedded_serialization::SizeTraits<0U>::type), 1U);
  EXPECT_EQ(sizeof(embedded_serialization::SizeTraits<255U>::type), 1U);
}

TEST(size_traits, u16) {
  EXPECT_EQ(sizeof(embedded_serialization::SizeTraits<256U>::type), 2U);
  EXPECT_EQ(sizeof(embedded_serialization::SizeTraits<65535U>::type), 2U);
}

TEST(size_traits, u32) {
  EXPECT_EQ(sizeof(embedded_serialization::SizeTraits<65536U>::type), 4U);
  EXPECT_EQ(sizeof(embedded_serialization::SizeTraits<0xffffffff>::type), 4U);
}

TEST(size_traits, u64) {
  EXPECT_EQ(sizeof(embedded_serialization::SizeTraits<4294967296U>::type), 8U);
  EXPECT_EQ(sizeof(embedded_serialization::SizeTraits<18446744073709551615LU>::type), 8U);
}