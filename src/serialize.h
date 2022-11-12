#ifndef __EMBEDDED_SERIALIZATION_SERIALIZE__
#define __EMBEDDED_SERIALIZATION_SERIALIZE__

#include "span.h"
#include <cstdint>
#include <cstring>
#include <tuple>

namespace embedded_serialization {

using u32 = uint32_t;
using u8 = uint8_t;

namespace detail {

template <class T> inline u32 serialization_single_impl(T const &data, Span<u8> const data_area) noexcept;
template <> inline u32 serialization_single_impl(u32 const &data, Span<u8> data_area) noexcept {
  std::memcpy(&data_area[0], &data, 4U);
  return 4U;
}

template <u32 Start, u32 End, class... Args> class SerializationImpl {
public:
  u32 operator()(std::tuple<Args...> const &data, Span<u8> const data_area) noexcept {
    u32 length = serialization_single_impl(std::get<Start>(data), data_area);
    Span<u8> const new_span = data_area.subspan(length);
    u32 other_length = SerializationImpl<Start + 1, End, Args...>{}(data, new_span);
    return length + other_length;
  }
};
template <u32 End, class... Args> class SerializationImpl<End, End, Args...> {
public:
  u32 operator()(std::tuple<Args...> const &data, Span<u8> const data_area) noexcept { return 0U; }
};

}; // namespace detail

template <class... Args> u32 serialization(std::tuple<Args...> const &data, Span<u8> const data_area) noexcept {
  return detail::SerializationImpl<0, std::tuple_size<std::tuple<Args...>>::value, Args...>{}(data, data_area);
}

}; // namespace embedded_serialization

#endif