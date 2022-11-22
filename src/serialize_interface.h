#ifndef __EMBEDDED_SERIALIZATION_SERIALIZE_INTERFACE__
#define __EMBEDDED_SERIALIZATION_SERIALIZE_INTERFACE__

#include "span.h"
#include "type.h"

namespace embedded_serialization {

template <class T> class ISerializedData {
public:
  u32 get_size() const noexcept { return static_cast<T const *>(this)->getSizeImpl(); }
  u32 serialize(Span<u8> const data_area) const noexcept {
    return static_cast<T const *>(this)->serializeImpl(data_area);
  }
  u32 deserialize(Span<const u8> const data_area) noexcept {
    return static_cast<T *>(this)->deserializeImpl(data_area);
  }

protected:
  u32 getSizeImpl() const noexcept { return 0U; }
  u32 serializeImpl(Span<u8> const /*data_area*/) const noexcept { return 0U; }
  u32 deserializeImpl(Span<const u8> const /*data_area*/) noexcept { return 0U; }
};

} // namespace embedded_serialization

#endif
