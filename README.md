# embedded-serialization

![GitHub Workflow Status](https://img.shields.io/github/workflow/status/HerrCai0907/embedded-serialization/Release%20Test/main?label=build%20and%20test)
![GitHub last commit](https://img.shields.io/github/last-commit/HerrCai0907/embedded-serialization/main)
![GitHub License](https://img.shields.io/github/license/HerrCai0907/embedded-serialization)
![GitHub repo size](https://img.shields.io/github/repo-size/HerrCai0907/embedded-serialization)

serialization library for embedded device (header only)

## How to use

### API

```c++

template <class T> class DeserializedType {
public:
  using type = typename detail::SerializeImpl<UnknownEndian, T>::DeserializedType;
};
template <class T> u32 get_size(T const &data) noexcept;
template <class T, class Endian = UnknownEndian> u32 serialization(T const &data, Span<u8> const data_area) noexcept ;
template <class T, class Endian = UnknownEndian>
u32 deserialization(Span<u8> const data_area, typename DeserializedType<T>::type &out) noexcept;
```

### example

[demo code](example/main.cpp)

### cmake flag

- EMBEDDED_SERIALIZATION_INTEGRATION: enable integration mode, will disable unittest and example
- ENABLE_COV: enable code coverage analysis

## serialized bytes

| type          | format                           |
| ------------- | -------------------------------- |
| u8            | 1 byte                           |
| u16           | 2 bytes little endian data       |
| u32           | 4 bytes little endian data       |
| u64           | 8 bytes little endian data       |
| static array  | T ...                            |
| dynamic array | size<sup>1</sup> + T ...         |
| other         | compile time error "not support" |

1. The size is automatically adjusted according to the maximum number of array elements

## Build and Test

```bash
cmake -S . -B build -DENABLE_COV=on
cmake --build build --parallel
ctest --test-dir build
```

or

```bash
bazel test //tests
bazel coverage //tests && genhtml --branch-coverage "$(bazel info output_path)/_coverage/_coverage_report.dat" -o bazel-cov
```
