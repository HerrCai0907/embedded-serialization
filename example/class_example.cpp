#include "serialize.h"
#include "serialize_interface.h"
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

class Data : public embedded_serialization::ISerializedData<Data> {
public:
  void dump() const noexcept {
    std::cout << "vec: [";
    for (auto v : vec_) {
      std::cout << v << " ";
    }
    std::cout << "]\n";

    std::cout << "arr: [";
    for (auto v : arr_) {
      std::cout << v << " ";
    }
    std::cout << "]\n";

    std::cout << "state: ";
    switch (state_) {
    case State::A:
      std::cout << "A";
      break;
    case State::C:
      std::cout << "C";
      break;

    default:
      break;
    }
    std::cout << std::endl;
  }

private:
  std::vector<uint32_t> vec_{1, 2, 3, 4};
  std::array<uint64_t, 5U> arr_{2, 4, 8, 16, 32};
  enum class State : uint8_t { A, C } state_{State::C};

  using VecSpan = embedded_serialization::SerializedSpan<const uint32_t, 0U, 16U>;
  using ArrSpan = embedded_serialization::SerializedSpan<const uint64_t, 5U, 5U>;
  using TupleWrapper = std::tuple<VecSpan, ArrSpan, uint8_t>;
  uint32_t getSizeImpl() const noexcept {
    TupleWrapper tuple_data =
        std::make_tuple(VecSpan{vec_.data(), static_cast<uint32_t>(vec_.size())},
                        ArrSpan{arr_.data(), static_cast<uint32_t>(arr_.size())}, static_cast<uint8_t>(state_));
    return embedded_serialization::get_size(tuple_data);
  }
  uint32_t serializeImpl(embedded_serialization::Span<uint8_t> const data_area) const noexcept {
    TupleWrapper tuple_data =
        std::make_tuple(VecSpan{vec_.data(), static_cast<uint32_t>(vec_.size())},
                        ArrSpan{arr_.data(), static_cast<uint32_t>(arr_.size())}, static_cast<uint8_t>(state_));
    return embedded_serialization::serialization(tuple_data, data_area);
  }
  uint32_t deserializeImpl(embedded_serialization::Span<const uint8_t> const data_area) noexcept {
    embedded_serialization::DeserializedType<TupleWrapper>::type tuple_data{};
    uint32_t size = embedded_serialization::deserialization<TupleWrapper>(data_area, tuple_data);
    vec_ = std::vector<uint32_t>{std::get<0>(tuple_data).begin(), std::get<0>(tuple_data).end()};
    for (uint32_t i = 0; i < std::get<1>(tuple_data).size(); ++i) {
      arr_[i] = std::get<1>(tuple_data)[i];
    }
    state_ = static_cast<State>(std::get<2>(tuple_data));
    return size;
  }

  friend embedded_serialization::ISerializedData<Data>;
};

std::array<uint8_t, 1024U> data_exchange{};

int main() {
  Data data{};
  embedded_serialization::serialization<Data, embedded_serialization::LittleEndian>(
      data, embedded_serialization::Span<uint8_t>(data_exchange.data(), data_exchange.size()));
  std::cout << "data: \n";
  data.dump();

  Data out{};
  embedded_serialization::deserialization<Data, embedded_serialization::LittleEndian>(
      embedded_serialization::Span<uint8_t>(data_exchange.data(), data_exchange.size()), out);
  std::cout << "out: \n";
  out.dump();
}