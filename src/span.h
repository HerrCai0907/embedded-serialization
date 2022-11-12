#ifndef __EMBEDDED_SERIALIZATION_SPAN__
#define __EMBEDDED_SERIALIZATION_SPAN__

#include <cassert>
#include <cstdint>

namespace embedded_serialization {

using u32 = uint32_t;
using u8 = uint8_t;

template <class T> class Span {
public:
  Span() noexcept : data_(nullptr), size_(0U) {}
  Span(T *const data, u32 const size) noexcept : data_(data), size_(size) {}

  T &operator[](u32 const index) const noexcept { return data_[index]; }
  T &at(u32 const index) const noexcept {
    assert(index < size_);
    return data_[index];
  }

  Span<T> subspan(u32 const offset) const noexcept { return Span<T>{&at(offset), size_ - offset}; }

private:
  T *data_;
  u32 size_;
};

}; // namespace embedded_serialization

#endif
