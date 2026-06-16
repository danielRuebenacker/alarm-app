# Building and Testing
```
rm -rf build                            // optionally clear cached
cmake --build build && ./build/tests/run_project_tests
```

# More advances flags for testing
```
ctest -V                                                        // more info
./build/tests/run_project_tests --list-test-cases               // list cases
./build/tests/run_project_tests --test-case="specific"          // specific test case
```
