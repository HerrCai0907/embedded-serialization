# embedded-serialization

serialization library for embedded device (header only)

## serialized bytes

| type          | format                           |
| ------------- | -------------------------------- |
| u32           | 4 bytes little endian data       |
| static array  | T ...                            |
| dynamic array | size:u32 + T ...                 |
| other         | compile time error "not support" |

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
