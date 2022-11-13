#ifndef __EMBEDDED_SERIALIZATION_SERIALIZE__
#define __EMBEDDED_SERIALIZATION_SERIALIZE__

#include "endian.h"
#include "size_traits.h"
#include "span.h"
#include "type.h"
#include <cstring>
#include <tuple>
#include <type_traits>

namespace embedded_serialization {

namespace detail {

template <class BigEndian, class T> class SingleDataTypeImpl {
public:
  using SupportedType = std::false_type;
};

template <class Endian> class SingleDataTypeImpl<Endian, u8> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u8 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = data;
    return 1U;
  }
};
template <> class SingleDataTypeImpl<BigEndian, u16> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u16 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = static_cast<u8>(data & static_cast<u16>(0x00ffU));
    data_area[1U] = static_cast<u8>((data & static_cast<u16>(0xff00U)) >> 8U);
    return 2U;
  }
};
template <> class SingleDataTypeImpl<LittleEndian, u16> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u16 const &data, Span<u8> data_area) noexcept {
    std::memcpy(&data_area[0], &data, 2U);
    return 2U;
  }
};
template <> class SingleDataTypeImpl<UnknownEndian, u16> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u16 const &data, Span<u8> data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SingleDataTypeImpl<BigEndian, u16>::serialize(data, data_area);
    } else {
      return SingleDataTypeImpl<LittleEndian, u16>::serialize(data, data_area);
    }
  }
};
template <> class SingleDataTypeImpl<BigEndian, u32> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u32 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = static_cast<u8>(data & static_cast<u32>(0x000000ffU));
    data_area[1U] = static_cast<u8>((data & static_cast<u32>(0x0000ff00U)) >> 8U);
    data_area[2U] = static_cast<u8>((data & static_cast<u32>(0x00ff0000U)) >> 16U);
    data_area[3U] = static_cast<u8>((data & static_cast<u32>(0xff000000U)) >> 24U);
    return 4U;
  }
};
template <> class SingleDataTypeImpl<LittleEndian, u32> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u32 const &data, Span<u8> data_area) noexcept {
    std::memcpy(&data_area[0], &data, 4U);
    return 4U;
  }
};
template <> class SingleDataTypeImpl<UnknownEndian, u32> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u32 const &data, Span<u8> data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SingleDataTypeImpl<BigEndian, u32>::serialize(data, data_area);
    } else {
      return SingleDataTypeImpl<LittleEndian, u32>::serialize(data, data_area);
    }
  }
};
template <> class SingleDataTypeImpl<BigEndian, u64> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u64 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = static_cast<u8>(data & static_cast<u64>(0x00000000000000ffU));
    data_area[1U] = static_cast<u8>((data & static_cast<u64>(0x000000000000ff00U)) >> 8U);
    data_area[2U] = static_cast<u8>((data & static_cast<u64>(0x0000000000ff0000U)) >> 16U);
    data_area[3U] = static_cast<u8>((data & static_cast<u64>(0x00000000ff000000U)) >> 24U);
    data_area[4U] = static_cast<u8>((data & static_cast<u64>(0x000000ff00000000U)) >> 32U);
    data_area[5U] = static_cast<u8>((data & static_cast<u64>(0x0000ff0000000000U)) >> 40U);
    data_area[6U] = static_cast<u8>((data & static_cast<u64>(0x00ff000000000000U)) >> 48U);
    data_area[7U] = static_cast<u8>((data & static_cast<u64>(0xff00000000000000U)) >> 56U);
    return 8U;
  }
};
template <> class SingleDataTypeImpl<LittleEndian, u64> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u64 const &data, Span<u8> data_area) noexcept {
    std::memcpy(&data_area[0], &data, 8U);
    return 8U;
  }
};
template <> class SingleDataTypeImpl<UnknownEndian, u64> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u64 const &data, Span<u8> data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SingleDataTypeImpl<BigEndian, u64>::serialize(data, data_area);
    } else {
      return SingleDataTypeImpl<LittleEndian, u64>::serialize(data, data_area);
    }
  }
};
template <class Endian, class T, u32 MinSize, u32 MaxSize>
class SingleDataTypeImpl<Endian, SerializedSpan<T, MinSize, MaxSize>> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(SerializedSpan<T, MinSize, MaxSize> const &data, Span<u8> data_area) noexcept {
    using SizeType = typename SizeTraits<static_cast<u64>(MaxSize)>::type;
    u32 offset = SingleDataTypeImpl<Endian, SizeType>::serialize(data.size(), data_area);
    for (uint32_t i = 0; i < data.size(); ++i) {
      u32 length = SingleDataTypeImpl<Endian, u32>::serialize(data[i], data_area.subspan(offset));
      offset += length;
    }
    return offset;
  }
};
template <class Endian, class T, u32 Size> class SingleDataTypeImpl<Endian, SerializedSpan<T, Size, Size>> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(SerializedSpan<T, Size, Size> const &data, Span<u8> data_area) noexcept {
    u32 offset = 0;
    for (uint32_t i = 0; i < Size; ++i) {
      u32 length = SingleDataTypeImpl<Endian, u32>::serialize(data[i], data_area.subspan(offset));
      offset += length;
    }
    return offset;
  }
};

template <class Endian, u32 Start, u32 End, class... Args> class SerializationImpl {
public:
  u32 operator()(std::tuple<Args...> const &data, Span<u8> const data_area) noexcept {
    using StartElementType = typename std::tuple_element<Start, std::tuple<Args...>>::type;
    using StartElementCleanType = typename std::remove_const<StartElementType>::type;
    static_assert(SingleDataTypeImpl<Endian, StartElementCleanType>::SupportedType::value, "not support type");
    u32 length = SingleDataTypeImpl<Endian, StartElementCleanType>::serialize(std::get<Start>(data), data_area);
    Span<u8> const new_span = data_area.subspan(length);
    u32 other_length = SerializationImpl<Endian, Start + 1, End, Args...>{}(data, new_span);
    return length + other_length;
  }
};
template <class Endian, u32 End, class... Args> class SerializationImpl<Endian, End, End, Args...> {
public:
  u32 operator()(std::tuple<Args...> const &data, Span<u8> const data_area) noexcept { return 0U; }
};

}; // namespace detail

template <class Endian = UnknownEndian, class... Args>
u32 serialization(std::tuple<Args...> const &data, Span<u8> const data_area) noexcept {
  return detail::SerializationImpl<Endian, 0, std::tuple_size<std::tuple<Args...>>::value, Args...>{}(data, data_area);
}

}; // namespace embedded_serialization

#endif
