# embedded-serialization

serialization library for embedded device (header only)

## serialized bytes

| type          | format                           |
| ------------- | -------------------------------- |
| u8            | 1 bytes little endian data       |
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
cmake -S . -B build -DENABLE_COV=on && cmake --build build --parallel && ctest --test-dir build
```
