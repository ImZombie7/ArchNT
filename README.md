# ArchNT

**Arch Linux userspace on the Windows NT kernel. No WSL.**

ArchNT is an experimental Linux-compatibility project. The goal is to run real Linux/Arch userspace directly on Windows NT, without WSL, a Linux virtual machine, or a replacement kernel.

> Status: concept / early scaffolding. This does not run Linux binaries yet.

## The actual goal

ArchNT is not a themed Windows shell. It must provide enough Linux execution compatibility for Linux programs and, eventually, Arch Linux userspace and package workflows to run while Windows NT remains the host kernel.

High-level components:

1. **Linux executable support** — inspect and load ELF binaries, including architecture and ABI validation.
2. **Linux ABI compatibility** — implement Linux system-call semantics using supported Windows NT / Win32 primitives where possible.
3. **Process and thread model** — Linux process creation, exit status, signals, waits, file descriptors, and per-process state.
4. **Linux filesystem view** — Linux paths, permissions, metadata, symlinks, proc-like interfaces, and compatible file operations.
5. **Runtime and libraries** — load Linux ELF interpreters and shared libraries; support required memory mapping and TLS behavior.
6. **Arch userspace** — after the runtime is viable, investigate running Arch binaries and integrating package management.

See [ARCHITECTURE.md](ARCHITECTURE.md) for the staged plan and constraints.

## Non-goals

- Replacing or patching the Windows NT kernel.
- Pretending that a Windows shell is a Linux environment.
- Depending on WSL, a Linux VM, or Docker to provide the Linux runtime.
- Claiming compatibility before real Linux binaries pass tests.

## Current repository state

The current C/CMake files are throwaway scaffolding, not the compatibility layer. The existing archnt.exe prototype is a Windows command wrapper and cannot execute Linux ELF programs. Do not mistake it for a working Linux runtime.

## Development principles

- Start with a narrow, testable Linux ABI subset rather than claiming full compatibility.
- Document every syscall's supported behavior and known differences.
- Keep Windows-specific implementation details behind internal interfaces.
- Use reproducible tests and real Linux-built ELF fixtures.
- Never claim Arch package compatibility until the loader, runtime, filesystem, and required syscalls work together.

## Initial milestone

The first meaningful milestone is a Linux ELF inspection tool and compatibility-layer skeleton, followed by a deliberately small executable test. A package manager comes much later; pacman cannot compensate for a missing Linux runtime.
