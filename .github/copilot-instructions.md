# ActTest: Stress Testing Harness

## Project Overview

ActTest is a **multi-language stress testing framework** that validates code solutions through comprehensive input testing. The architecture consists of:

- **`codice_da_testare.cpp`**: The target solution code to be tested (user-written)
- **`tester.cpp`**: The test engine that compiles/interprets the target, then runs it against a stress test dataset
- **`Dockerfile`**: Containerized environment with GCC, Python3, and Node.js for reproducible testing across languages

## Core Workflow & Architecture

The testing flow is:
1. User submits solution code (`.cpp`, `.py`, or `.js`)
2. `tester.cpp` detects the language and compiles/prepares it (only C++ requires compilation)
3. Harness pipes a series of test inputs to the program via stdin
4. If any input causes non-zero exit code → test fails with that input logged
5. Container's `ENTRYPOINT` automatically runs the engine on startup

**Key Design Pattern**: The tester uses `std::system()` to shell out to either compilation (`g++`) or direct execution (`python3`/`node`). Input is passed via pipe: `echo {input} | {cmd}`.

## Multi-Language Support

Each language is handled differently:
- **C++**: Compiled to `./user_bin` before testing (requires `g++`)
- **Python3**: Directly interpreted with `python3 {target}`
- **Node.js**: Directly interpreted with `node {target}`

All programs receive input on stdin and are expected to exit with code 0 on success, non-zero on error.

## Test Dataset Strategy

`tester.cpp` generates 57 test cases combining:
- **Edge cases**: `{0, 1, -1, 100, -100, 2147483647, -2147483648}`
- **Random values**: 50 random integers in range [-10000, 10000]

This catches boundary conditions, division-by-zero, integer overflow, and general crashes. Output is redirected to `/dev/null` - only exit codes matter.

## Running & Debugging

**Build Docker image:**
```bash
docker build -t acttest .
```

**Run against a solution:**
```bash
docker run acttest codice_da_testare.cpp
# or
docker run acttest solution.py
docker run acttest solution.js
```

**Local testing (without Docker):**
```bash
g++ -O3 tester.cpp -o engine
./engine codice_da_testare.cpp
```

## Common Issues & Fixes

- **"CRASH! Input that broke the code: 0"**: Division by zero. Add `if (n == 0) return 1;` checks
- **Compilation fails**: Ensure `g++` is installed; check C++ syntax
- **Python import errors**: Verify dependencies; test locally first with `python3 solution.py`
- **Output to stderr pollutes logs**: Redirect with `2>&1` (already done in tester's shell command)

## Extending the Test Suite

To add more edge cases:
1. Modify the `inputs` vector in `tester.cpp` to include new test values
2. Recompile: `g++ -O3 tester.cpp -o /engine` (inside Dockerfile or manually)
3. Rebuild Docker image: `docker build -t acttest .`

The harness is intentionally stress-focused—it finds breaking inputs fast rather than verifying correctness.
