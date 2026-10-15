# ArchNT architecture and staged plan

## Objective

Run actual Arch Linux userspace programs on the Windows NT kernel without WSL, a Linux VM, or a replacement kernel.

This is a Linux compatibility environment, not a conventional Linux distribution booting its own kernel. Windows NT remains the host kernel, so ArchNT must translate Linux user-visible behavior into host mechanisms and implement behavior that has no direct host equivalent.

## Hard technical problems

- **ELF loading:** parse ELF headers and program headers, validate architecture/ABI, map segments, apply relocations as needed, and load the ELF interpreter and shared objects.
- **System calls:** implement Linux syscall numbers, arguments, error conventions, memory behavior, and observable semantics. Calling a similarly named Win32 function is not automatically equivalent.
- **Memory:** support Linux mmap, munmap, and mprotect behavior, page alignment, shared mappings, and executable-memory rules.
- **Processes and threads:** emulate Linux process/thread semantics, file descriptors, waits, exit status, signals, and thread-local storage.
- **Filesystem:** map a Linux-style root and path model, permissions, ownership metadata, symlinks, special files, and mount-like behavior.
- **Runtime:** support the target architecture's Linux ABI, dynamic linker, shared libraries, TLS, and required runtime interfaces.
- **Compatibility surface:** Arch package contents vary widely; many programs require syscalls, kernel interfaces, services, or device behavior outside an initial minimal subset.

## Proposed milestones

### M0 — Repository and target definition
- [ ] State supported host Windows versions and CPU architecture.
- [ ] Select the first Linux target ABI, likely x86-64 Linux on x86-64 Windows.
- [ ] Establish coding standards, threat model, test strategy, and build CI.
- [ ] Keep the current command-wrapper prototype clearly marked as disposable scaffolding.

### M1 — ELF inspection
- [ ] Build a native Windows C tool that safely parses ELF64 headers and program headers.
- [ ] Reject malformed, unsupported, and out-of-bounds inputs without crashing.
- [ ] Report architecture, entry point, segments, interpreter, and dependencies where discoverable.
- [ ] Add test fixtures produced by a Linux toolchain and malformed-file tests.

### M2 — Runtime research and proof of concept
- [ ] Choose an execution strategy: a user-mode compatibility runtime versus another explicitly justified design.
- [ ] Define the process boundary and Windows API usage.
- [ ] Demonstrate a tiny statically linked ELF with a very small, documented syscall subset.
- [ ] Add syscall tracing and deterministic tests.

### M3 — Minimal Linux process environment
- [ ] File descriptors and basic file I/O.
- [ ] Process exit, arguments/environment, clock, memory mapping, and basic process creation as scope permits.
- [ ] A coherent Linux-style filesystem root and error semantics.
- [ ] Run a small suite of Linux-built tests without WSL or a VM.

### M4 — Dynamic executables
- [ ] ELF interpreter and shared-library loading.
- [ ] Required relocations, TLS, thread behavior, and additional syscalls.
- [ ] Test a dynamically linked program against a declared libc/runtime target.

### M5 — Arch userspace experiments
- [ ] Establish a Linux-compatible root filesystem layout.
- [ ] Identify and implement syscall/kernel-interface requirements of selected Arch packages.
- [ ] Investigate pacman only when the underlying runtime can execute its dependencies.
- [ ] Publish a compatibility matrix; do not imply general Arch compatibility from a few successful binaries.

## Architectural rule

Do not expose the project as a shell first. The core should be separated into at least:

- elf/: safe ELF parsing and loading primitives
- abi/: Linux syscall dispatch and Linux error semantics
- process/: process/thread state and lifecycle
- memory/: virtual-memory operations
- fs/: Linux path and file-descriptor model
- host/win32/: Windows-specific implementations
- tests/: ELF fixtures, syscall tests, malformed-input tests, and compatibility tests

These are proposed boundaries, not a claim that those components already exist.

## Important constraints

A user-mode compatibility layer cannot transparently reproduce every Linux kernel feature just by wrapping Windows APIs. Some Linux semantics have no direct equivalent, and a practical implementation will require deliberate emulation, documented gaps, and substantial testing. A broad target such as arbitrary Arch Linux packages is a long-term goal, not an initial milestone.
