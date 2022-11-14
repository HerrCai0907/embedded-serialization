#ifndef __EMBEDDED_SERIALIZATION_ENDIAN__
#define __EMBEDDED_SERIALIZATION_ENDIAN__

#ifdef __cpp_lib_endian
#include <bit>
#endif

namespace embedded_serialization {

class EndianTest {
public:
#ifdef __cpp_lib_endian
  static bool isBigEndian() noexcept { return std::endian::native == std::endian::big; }
#else
  static bool isBigEndian() noexcept {
    union EndianTestUnion {
      int i;
      char c;
    };
    EndianTestUnion u;
    u.i = 1U;
    return u.c != 1U;
  }
#endif
};

struct BigEndian {};
struct LittleEndian {};
struct UnknownEndian {};

} // namespace embedded_serialization

#endif
