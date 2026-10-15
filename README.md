# ArchNT

**A DIY Linux-inspired terminal workflow on the native Windows NT kernel.**

> Status: early prototype. Not a Linux distribution, Linux compatibility layer, or replacement kernel.

ArchNT aims to bring a tidy, keyboard-first, package-manager-inspired developer workflow to Windows without requiring WSL, a virtual machine, or Docker.

## Current prototype

- Native C executable for Windows
- Small interactive prompt
- Built-ins: `help`, `clear`/`cls`, `pwd`, `cd`, `exit`
- Other commands delegated to `cmd.exe`

## Build

Requirements: Windows, CMake 3.21+, and a C compiler such as MSVC or MinGW-w64.

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Run `build\Release\archnt.exe` with Visual Studio generators, or `build\archnt.exe` with a single-configuration generator such as MinGW Makefiles.

## Scope and safety

ArchNT currently wraps Windows command execution; it does **not** implement Linux syscalls, an ELF loader, or Linux package compatibility. Do not enter untrusted commands: delegated commands run with the same permissions as the current user.

## Roadmap

- [x] Establish native Windows C/CMake project
- [x] Minimal interactive prompt and built-ins
- [ ] Add command parsing and reliable argument handling
- [ ] Define a configuration and aliases format
- [ ] Add a package/workflow manager (design before implementation)
- [ ] Add tests and CI for supported Windows toolchains
