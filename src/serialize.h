#ifndef __EMBEDDED_SERIALIZATION_SERIALIZE__
#define __EMBEDDED_SERIALIZATION_SERIALIZE__

#include "endian.h"
#include "span.h"
#include "type.h"
#include <cstring>
#include <tuple>
#include <type_traits>

namespace embedded_serialization {

namespace detail {

template <class BigEndian, class T> class SerializationSingleImpl;

template <class Endian> class SerializationSingleImpl<Endian, u8> {
public:
  inline u32 operator()(u8 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = data;
    return 1U;
  }
};
template <> class SerializationSingleImpl<BigEndian, u16> {
public:
  inline u32 operator()(u16 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = static_cast<u8>(data & static_cast<u16>(0x00ffU));
    data_area[1U] = static_cast<u8>((data & static_cast<u16>(0xff00U)) >> 8U);
    return 2U;
  }
};
template <> class SerializationSingleImpl<LittleEndian, u16> {
public:
  inline u32 operator()(u16 const &data, Span<u8> data_area) noexcept {
    std::memcpy(&data_area[0], &data, 2U);
    return 2U;
  }
};
template <> class SerializationSingleImpl<UnknownEndian, u16> {
public:
  inline u32 operator()(u16 const &data, Span<u8> data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializationSingleImpl<BigEndian, u16>{}(data, data_area);
    } else {
      return SerializationSingleImpl<LittleEndian, u16>{}(data, data_area);
    }
  }
};
template <> class SerializationSingleImpl<BigEndian, u32> {
public:
  inline u32 operator()(u32 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = static_cast<u8>(data & static_cast<u32>(0x000000ffU));
    data_area[1U] = static_cast<u8>((data & static_cast<u32>(0x0000ff00U)) >> 8U);
    data_area[2U] = static_cast<u8>((data & static_cast<u32>(0x00ff0000U)) >> 16U);
    data_area[3U] = static_cast<u8>((data & static_cast<u32>(0xff000000U)) >> 24U);
    return 4U;
  }
};
template <> class SerializationSingleImpl<LittleEndian, u32> {
public:
  inline u32 operator()(u32 const &data, Span<u8> data_area) noexcept {
    std::memcpy(&data_area[0], &data, 4U);
    return 4U;
  }
};
template <> class SerializationSingleImpl<UnknownEndian, u32> {
public:
  inline u32 operator()(u32 const &data, Span<u8> data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializationSingleImpl<BigEndian, u32>{}(data, data_area);
    } else {
      return SerializationSingleImpl<LittleEndian, u32>{}(data, data_area);
    }
  }
};
template <> class SerializationSingleImpl<BigEndian, u64> {
public:
  inline u32 operator()(u64 const &data, Span<u8> data_area) noexcept {
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
template <> class SerializationSingleImpl<LittleEndian, u64> {
public:
  inline u32 operator()(u64 const &data, Span<u8> data_area) noexcept {
    std::memcpy(&data_area[0], &data, 8U);
    return 8U;
  }
};
template <> class SerializationSingleImpl<UnknownEndian, u64> {
public:
  inline u32 operator()(u64 const &data, Span<u8> data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializationSingleImpl<BigEndian, u64>{}(data, data_area);
    } else {
      return SerializationSingleImpl<LittleEndian, u64>{}(data, data_area);
    }
  }
};
template <class Endian, class T, u32 MinSize, u32 MaxSize>
class SerializationSingleImpl<Endian, SerializedSpan<T, MinSize, MaxSize>> {
public:
  inline u32 operator()(SerializedSpan<T, MinSize, MaxSize> const &data, Span<u8> data_area) noexcept {
    u32 offset = SerializationSingleImpl<Endian, u32>{}(data.size(), data_area);
    for (uint32_t i = 0; i < data.size(); ++i) {
      u32 length = SerializationSingleImpl<Endian, u32>{}(data[i], data_area.subspan(offset));
      offset += length;
    }
    return offset;
  }
};
template <class Endian, class T, u32 Size> class SerializationSingleImpl<Endian, SerializedSpan<T, Size, Size>> {
public:
  inline u32 operator()(SerializedSpan<T, Size, Size> const &data, Span<u8> data_area) noexcept {
    u32 offset = 0;
    for (uint32_t i = 0; i < Size; ++i) {
      u32 length = SerializationSingleImpl<Endian, u32>{}(data[i], data_area.subspan(offset));
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
    u32 length = SerializationSingleImpl<Endian, StartElementCleanType>{}(std::get<Start>(data), data_area);
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
