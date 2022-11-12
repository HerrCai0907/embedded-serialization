#ifndef __EMBEDDED_SERIALIZATION_SERIALIZE__
#define __EMBEDDED_SERIALIZATION_SERIALIZE__

#include "endian.h"
#include "span.h"
#include <cstdint>
#include <cstring>
#include <tuple>

namespace embedded_serialization {

using u32 = uint32_t;
using u8 = uint8_t;

namespace detail {

template <class BigEndian, class T> class SerializationSingleImpl;
template <> class SerializationSingleImpl<BigEndian, u32> {
public:
  inline u32 operator()(u32 const &data, Span<u8> data_area) {
    data_area[0U] = static_cast<u8>(data & static_cast<u32>(0x000000ffU));
    data_area[1U] = static_cast<u8>((data & static_cast<u32>(0x0000ff00U)) >> 8U);
    data_area[2U] = static_cast<u8>((data & static_cast<u32>(0x00ff0000U)) >> 16U);
    data_area[3U] = static_cast<u8>((data & static_cast<u32>(0xff000000U)) >> 24U);
    return 4U;
  }
};
template <> class SerializationSingleImpl<LittleEndian, u32> {
public:
  inline u32 operator()(u32 const &data, Span<u8> data_area) {
    std::memcpy(&data_area[0], &data, 4U);
    return 4U;
  }
};
template <> class SerializationSingleImpl<UnknownEndian, u32> {
public:
  inline u32 operator()(u32 const &data, Span<u8> data_area) {
    if (EndianTest::isBigEndian()) {
      return SerializationSingleImpl<BigEndian, u32>{}(data, data_area);
    } else {
      return SerializationSingleImpl<LittleEndian, u32>{}(data, data_area);
    }
  }
};

template <class Endian, u32 Start, u32 End, class... Args> class SerializationImpl {
public:
  u32 operator()(std::tuple<Args...> const &data, Span<u8> const data_area) noexcept {
    u32 length = SerializationSingleImpl<Endian, typename std::tuple_element<Start, std::tuple<Args...>>::type>{}(
        std::get<Start>(data), data_area);
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
