# Repository Guidelines

## Project Structure & Module Organization

The camera application is organized by responsibility: `src/` contains implementations and `include/` contains public headers. Hardware capture, framebuffer output, touch input, JPEG encoding, HTTP streaming, and face analysis each have corresponding modules. Tests live in `tests/`; board support and driver work are under `drv/`. Configuration is in `config/`, with helper scripts in `scripts/` and the Haar cascade model in `assets/`. Generated binaries belong in `build/` or `build_arm/` and should not be committed.

## Build, Test, and Development Commands

- `make` builds the native `build/cam_server` executable.
- `make run` builds and starts the native application; camera and display hardware are needed for full operation.
- `make test` builds and runs the HTTP, video, V4L2, and face-analysis tests.
- `make menuconfig` edits `.config`; `make defconfig` restores the default configuration.
- `make -f Makefile.arm package_arm` cross-compiles and assembles `dist_arm/`; set `BUILDROOT_SDK` or `OPENCV_ARM` when dependencies are installed elsewhere.
- `make clean` removes the selected native build directory.

## Coding Style & Naming Conventions

Use C++14, four spaces for indentation, and the existing `-Wall -Wextra` warning set. Keep declarations in `include/*.hpp` and implementations in matching `src/*.cpp` files. Use descriptive `PascalCase` type names and `camelCase` functions and variables, following nearby code. Add concise English Doxygen comments to important functions; document public interfaces in headers rather than repeating comments in implementation files.

## Testing Guidelines

Add focused tests in `tests/` using the existing `*_test.cpp` naming pattern. Run `make test` after changes; hardware-dependent behavior should also be checked on the i.MX6ULL target when available. HTTP tests use local sockets, while V4L2 tests wrap system calls to exercise device handling without a camera.

## Commit & Pull Request Guidelines

Recent commits use short, descriptive subjects, sometimes in Chinese and sometimes English; no strict prefix convention is established. Keep each commit focused and state the behavior changed. PRs should describe the affected module, list validation performed, note hardware or configuration requirements, and update `cam.md` for technical changes or `record.md` when a design decision changes.
