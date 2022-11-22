#ifndef __EMBEDDED_SERIALIZATION_SPAN__
#define __EMBEDDED_SERIALIZATION_SPAN__

#include "type.h"
#include <cassert>
#include <type_traits>

namespace embedded_serialization {

template <class T> class Span {
public:
  Span() noexcept : data_(nullptr), size_(0U) {}
  Span(T *const data, u32 const size) noexcept : data_(data), size_(size) {}
  // NOLINTNEXTLINE(google-explicit-constructor)
  Span(Span<typename std::remove_const<T>::type> const &v) noexcept : data_(v.data()), size_(v.size()) {}
  Span &operator=(Span<typename std::remove_const<T>::type> const &v) noexcept {
    this->data_ = v.data();
    this->size_ = v.size();
    return *this;
  }

  T &operator[](u32 const index) const noexcept { return data_[index]; }
  T &at(u32 const index) const noexcept {
    // LCOV_EXCL_START
    assert(index < size_);
    // LCOV_EXCL_STOP
    return data_[index];
  }

  T *data() const noexcept { return data_; }
  u32 size() const noexcept { return size_; }

  Span<T> subspan(u32 const offset) const noexcept { return Span<T>{&at(offset), size_ - offset}; }
  void reset(T *const data, u32 const size) noexcept {
    data_ = data;
    size_ = size;
  }

protected:
  T *data_;
  u32 size_;
};

template <class T, u32 MinSize, u32 MaxSize> class SerializedSpan : public Span<T> {
public:
  using iterator = T *;
  SerializedSpan() noexcept : Span<T>() {}
  SerializedSpan(T *const data, u32 const size) noexcept : Span<T>(data, size) {
    // LCOV_EXCL_START
    assert(size >= MinSize && size <= MaxSize);
    // LCOV_EXCL_STOP
  }
  // NOLINTNEXTLINE(google-explicit-constructor)
  SerializedSpan(SerializedSpan<typename std::remove_const<T>::type, MinSize, MaxSize> const &v) noexcept
      : Span<T>(v.data(), v.size()) {}
  SerializedSpan &operator=(SerializedSpan<typename std::remove_const<T>::type, MinSize, MaxSize> const &v) noexcept {
    this->data_ = v.data();
    this->size_ = v.size();
    return *this;
  }
  iterator begin() const noexcept { return &this->data_[0]; }
  iterator end() const noexcept { return &this->data_[this->size_]; }
};

template <class T, class U, u32 MinSize, u32 MaxSize>
bool operator==(SerializedSpan<T, MinSize, MaxSize> const &lhs,
                SerializedSpan<U, MinSize, MaxSize> const &rhs) noexcept {
  if (lhs.size() != rhs.size()) {
    return false;
  }
  for (u32 i = 0U; i < lhs.size(); ++i) {
    if (lhs[i] != rhs[i]) {
      return false;
    }
  }
  return true;
}
template <class T, class U, u32 MinSize, u32 MaxSize>
bool operator!=(SerializedSpan<T, MinSize, MaxSize> const &lhs,
                SerializedSpan<U, MinSize, MaxSize> const &rhs) noexcept {
  return !(lhs == rhs);
}

} // namespace embedded_serialization

#endif
