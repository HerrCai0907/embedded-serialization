#include "serialize.h"
#include "serialize_interface.h"
#include <cstring>
#include <gtest/gtest.h>

using namespace embedded_serialization;

class ST : public ISerializedData<ST> {
public:
  u32 getSizeImpl() const noexcept { return vec.size() * sizeof(int) + sizeof(u8); }
  u32 serializeImpl(Span<u8> const data_area) const noexcept {
    u32 size = getSizeImpl();
    assert(vec.size() < UINT8_MAX);
    data_area[0U] = static_cast<u8>(vec.size());
    std::memcpy(&data_area[1U], vec.data(), size);
    return size;
  }
  u32 deserializeImpl(Span<const u8> const data_area) noexcept {
    u8 count = data_area[0U];
    vec.resize(static_cast<std::size_t>(count));
    std::memcpy(vec.data(), &data_area[1U], count * sizeof(int));
    return getSizeImpl();
  }
  std::vector<int> vec{};
};

// NOLINTNEXTLINE(cert-err58-cpp, cppcoreguidelines-special-member-functions)
TEST(serialize_user_defined, normal) {
  std::array<u8, 32> data_area{};
  embedded_serialization::Span<u8> const data_span{data_area.data(), data_area.size()};
  embedded_serialization::Span<u8 const> const data_span_const{data_area.data(), data_area.size()};

  ST in{};
  in.vec = std::vector<int>{1, 2, 3, 4, 5, 6};
  serialization(in, data_span);
  ST out{};
  deserialization<ST>(data_span_const, out);
  EXPECT_EQ(in.vec, out.vec);
}