#ifndef __EMBEDDED_SERIALIZATION_SERIALIZE__
#define __EMBEDDED_SERIALIZATION_SERIALIZE__

#include "endian.h"
#include "size_traits.h"
#include "span.h"
#include "type.h"
#include <cassert>
#include <cstring>
#include <tuple>
#include <type_traits>

namespace embedded_serialization {

namespace detail {

template <class BigEndian, class T> class SerializeImpl {
public:
  using SupportedType = std::false_type;
};

template <class Endian> class SerializeImpl<Endian, u8> {
public:
  using SupportedType = std::true_type;
  static inline u32 get_size(u8 const &data) noexcept { return 1U; }
  static inline u32 serialize(u8 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = data;
    return 1U;
  }
  static inline u32 deserialize(Span<u8 const> const data_area, u8 &out) noexcept {
    out = data_area.at(0U);
    return 1U;
  }
};
template <> class SerializeImpl<BigEndian, u16> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u16 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = static_cast<u8>(data & static_cast<u16>(0x00ffU));
    data_area[1U] = static_cast<u8>((data & static_cast<u16>(0xff00U)) >> 8U);
    return 2U;
  }
  static inline u32 deserialize(Span<u8 const> const data_area, u16 &out) noexcept {
    out = static_cast<u16>((static_cast<u32>(data_area.at(1U)) << 8U) + static_cast<u32>(data_area.at(0U)));
    return 2U;
  }
};
template <> class SerializeImpl<LittleEndian, u16> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u16 const &data, Span<u8> data_area) noexcept {
    std::memcpy(&data_area[0], &data, 2U);
    return 2U;
  }
  static inline u32 deserialize(Span<u8 const> const data_area, u16 &out) noexcept {
    std::memcpy(&out, &data_area[0], 2U);
    return 2U;
  }
};
template <> class SerializeImpl<UnknownEndian, u16> {
public:
  using SupportedType = std::true_type;
  static inline u32 get_size(u16 const &data) noexcept { return 2U; }
  static inline u32 serialize(u16 const &data, Span<u8> data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u16>::serialize(data, data_area);
    } else {
      return SerializeImpl<LittleEndian, u16>::serialize(data, data_area);
    }
  }
  static inline u32 deserialize(Span<u8 const> const data_area, u16 &out) noexcept {
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
  static inline u32 serialize(u32 const &data, Span<u8> data_area) noexcept {
    data_area[0U] = static_cast<u8>(data & static_cast<u32>(0x000000ffU));
    data_area[1U] = static_cast<u8>((data & static_cast<u32>(0x0000ff00U)) >> 8U);
    data_area[2U] = static_cast<u8>((data & static_cast<u32>(0x00ff0000U)) >> 16U);
    data_area[3U] = static_cast<u8>((data & static_cast<u32>(0xff000000U)) >> 24U);
    return 4U;
  }
  static inline u32 deserialize(Span<u8 const> const data_area, u32 &out) noexcept {
    out = static_cast<u32>(data_area.at(0U)) + (static_cast<u32>(data_area.at(1U)) << 8U) +
          (static_cast<u32>(data_area.at(2U)) << 16U) + (static_cast<u32>(data_area.at(3U)) << 24U);
    return 4U;
  }
};
template <> class SerializeImpl<LittleEndian, u32> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u32 const &data, Span<u8> data_area) noexcept {
    std::memcpy(&data_area[0], &data, 4U);
    return 4U;
  }
  static inline u32 deserialize(Span<u8 const> const data_area, u32 &out) noexcept {
    std::memcpy(&out, &data_area[0], 4U);
    return 4U;
  }
};
template <> class SerializeImpl<UnknownEndian, u32> {
public:
  using SupportedType = std::true_type;
  static inline u32 get_size(u32 const &data) noexcept { return 4U; }
  static inline u32 serialize(u32 const &data, Span<u8> data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u32>::serialize(data, data_area);
    } else {
      return SerializeImpl<LittleEndian, u32>::serialize(data, data_area);
    }
  }
  static inline u32 deserialize(Span<u8 const> const data_area, u32 &out) noexcept {
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
  static inline u32 deserialize(Span<u8 const> const data_area, u64 &out) noexcept {
    out = static_cast<u64>(data_area.at(0U)) + (static_cast<u64>(data_area.at(1U)) << 8U) +
          (static_cast<u64>(data_area.at(2U)) << 16U) + (static_cast<u64>(data_area.at(3U)) << 24U) +
          (static_cast<u64>(data_area.at(4U)) << 32U) + (static_cast<u64>(data_area.at(5U)) << 40U) +
          (static_cast<u64>(data_area.at(6U)) << 48U) + (static_cast<u64>(data_area.at(7U)) << 56U);
    return 8U;
  }
};
template <> class SerializeImpl<LittleEndian, u64> {
public:
  using SupportedType = std::true_type;
  static inline u32 serialize(u64 const &data, Span<u8> data_area) noexcept {
    std::memcpy(&data_area[0], &data, 8U);
    return 8U;
  }
  static inline u32 deserialize(Span<u8 const> const data_area, u64 &out) noexcept {
    std::memcpy(&out, &data_area[0], 8U);
    return 8U;
  }
};
template <> class SerializeImpl<UnknownEndian, u64> {
public:
  using SupportedType = std::true_type;
  static inline u32 get_size(u64 const &data) noexcept { return 8U; }
  static inline u32 serialize(u64 const &data, Span<u8> data_area) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u64>::serialize(data, data_area);
    } else {
      return SerializeImpl<LittleEndian, u64>::serialize(data, data_area);
    }
  }
  static inline u32 deserialize(Span<u8 const> const data_area, u64 &out) noexcept {
    if (EndianTest::isBigEndian()) {
      return SerializeImpl<BigEndian, u64>::deserialize(data_area, out);
    } else {
      return SerializeImpl<LittleEndian, u64>::deserialize(data_area, out);
    }
  }
};
template <class Endian, class T, u32 MinSize, u32 MaxSize>
class SerializeImpl<Endian, SerializedSpan<T, MinSize, MaxSize>> {
  using SizeType = typename SizeTraits<static_cast<u64>(MaxSize)>::type;

public:
  using SupportedType = std::true_type;
  static inline u32 get_size(SerializedSpan<T, MinSize, MaxSize> const &data) noexcept {
    u32 size = sizeof(SizeType);
    for (u32 i = 0; i < data.size(); ++i) {
      size += SerializeImpl<Endian, T>::get_size(data[i]);
    }
    return size;
  }
  static inline u32 serialize(SerializedSpan<T, MinSize, MaxSize> const &data, Span<u8> data_area) noexcept {
    u32 offset = SerializeImpl<Endian, SizeType>::serialize(data.size(), data_area);
    for (uint32_t i = 0; i < data.size(); ++i) {
      u32 length = SerializeImpl<Endian, T>::serialize(data[i], data_area.subspan(offset));
      offset += length;
    }
    return offset;
  }
  static inline u32 deserialize(Span<u8 const> const data_area, SerializedSpan<T, MinSize, MaxSize> &out) noexcept {
    // TODO(handle nest span)
    u32 size = 0U;
    u32 offset = SerializeImpl<Endian, SizeType>::deserialize(data_area, size);
    out.reset(&data_area.at(offset), size);
    return offset + size * sizeof(T);
  }
};
template <class Endian, class T, u32 Size> class SerializeImpl<Endian, SerializedSpan<T, Size, Size>> {
public:
  using SupportedType = std::true_type;
  static inline u32 get_size(SerializedSpan<T, Size, Size> const &data) noexcept {
    u32 size = 0;
    for (u32 i = 0; i < data.size(); ++i) {
      size += SerializeImpl<Endian, T>::get_size(data[i]);
    }
    return size;
  }
  static inline u32 serialize(SerializedSpan<T, Size, Size> const &data, Span<u8> data_area) noexcept {
    u32 offset = 0;
    for (uint32_t i = 0; i < Size; ++i) {
      u32 length = SerializeImpl<Endian, T>::serialize(data[i], data_area.subspan(offset));
      offset += length;
    }
    return offset;
  }
  static inline u32 deserialize(Span<u8 const> const data_area, SerializedSpan<T, Size, Size> &out) noexcept {
    // TODO(handle nest span)
    out.reset(&data_area.at(0), Size);
    return Size * sizeof(T);
  }
};
template <class Endian, class... Args> class SerializeImpl<Endian, std::tuple<Args...>> {
  using DataType = std::tuple<Args...>;

  template <u32 Start, u32 End> class TupleSerializeImpl {
    using StartElementType = typename std::tuple_element<Start, std::tuple<Args...>>::type;
    using StartElementCleanType = typename std::remove_const<StartElementType>::type;
    static_assert(SerializeImpl<Endian, StartElementCleanType>::SupportedType::value, "not support type");

  public:
    static inline u32 get_size(DataType const &data) noexcept {
      return SerializeImpl<Endian, StartElementCleanType>::get_size(std::get<Start>(data)) +
             TupleSerializeImpl<Start + 1, End>::get_size(data);
    }
    static inline u32 serialize(DataType const &data, Span<u8> const data_area) noexcept {
      u32 const length = SerializeImpl<Endian, StartElementCleanType>::serialize(std::get<Start>(data), data_area);
      u32 const other_length = TupleSerializeImpl<Start + 1, End>::serialize(data, data_area.subspan(length));
      return length + other_length;
    }
    static inline u32 deserialize(Span<u8 const> const data_area, DataType &out) noexcept {
      u32 const length = SerializeImpl<Endian, StartElementCleanType>::deserialize(data_area, std::get<Start>(out));
      u32 const other_length = TupleSerializeImpl<Start + 1, End>::deserialize(data_area.subspan(length), out);
      return length + other_length;
    }
  };
  template <u32 End> class TupleSerializeImpl<End, End> {
  public:
    static inline u32 get_size(DataType const &data) noexcept {
      static_cast<void>(data);
      return 0U;
    }
    static inline u32 serialize(DataType const &data, Span<u8> const data_area) noexcept {
      static_cast<void>(data);
      static_cast<void>(data_area);
      return 0U;
    }
    static inline u32 deserialize(Span<u8 const> const data_area, DataType &out) noexcept {
      static_cast<void>(data_area);
      static_cast<void>(out);
      return 0U;
    }
  };

public:
  using SupportedType = std::true_type;

  static inline u32 get_size(DataType const &data) noexcept {
    return TupleSerializeImpl<0U, std::tuple_size<DataType>::value>::get_size(data);
  }
  static inline u32 serialize(DataType const &data, Span<u8> data_area) noexcept {
    return TupleSerializeImpl<0U, std::tuple_size<DataType>::value>::serialize(data, data_area);
  }
  static inline u32 deserialize(Span<u8 const> const data_area, DataType &out) noexcept {
    return TupleSerializeImpl<0U, std::tuple_size<DataType>::value>::deserialize(data_area, out);
  }
};

} // namespace detail

template <class T> u32 get_size(T const &data) noexcept {
  return detail::SerializeImpl<UnknownEndian, T>::get_size(data);
}
template <class Endian = UnknownEndian, class T> u32 serialization(T const &data, Span<u8> const data_area) noexcept {
  // LCOV_EXCL_START
  assert(data_area.size() >= get_size(data) && "data size not enough");
  // LCOV_EXCL_STOP
  return detail::SerializeImpl<Endian, T>::serialize(data, data_area);
}
template <class Endian = UnknownEndian, class T> u32 deserialization(Span<u8 const> const data_area, T &out) noexcept {
  return detail::SerializeImpl<Endian, T>::deserialize(data_area, out);
}

} // namespace embedded_serialization

#endif
