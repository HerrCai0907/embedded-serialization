#ifndef __EMBEDDED_SERIALIZATION_SERIALIZE__
#define __EMBEDDED_SERIALIZATION_SERIALIZE__

#include "endian.h"
#include "serialize_interface.h"
#include "size_traits.h"
#include "span.h"
#include "tuple_merge.h"
#include "type.h"
#include <array>
#include <cassert>
#include <cstring>
#include <tuple>
#include <type_traits>

namespace embedded_serialization {

namespace detail {

template <class Endian, class T> class SerializeImpl;

template <class Endian, class T> class NotSupportedSerializeImpl {
public:
  using SupportedType =
      typename std::conditional<std::is_base_of<ISerializedData<T>, T>::value, std::true_type, std::false_type>::type;
  using SupportedSpanContainer = std::false_type;
  using DeserializedType = T;
  static_assert(SupportedType::value, "not supported type");

  static inline u32 get_size(T const &data) noexcept { return data.get_size(); }
  static inline u32 serialize(T const &data, Span<u8> const data_area) noexcept { return data.serialize(data_area); }
  static inline u32 deserialize(Span<const u8> const data_area, T &out) noexcept { return out.deserialize(data_area); }
};
template <class Endian, class T> class ClassSerializeImpl {
public:
  using SupportedType = std::true_type;
  using SupportedSpanContainer = std::false_type;
  using DeserializedType = T;

  static inline u32 get_size(T const &data) noexcept { return data.get_size(); }
  static inline u32 serialize(T const &data, Span<u8> const data_area) noexcept { return data.serialize(data_area); }
  static inline u32 deserialize(Span<const u8> const data_area, T &out) noexcept { return out.deserialize(data_area); }
};
template <class Endian, class T> class EnumSerializeImplImpl {
  using UnderlyingType = typename std::make_unsigned<typename std::underlying_type<T>::type>::type;

public:
  using SupportedType = typename SerializeImpl<Endian, UnderlyingType>::SupportedType;
  using SupportedSpanContainer = typename SerializeImpl<Endian, UnderlyingType>::SupportedSpanContainer;
  using DeserializedType = T;

  static inline u32 get_size(T const &data) noexcept {
    return SerializeImpl<Endian, UnderlyingType>::get_size(static_cast<UnderlyingType>(data));
  }
  static inline u32 serialize(T const &data, Span<u8> const data_area) noexcept {
    return SerializeImpl<Endian, UnderlyingType>::serialize(static_cast<UnderlyingType const>(data), data_area);
  }
  static inline u32 deserialize(Span<const u8> const data_area, T &out) noexcept {
    UnderlyingType underlying_out;
    u32 length = SerializeImpl<Endian, UnderlyingType>::deserialize(data_area, underlying_out);
    out = static_cast<T>(underlying_out);
    return length;
  }
};

template <class Endian, class T,
          typename std::enable_if<!(std::is_base_of<ISerializedData<T>, T>::value || std::is_enum<T>::value),
                                  bool>::type = true>
NotSupportedSerializeImpl<Endian, T> typeDispatch();
template <class Endian, class T,
          typename std::enable_if<std::is_base_of<ISerializedData<T>, T>::value, bool>::type = true>
ClassSerializeImpl<Endian, T> typeDispatch();
template <class Endian, class T, typename std::enable_if<std::is_enum<T>::value, bool>::type = true>
EnumSerializeImplImpl<Endian, T> typeDispatch();

template <class Endian, class T> class SerializeImpl {
  using ImplType = decltype(typeDispatch<Endian, T>());

public:
  using SupportedType = typename ImplType::SupportedType;
  using SupportedSpanContainer = typename ImplType::SupportedSpanContainer;
  using DeserializedType = typename ImplType::DeserializedType;

  static inline u32 get_size(T const &data) noexcept { return ImplType::get_size(data); }
  static inline u32 serialize(T const &data, Span<u8> const data_area) noexcept {
    return ImplType::serialize(data, data_area);
  }
  static inline u32 deserialize(Span<const u8> const data_area, T &out) noexcept {
    return ImplType::deserialize(data_area, out);
  }
};

// ?????? ?????????    ?????? ???????????????????????? ?????????????????????  ??????????????????  ????????????????????? ??????????????????
// ?????? ????????????   ??????    ??????    ??????      ??????       ??????      ??????   ??????
// ?????? ?????? ??????  ??????    ??????    ???????????????   ??????   ????????? ???????????????   ??????????????????
// ?????? ??????  ?????? ??????    ??????    ??????      ??????    ?????? ??????      ??????   ??????
// ?????? ??????   ????????????    ??????    ?????????????????????  ??????????????????  ????????????????????? ??????   ??????

template <class Endian> class SerializeImpl<Endian, u8> {
public:
  using SupportedType = std::true_type;
  using SupportedSpanContainer = std::true_type;
  using DeserializedType = u8;
  static inline u32 get_size(u8 const & /*data*/) noexcept { return 1U; }
  static inline u32 serialize(u8 const &data, Span<u8> const data_area) noexcept {
    data_area[0U] = data;
    return 1U;
  }
  static inline u32 deserialize(Span<const u8> const data_area, u8 &out) noexcept {
    out = data_area.at(0U);
    return 1U;
  }
};
template <> class SerializeImpl<BigEndian, u16> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u16 const &data, Span<u8> const data_area) noexcept {
    data_area[0U] = static_cast<u8>(data);
    data_area[1U] = static_cast<u8>(data >> 8U);
    return 2U;
  }
  static inline u32 deserialize(Span<const u8> const data_area, u16 &out) noexcept {
    out = static_cast<u16>((static_cast<u32>(data_area.at(1U)) << 8U) + static_cast<u32>(data_area.at(0U)));
    return 2U;
  }
};
template <> class SerializeImpl<LittleEndian, u16> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u16 const &data, Span<u8> const data_area) noexcept {
    std::memcpy(&data_area[0], &data, 2U);
    return 2U;
  }
  static inline u32 deserialize(Span<const u8> const data_area, u16 &out) noexcept {
    std::memcpy(&out, &data_area[0], 2U);
    return 2U;
  }
};
template <> class SerializeImpl<UnknownEndian, u16> {
public:
  using SupportedType = std::true_type;
  using SupportedSpanContainer = std::true_type;
  using DeserializedType = u16;
  static inline u32 get_size(u16 const & /*data*/) noexcept { return 2U; }
  static inline u32 serialize(u16 const &data, Span<u8> const data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u16>::serialize(data, data_area);
    } else {
      return SerializeImpl<LittleEndian, u16>::serialize(data, data_area);
    }
  }
  static inline u32 deserialize(Span<const u8> const data_area, u16 &out) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u16>::deserialize(data_area, out);
    } else {
      return SerializeImpl<LittleEndian, u16>::deserialize(data_area, out);
    }
  }
};
template <> class SerializeImpl<BigEndian, u32> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u32 const &data, Span<u8> const data_area) noexcept {
    u32 v = data;
    data_area[0U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[1U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[2U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[3U] = static_cast<u8>(v);
    return 4U;
  }
  static inline u32 deserialize(Span<const u8> const data_area, u32 &out) noexcept {
    out = static_cast<u32>(data_area.at(3U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(2U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(1U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(0U));
    return 4U;
  }
};
template <> class SerializeImpl<LittleEndian, u32> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u32 const &data, Span<u8> const data_area) noexcept {
    std::memcpy(&data_area[0], &data, 4U);
    return 4U;
  }
  static inline u32 deserialize(Span<const u8> const data_area, u32 &out) noexcept {
    std::memcpy(&out, &data_area[0], 4U);
    return 4U;
  }
};
template <> class SerializeImpl<UnknownEndian, u32> {
public:
  using SupportedType = std::true_type;
  using SupportedSpanContainer = std::true_type;
  using DeserializedType = u32;
  static inline u32 get_size(u32 const & /*data*/) noexcept { return 4U; }
  static inline u32 serialize(u32 const &data, Span<u8> const data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u32>::serialize(data, data_area);
    } else {
      return SerializeImpl<LittleEndian, u32>::serialize(data, data_area);
    }
  }
  static inline u32 deserialize(Span<const u8> const data_area, u32 &out) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u32>::deserialize(data_area, out);
    } else {
      return SerializeImpl<LittleEndian, u32>::deserialize(data_area, out);
    }
  }
};
template <> class SerializeImpl<BigEndian, u64> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u64 const &data, Span<u8> const data_area) noexcept {
    u64 v = data;
    data_area[0U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[1U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[2U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[3U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[4U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[5U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[6U] = static_cast<u8>(v);
    v >>= 8U;
    data_area[7U] = static_cast<u8>(v);
    return 8U;
  }
  static inline u32 deserialize(Span<const u8> const data_area, u64 &out) noexcept {
    out = static_cast<u32>(data_area.at(7U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(6U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(5U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(4U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(3U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(2U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(1U));
    out <<= 8U;
    out += static_cast<u32>(data_area.at(0U));
    return 8U;
  }
};
template <> class SerializeImpl<LittleEndian, u64> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u64 const &data, Span<u8> const data_area) noexcept {
    std::memcpy(&data_area[0], &data, 8U);
    return 8U;
  }
  static inline u32 deserialize(Span<const u8> const data_area, u64 &out) noexcept {
    std::memcpy(&out, &data_area[0], 8U);
    return 8U;
  }
};
template <> class SerializeImpl<UnknownEndian, u64> {
public:
  using SupportedType = std::true_type;
  using SupportedSpanContainer = std::true_type;
  using DeserializedType = u64;
  static inline u32 get_size(u64 const & /*data*/) noexcept { return 8U; }
  static inline u32 serialize(u64 const &data, Span<u8> const data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u64>::serialize(data, data_area);
    } else {
      return SerializeImpl<LittleEndian, u64>::serialize(data, data_area);
    }
  }
  static inline u32 deserialize(Span<const u8> const data_area, u64 &out) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u64>::deserialize(data_area, out);
    } else {
      return SerializeImpl<LittleEndian, u64>::deserialize(data_area, out);
    }
  }
};

//  ???????????????  ??????????????????  ??????????????????   ???????????????  ??????    ??????
// ??????   ?????? ??????   ?????? ??????   ?????? ??????   ??????  ??????  ??????
// ????????????????????? ??????????????????  ??????????????????  ?????????????????????   ????????????
// ??????   ?????? ??????   ?????? ??????   ?????? ??????   ??????    ??????
// ??????   ?????? ??????   ?????? ??????   ?????? ??????   ??????    ??????

template <class Endian, class T, u32 MinSize, u32 MaxSize>
class SerializeImpl<Endian, SerializedSpan<T, MinSize, MaxSize>> {
  using SizeType = typename SizeTraits<static_cast<u64>(MaxSize)>::type;
  using CleanType = typename std::remove_const<T>::type;
  using ElementImplType = SerializeImpl<UnknownEndian, CleanType>;

public:
  using SupportedType = std::true_type;
  using SupportedSpanContainer = std::false_type;
  using DeserializedType =
      typename std::conditional<ElementImplType::SupportedSpanContainer::value,
                                SerializedSpan<const typename ElementImplType::DeserializedType, MinSize, MaxSize>,
                                SerializedArray<typename ElementImplType::DeserializedType, MaxSize>>::type;
  static inline u32 get_size(SerializedSpan<T, MinSize, MaxSize> const &data) noexcept {
    u32 size = sizeof(SizeType);
    for (u32 i = 0; i < data.size(); ++i) {
      size += SerializeImpl<Endian, CleanType>::get_size(data[i]);
    }
    return size;
  }
  static inline u32 serialize(SerializedSpan<T, MinSize, MaxSize> const &data, Span<u8> const data_area) noexcept {
    u32 offset = SerializeImpl<Endian, SizeType>::serialize(data.size(), data_area);
    for (uint32_t i = 0; i < data.size(); ++i) {
      u32 length = SerializeImpl<Endian, CleanType>::serialize(data[i], data_area.subspan(offset));
      offset += length;
    }
    return offset;
  }
  static inline u32 deserialize(Span<const u8> const data_area, DeserializedType &out) noexcept {
    return deserialize(data_area, out, typename ElementImplType::SupportedSpanContainer{});
  }

private:
  static inline u32 deserialize(Span<const u8> const data_area, DeserializedType &out,
                                std::true_type /*isSpanContainer*/) noexcept {
    SizeType size{};
    u32 offset = SerializeImpl<Endian, SizeType>::deserialize(data_area, size);
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    out.reset(reinterpret_cast<const typename ElementImplType::DeserializedType *>(&data_area.at(offset)), size);
    return offset + size * sizeof(T);
  }
  static inline u32 deserialize(Span<const u8> const data_area, DeserializedType &out,
                                std::false_type /*isSpanContainer*/) noexcept {
    SizeType size{};
    u32 offset = SerializeImpl<Endian, SizeType>::deserialize(data_area, size);
    for (SizeType i = 0; i < size; ++i) {
      u32 length = SerializeImpl<Endian, CleanType>::deserialize(data_area.subspan(offset), out[i]);
      offset += length;
    }
    out.setActualSize(static_cast<u32>(size));
    return offset;
  }
};
template <class Endian, class T, u32 Size> class SerializeImpl<Endian, SerializedSpan<T, Size, Size>> {
  using CleanType = typename std::remove_const<T>::type;
  using ElementImplType = SerializeImpl<UnknownEndian, CleanType>;

public:
  using SupportedType = std::true_type;
  using SupportedSpanContainer = std::false_type;
  using DeserializedType =
      typename std::conditional<ElementImplType::SupportedSpanContainer::value,
                                SerializedSpan<const typename ElementImplType::DeserializedType, Size, Size>,
                                std::array<typename ElementImplType::DeserializedType, Size>>::type;
  static inline u32 get_size(SerializedSpan<T, Size, Size> const &data) noexcept {
    u32 size = 0;
    for (u32 i = 0; i < data.size(); ++i) {
      size += SerializeImpl<Endian, CleanType>::get_size(data[i]);
    }
    return size;
  }
  static inline u32 serialize(SerializedSpan<T, Size, Size> const &data, Span<u8> const data_area) noexcept {
    u32 offset = 0;
    for (uint32_t i = 0; i < Size; ++i) {
      u32 length = SerializeImpl<Endian, CleanType>::serialize(data[i], data_area.subspan(offset));
      offset += length;
    }
    return offset;
  }
  static inline u32 deserialize(Span<const u8> const data_area, DeserializedType &out) noexcept {
    return deserialize(data_area, out, typename ElementImplType::SupportedSpanContainer{});
  }

private:
  static inline u32 deserialize(Span<const u8> const data_area, DeserializedType &out,
                                std::true_type /*isSpanContainer*/) noexcept {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
    out.reset(reinterpret_cast<const typename ElementImplType::DeserializedType *>(&data_area.at(0)), Size);
    return Size * sizeof(T);
  }
  static inline u32 deserialize(Span<const u8> const data_area, DeserializedType &out,
                                std::false_type /*isSpanContainer*/) noexcept {
    u32 offset = 0;
    for (u32 i = 0; i < Size; ++i) {
      u32 length = SerializeImpl<Endian, CleanType>::deserialize(data_area.subspan(offset), out[i]);
      offset += length;
    }
    return offset;
  }
};

// ???????????????????????? ??????    ?????? ??????????????????  ??????      ?????????????????????
//    ??????    ??????    ?????? ??????   ?????? ??????      ??????
//    ??????    ??????    ?????? ??????????????????  ??????      ???????????????
//    ??????    ??????    ?????? ??????      ??????      ??????
//    ??????     ??????????????????  ??????      ????????????????????? ?????????????????????

template <class Endian, class... Args> class SerializeImpl<Endian, std::tuple<Args...>> {
  using DataType = std::tuple<Args...>;

  template <u32 Start, u32 End> class TupleSerializeImpl {
    using StartElementType = typename std::tuple_element<Start, std::tuple<Args...>>::type;
    using StartElementCleanType = typename std::remove_const<StartElementType>::type;
    using StartElementImplType = SerializeImpl<UnknownEndian, StartElementCleanType>;
    static_assert(StartElementImplType::SupportedType::value, "not support type");

  public:
    using RecursionDeserializedType =
        typename TupleMerge<typename StartElementImplType::DeserializedType,
                            typename TupleSerializeImpl<Start + 1, End>::RecursionDeserializedType>::type;
    static inline u32 get_size(DataType const &data) noexcept {
      return StartElementImplType::get_size(std::get<Start>(data)) + TupleSerializeImpl<Start + 1, End>::get_size(data);
    }
    static inline u32 serialize(DataType const &data, Span<u8> const data_area) noexcept {
      u32 const length = SerializeImpl<Endian, StartElementCleanType>::serialize(std::get<Start>(data), data_area);
      u32 const other_length = TupleSerializeImpl<Start + 1, End>::serialize(data, data_area.subspan(length));
      return length + other_length;
    }
  };
  template <u32 End> class TupleSerializeImpl<End, End> {
  public:
    using RecursionDeserializedType = std::tuple<>;
    static inline u32 get_size(DataType const &data) noexcept {
      static_cast<void>(data);
      return 0U;
    }
    static inline u32 serialize(DataType const &data, Span<u8> const data_area) noexcept {
      static_cast<void>(data);
      static_cast<void>(data_area);
      return 0U;
    }
  };

public:
  using SupportedType = std::true_type;
  using DeserializedType = typename TupleSerializeImpl<0U, std::tuple_size<DataType>::value>::RecursionDeserializedType;
  using SupportedSpanContainer = std::false_type;

private:
  template <u32 Start, u32 End> class TupleDeserializeImpl {
    using StartElementType = typename std::tuple_element<Start, std::tuple<Args...>>::type;
    using StartElementCleanType = typename std::remove_const<StartElementType>::type;

  public:
    static inline u32 deserialize(Span<const u8> const data_area, DeserializedType &out) noexcept {
      u32 const length = SerializeImpl<Endian, StartElementCleanType>::deserialize(data_area, std::get<Start>(out));
      u32 const other_length = TupleDeserializeImpl<Start + 1, End>::deserialize(data_area.subspan(length), out);
      return length + other_length;
    }
  };

  template <u32 End> class TupleDeserializeImpl<End, End> {
  public:
    static inline u32 deserialize(Span<const u8> const data_area, DeserializedType &out) noexcept {
      static_cast<void>(data_area);
      static_cast<void>(out);
      return 0U;
    }
  };

public:
  static inline u32 get_size(DataType const &data) noexcept {
    return TupleSerializeImpl<0U, std::tuple_size<DataType>::value>::get_size(data);
  }
  static inline u32 serialize(DataType const &data, Span<u8> const data_area) noexcept {
    return TupleSerializeImpl<0U, std::tuple_size<DataType>::value>::serialize(data, data_area);
  }
  static inline u32 deserialize(Span<const u8> const data_area, DeserializedType &out) noexcept {
    return TupleDeserializeImpl<0U, std::tuple_size<DataType>::value>::deserialize(data_area, out);
  }
};

} // namespace detail

template <class T> class DeserializedType {
public:
  using type = typename detail::SerializeImpl<UnknownEndian, T>::DeserializedType;
};

template <class T> u32 get_size(T const &data) noexcept {
  return detail::SerializeImpl<UnknownEndian, T>::get_size(data);
}
template <class T, class Endian = UnknownEndian> u32 serialization(T const &data, Span<u8> const data_area) noexcept {
  // LCOV_EXCL_START
  assert(data_area.size() >= get_size(data) && "data size not enough");
  // LCOV_EXCL_STOP
  return detail::SerializeImpl<Endian, T>::serialize(data, data_area);
}
template <class T, class Endian = UnknownEndian>
u32 deserialization(Span<const u8> const data_area, typename DeserializedType<T>::type &out) noexcept {
  return detail::SerializeImpl<Endian, T>::deserialize(data_area, out);
}

} // namespace embedded_serialization

#endif
