# test-YADRO
Build:
```bash
cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Release && cmake --build ./build --config Release && cd build
```
Run:
```bash
./task ./../test_files/unit_tests/1.txt
```
Run tests:
```bash
./tests
```