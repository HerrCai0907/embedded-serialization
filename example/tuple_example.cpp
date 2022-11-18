#include "serialize.h"
#include "span.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using DataType = std::tuple<uint32_t, embedded_serialization::SerializedSpan<std::tuple<uint16_t, uint16_t>, 0U, 256U>,
                            embedded_serialization::SerializedSpan<uint8_t const, 0U, 128U>>;

using DeserializedDataType = embedded_serialization::DeserializedType<DataType>::type;

std::array<uint8_t, 1024U> data_exchange_array{};

int main() {
  std::vector<std::tuple<uint16_t, uint16_t>> meta{};
  meta.emplace_back(std::forward_as_tuple(1, 2));
  meta.emplace_back(std::forward_as_tuple(128, UINT16_MAX));
  std::string message = "test serialize";

  // prepare data
  DataType data = std::make_tuple(
      25U, embedded_serialization::SerializedSpan<std::tuple<uint16_t, uint16_t>, 0U, 256U>(meta.data(), meta.size()),
      embedded_serialization::SerializedSpan<uint8_t const, 0, 128U>(reinterpret_cast<uint8_t const *>(message.data()),
                                                                     message.size()));
  // serialization
  auto size = embedded_serialization::serialization(
      data, embedded_serialization::Span<uint8_t>(data_exchange_array.data(), data_exchange_array.size()));

  // dump data_exchange_array
  for (uint32_t i = 0; i < size; ++i) {
    std::cout << static_cast<int>(data_exchange_array[i]) << "(" << data_exchange_array[i] << ") ";
  }
  std::cout << std::endl;

  // deserialization
  DeserializedDataType out{};
  embedded_serialization::deserialization<DataType>(
      embedded_serialization::Span<uint8_t>(data_exchange_array.data(), data_exchange_array.size()), out);

  // dump deserialized result
  std::cout << "element 0: " << std::get<0>(out) << std::endl;
  std::cout << "element 1[0][0]: " << std::get<0>(std::get<1>(out)[0]) << std::endl;
  std::cout << "element 1[0][1]: " << std::get<1>(std::get<1>(out)[0]) << std::endl;
  std::cout << "element 1[1][0]: " << std::get<0>(std::get<1>(out)[1]) << std::endl;
  std::cout << "element 1[1][1]: " << std::get<1>(std::get<1>(out)[1]) << std::endl;
  std::cout << "element 2: " << std::get<2>(out).data() << std::endl;
}