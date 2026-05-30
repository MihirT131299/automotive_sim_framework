# Automotive Digital Twin — Simulation Framework

A modular C++ simulation framework for vehicle systems research and simulation infrastructure experimentation. The framework is designed around the principles of deterministic execution, uniform model interfaces, and clean separation between physics, signal routing, and orchestration.

## Project Goals

- Build a simulation platform that mirrors the architectural patterns used in industry automotive digital twin systems
- Support modular subsystem development where individual models can be replaced without modifying the core engine
- Enable distributed co-simulation, FMU integration, and scalable experiment management as the framework matures

## Current State — Chapters 1, 2, and 3 Complete

### Chapter 1 — Simulation Core
- CMake build system with C++17, external dependency management via FetchContent
- Deterministic run loop using an integer step counter (`t_current = step * dt`) to eliminate floating point accumulation
- `Logger` class — encapsulated CSV output with defensive file handle management
- `Config` class — TOML config parsing with nested map storage, supports doubles, strings, and string arrays
- CLI interface — `--config` flag with full argument validation and error reporting

### Chapter 2 — Modular Signal Bus Architecture
- `SignalBus` — central double-valued signal registry with writer ownership enforcement. One writer per signal, enforced at startup. Supports default signal initialization for open-loop model testing.
- `BusConnector` — per-model translation layer between internal signal names and global bus names. Models write and read using their own internal names; the connector handles name translation transparently.
- `Engine` — orchestrates startup, mapping file loading, bus construction, model wiring, and the simulation loop. Reads model participation list from config at runtime.
- `EV_Model` refactored — physics model now reads inputs from and writes outputs to the bus through its connector. Parameters still loaded directly from config at startup (static config vs dynamic signals).
- Mapping files (`interfaces/*.mapping.toml`) — runtime artifacts defining each model's interface contract: what it writes, what it reads, name translations, and default values for open-loop testing.

### Chapter 3 — Verification Infrastructure
- Framework source files extracted into `adt_core` static library — both the simulation executable and test executable link against it. New source files added in one place only.
- Google Test integrated via CMake FetchContent, wired as a separate `run_tests` target
- 24 unit and integration tests across four test files, all passing:
  - `test_signal_bus.cpp` — 10 tests covering registration, ownership enforcement, read/write, and graceful failure handling
  - `test_config.cpp` — 7 tests covering doubles, arrays, missing keys, and malformed files
  - `test_bus_connector.cpp` — 6 tests covering name translation, ownership enforcement, and independent signal mappings
  - `test_regression.cpp` — 1 bit-exact golden reference regression test for the full EV model simulation
- CI pipeline via GitHub Actions — triggers on every pull request against `main`, builds on Ubuntu, runs full test suite
- Branch protection on `main` — PRs cannot merge unless all CI checks pass
- CI dashboard hosted on GitHub Pages — updates on every CI run, shows per-suite pass/fail status and failure messages

## Architecture

```
config.toml
    └── [engine] models list
    └── [simulation_loop_settings]
    └── [vehicle_params]

interfaces/ev_model.mapping.toml
    └── [writes] velocity → ev.velocity
    └── [reads]  throttle → sim.throttle (default: open-loop value)

Engine (startup)
    ├── reads config, loads mapping files
    ├── initializes SignalBus with defaults
    ├── builds BusConnector per model
    └── wires connectors to models

Simulation Loop (per timestep)
    ├── EV_Model reads throttle from bus via connector
    ├── EV_Model computes velocity, writes to bus via connector
    └── Logger reads ev.velocity from bus, writes to CSV
```

## Project Structure

```
automotive_digital_twin/
├── CMakeLists.txt
├── config.toml                         # simulation configuration
├── interfaces/
│   └── ev_model.mapping.toml           # EV model signal interface definition
├── include/
│   ├── signal_bus.h
│   ├── bus_connector.h
│   ├── engine.h
│   ├── ev_model.h
│   ├── config.h
│   └── logger.h
└── src/
    ├── main.cpp
    ├── engine.cpp
    ├── signal_bus.cpp
    ├── bus_connector.cpp
    ├── ev_model.cpp
    ├── config.cpp
    └── logger.cpp
```

## Build and Run

From the project root:

```bash
mkdir build
cd build
cmake ..
cmake --build .
.\Debug\AutomotiveDigitalTwin.exe --config "../config.toml"
```

Output is written to `build/simulation_output.csv`.

## Configuration

`config.toml` controls all simulation parameters:

```toml
[engine]
models = ["ev_model"]           # models participating in this run

[simulation_loop_settings]
t_start = 0.0
t_end = 13.0
dt = 0.001

[vehicle_params]
mass = 1800.0                   # kg
max_force = 5000.0              # N
drag_coefficient = 0.5          # combined 0.5 * Cd * A * rho (kg/m)
```

## Adding a New Model

1. Implement the model class in `include/` and `src/`
2. Create `interfaces/<model_name>.mapping.toml` defining its signal interface
3. Add the model name to `[engine] models` in `config.toml`
4. Add the source file to `add_executable` in `CMakeLists.txt`
5. Wire the model in `Engine::run()` following the existing EV model pattern

## Design Principles

- **Architecture before fidelity** — structural scaffolding takes priority over physics accuracy
- **Determinism by design** — integer step counter eliminates floating point drift across timesteps
- **Uniform interface** — native and future external models connect through the same mapping layer
- **Static vs dynamic separation** — model parameters come from config at startup; runtime signals flow through the bus each timestep
- **Open-loop testability** — every signal has a default value in the mapping file, allowing any model to run without a connected source

## Roadmap

Planned chapters ahead:

- **Chapter 3** — ~~Verification infrastructure: unit tests, regression tests, CI pipeline~~ **Complete**
- **Chapter 4** — Calibration and parameter sweep tooling
- **Chapter 5** — Performance profiling and batch experiment management
- **Chapter 6** — FMU integration and external model interoperability
- **Chapter 7** — Distributed simulation with shared memory IPC
- **Chapters 8–12** — Progressive full vehicle buildup
- **Chapters 13–16** — Fault modeling, visualization, platform polish, capstone demo