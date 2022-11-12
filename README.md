# embedded-serialization

serialization library for embedded device (header only)

## serialized bytes

| type  | format                           |
| ----- | -------------------------------- |
| u32   | 4 bytes little endian data       |
| other | compile time error "not support" |

## Build

```bash
cmake -S . -B build
cmake --build build --parallel
ctest --test-dir build
```
