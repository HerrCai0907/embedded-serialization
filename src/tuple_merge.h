#ifndef __EMBEDDED_SERIALIZATION_TUPLE_MERGE__
#define __EMBEDDED_SERIALIZATION_TUPLE_MERGE__

#include <tuple>
#include <type_traits>

namespace embedded_serialization {

template <class T, class TupleType> class TupleMerge;
template <class T, class... Args> class TupleMerge<T, std::tuple<Args...>> {
public:
  using type = std::tuple<T, Args...>;
};

} // namespace embedded_serialization

#endif
