# Real-Time Multi-Sensor Fusion Pipeline

A real-time C++11 sensor fusion pipeline that aggregates readings from 4 concurrent temperature sensors and computes two pseudo-fusion values using a sliding window of configurable size.

## Problem

A device has 4 temperature sensors producing readings concurrently at different frequencies:

| Sensor | Frequency |
|--------|-----------|
| S0 | 96,000 Hz |
| S1 | 71,000 Hz |
| S2 | 69,900 Hz |
| S3 | 23,000 Hz |

Each reading is a value in `[0.0, 100.0]`. All readings are aggregated into a single stream and processed through a sliding window of size N (≥ 4096). Two fusion functions are computed continuously in real time.

## Fusion Functions

### Function 1 — Geometric Mean

$$F_1 = \exp\left(\frac{1}{N}\sum_{i=1}^{N}\ln(x_i)\right)$$

Computed in log-space for numerical stability. Returns `0.0` if any value in the window is zero.

### Function 2 — RMS of Pairwise Products

$$F_2 = \sqrt{\frac{\left(\sum x_i\right)^2 - \sum x_i^2}{N(N-1)}}$$

Uses the algebraic identity to reduce O(N²) pairwise computation to O(1) using running sums.

Both functions update in **O(1)** per new sample using three maintained running sums: `sum`, `sum_sq`, `sum_log`.

## Architecture

```
SensorSimulator x4  ──►  ThreadSafeQueue  ──►  SlidingWindow  ──►  F1 / F2  ──►  stdout
  (4 threads)              (mutex + cv)         (ring buffer)       (O(1))
                                  ▲
                            1 consumer thread
```

- **4 producer threads** — one per sensor, each sleeping between samples to simulate frequency
- **ThreadSafeQueue** — mutex + condition_variable, blocks consumer when empty
- **SlidingWindow** — circular buffer with O(1) push/evict and maintained running sums
- **Fusion1 / Fusion2** — stateless functions operating on the window's running sums
- **Aggregator** — owns all threads, orchestrates start/stop/join with timed shutdown

## Requirements

- C++11 compiler (GCC, Clang, MSVC)
- CMake ≥ 3.13
- No external dependencies — C++ standard library only

> **Note (Windows/MinGW):** Static linking is enabled automatically on MinGW builds to bundle all runtime libraries into the binary, preventing DLL version conflicts across different execution environments (CMD, PowerShell, Git Bash).

## Build Instructions

> If you received this as a zip/tarball, unzip it and run the build steps below from the project root.

### Linux / macOS

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

Clean without rebuild:
```bash
cmake --build . --target clean
```

Clean rebuild:
```bash
cmake --build . --clean-first
```

### Windows (PowerShell / CMD)

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

Clean without rebuild:
```powershell
cmake --build .\build --target clean
```

Clean rebuild:
```powershell
cmake --build .\build --clean-first
```

### Windows (Git Bash)

```bash
mkdir -p build && cd build
cmake .. -G "Unix Makefiles"
cmake --build .
```

## Running

```bash
# Default: window=4096, duration=10 seconds
./fusion_pipeline

# Custom window size, default duration
./fusion_pipeline 8192

# Custom window size and duration
./fusion_pipeline 8192 30
```

On Windows CMD/PowerShell:
```cmd
fusion_pipeline.exe 4096 10
```

### Arguments

| Argument | Default | Description |
|---|---|---|
| `window_size` | 4096 | Sliding window size (clamped to 4096 minimum) |
| `run_seconds` | 10 | How long to run before exiting cleanly |

### Sample Output

```
Starting fusion pipeline: window=4096  duration=10s
samples=10000  F1=36.91  F2=49.87
samples=20000  F1=37.25  F2=50.12
samples=30000  F1=36.78  F2=49.95
...
Aggregator stopped after 10 seconds.
```

`F1` (geometric mean) converges to ~36.8 and `F2` (pairwise RMS) converges to ~50.0 for uniform random input in `[0, 100]` — both match theoretical expectations.

## Project Structure

```
.
├── CMakeLists.txt
├── include/
│   ├── sample.h              # Core data unit (value, timestamp, sensor_id)
│   ├── thread_safe_queue.h   # Mutex-based producer-consumer queue
│   ├── sensor_simulator.h    # Per-thread sensor simulation
│   ├── sliding_window.h      # Circular buffer with O(1) running sums
│   ├── fusion1.h             # Geometric mean
│   ├── fusion2.h             # Pairwise RMS
│   └── aggregator.h          # Pipeline orchestrator
├── src/
│   ├── main.cpp
│   ├── thread_safe_queue.cpp
│   ├── sensor_simulator.cpp
│   ├── sliding_window.cpp
│   ├── fusion1.cpp
│   ├── fusion2.cpp
│   └── aggregator.cpp
└── docs/
```
