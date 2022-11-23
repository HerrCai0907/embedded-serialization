#include "serialize.h"
#include "serialize_interface.h"
#include <array>
#include <cstdint>
#include <cstring>
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
    return embedded_serialization::get_size(VecSpan{vec_.data(), static_cast<uint32_t>(vec_.size())}) +
           embedded_serialization::get_size(ArrSpan{arr_.data(), static_cast<uint32_t>(arr_.size())}) +
           embedded_serialization::get_size(state_);
  }
  uint32_t serializeImpl(embedded_serialization::Span<uint8_t> const data_area) const noexcept {
    uint32_t offset = 0;
    offset +=
        embedded_serialization::serialization(VecSpan{vec_.data(), static_cast<uint32_t>(vec_.size())}, data_area);
    offset += embedded_serialization::serialization(ArrSpan{arr_.data(), static_cast<uint32_t>(arr_.size())},
                                                    data_area.subspan(offset));
    offset += embedded_serialization::serialization(state_, data_area.subspan(offset));
    return offset;
  }
  uint32_t deserializeImpl(embedded_serialization::Span<const uint8_t> const data_area) noexcept {
    uint32_t offset = 0;

    embedded_serialization::DeserializedType<VecSpan>::type vecSpan{};
    offset += embedded_serialization::deserialization<VecSpan>(data_area, vecSpan);
    vec_.resize(vecSpan.size());
    std::memcpy(vec_.data(), vecSpan.data(), vecSpan.size());

    embedded_serialization::DeserializedType<ArrSpan>::type arrSpan{};
    offset += embedded_serialization::deserialization<ArrSpan>(data_area.subspan(offset), arrSpan);
    std::memcpy(arr_.data(), arrSpan.data(), arrSpan.size());

    offset += embedded_serialization::deserialization<State>(data_area.subspan(offset), state_);
    return offset;
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