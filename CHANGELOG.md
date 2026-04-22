# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- New view for grid

## [1.0.5] - 2026-04-22

### Fixed

- Removed, temporarily, MacOS x86_64 in CI, please build on your machine
- Fixed size of Grid on click on "New" button
- Fixed start/stop button

## [1.0.4] - 2026-04-22

### Fixed

- Fixed build CI for GitHub for MSVC, hoping it will work this time

## [1.0.3] - 2026-04-22

### Fixed

- Fixed build CI for GitHub for MSVC

## [1.0.2] - 2026-04-22

### Fixed

- Removed a blocking macro expansion

## [1.0.1] - 2026-04-22

### Fixed

- Encoding of a source file (src/I18n.cpp)
- Replacing logs used with std::cerr by LOG_ERROR in main source file (src/main.cpp)

## [1.0.0] - 2026-04-22

### Added
- First public version : basic implementation of Game of Life
- Save/load grid from file
- 10x10 cases grid
- GitHub build workflow.

[v1.0.0]: https://github.com/DostLeFan/GameOfLife/releases/tag/v1.0.0