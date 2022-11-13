#ifndef __EMBEDDED_SERIALIZATION_SIZE_TRAITS__
#define __EMBEDDED_SERIALIZATION_SIZE_TRAITS__

#include "type.h"
#include <cstdint>
#include <type_traits>

namespace embedded_serialization {

template <u64 Size> class SizeTraits {
public:
  using type = typename std::conditional<
      (Size > UINT32_MAX), u64,
      typename std::conditional<(Size > UINT16_MAX), u32,
                                typename std::conditional<(Size > UINT8_MAX), u16, u8>::type>::type>::type;
};

}; // namespace embedded_serialization

#endif
