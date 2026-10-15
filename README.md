# ArchNT

**Arch Linux on Windows NT, without WSL, a VM, or a hypervisor.**

ArchNT is an experimental project exploring a bridge between Arch Linux and the Windows NT kernel. The intended experience is to boot into an Arch environment and use it like an operating system, while Windows NT remains the actual host kernel.

> **Status: concept and early scaffolding.** The current prototype does not boot Arch or run Linux binaries. The architecture described here is the project goal, not a claim of working functionality.

## The core idea

ArchNT is not a Linux-themed Windows shell, nor is it intended to be a conventional virtual machine.

The intended design has three major parts:

1. **ArchNT launcher and boot menu** — a Windows executable opens a firmware/BIOS-like interface where the user chooses a boot source and configures the environment.
2. **ArchNT kernel bridge/runtime** — Arch must not depend on its normal Linux kernel. ArchNT is intended to provide the Linux-facing kernel functionality and translate or implement the required behavior using Windows NT facilities.
3. **Arch Linux userspace** — run real Arch userspace programs, shells, libraries, tools, and eventually package-management workflows through the bridge.

The launcher is firmware-like in its interface and purpose; it does not literally replace the motherboard firmware while Windows is already running.

## Boot sources

The boot menu is intended to support selectable sources such as:

- **Virtual disk images (.vhdx)** containing a persistent Arch installation.
- **Physical storage devices or partitions** containing an Arch installation, subject to safe access and filesystem support.
- **Arch Linux ISO images** as an optional source, potentially for installation, recovery, or reuse of compatible userspace files.

An ISO cannot simply follow its ordinary boot sequence unchanged: a standard Arch ISO expects to boot a Linux kernel. ArchNT must instead provide the required kernel-facing behavior itself. Accessing an ISO's files does not automatically make its initramfs or live environment compatible.

A persistent .vhdx is a practical candidate for an initial prototype, but this is a proposed development order, not a finalized limitation on boot sources.

## Host integration

ArchNT is intended to connect selected Windows resources to the Arch environment:

- Windows filesystems and drives may be exposed under Linux paths such as /mnt.
- /dev should expose Linux-compatible device interfaces supplied or mediated by ArchNT, not automatically grant unrestricted access to every Windows device.
- The bridge must implement the Linux-facing behavior expected by programs, including executable loading, system calls, processes, memory behavior, filesystem operations, and runtime/library support.

This is much more than mapping Linux syscall names to similarly named Windows APIs. Linux and Windows have different semantics, so compatibility must be implemented and tested.

## Isolation and interoperability

Arch and Windows should have distinct process and memory environments, with deliberate bridges for the resources they are meant to share.

| Capability | Intended policy |
|---|---|
| Read and modify files across the boundary | Allowed on both sides, subject to the access and filesystem mechanisms provided |
| Process management | Each environment gets its own normal process behavior. Cross-boundary process manipulation is not a goal or a user-facing feature |
| Memory access | Arch must not access Windows process memory. Windows, as the host kernel, inherently has the ability to inspect or control its own hosted ArchNT process |
| System services and privileged devices | Arch must not gain control of Windows services or privileged host devices through the bridge. These resources are not to be exposed as unrestricted controls to either environment through ArchNT |
| Cross-boundary program execution | A possible feature, initiated explicitly and governed by clear rules |
| Inter-environment communication (IPC) | Allowed through defined, authorized interfaces |

File sharing is an intentional exception to private filesystem separation. It does not imply raw-disk access or unrestricted device control. Likewise, a Windows program launched from Arch would execute as a Windows program under Windows security rules; it would not become a Linux process.

**Security limit:** because ArchNT is intended to run on the Windows NT kernel without a VM or hypervisor, it cannot promise protection from a fully privileged Windows kernel. The intended boundary is enforced by ArchNT's interfaces and permissions, not by pretending the host kernel has lost its authority over its own process.

## Non-goals and constraints

- Replacing or patching the Windows NT kernel.
- Requiring WSL, a Linux VM, a hypervisor, or Docker to provide the Linux runtime.
- Treating a Windows shell or command wrapper as a real Arch environment.
- Granting Arch unrestricted authority over Windows services, privileged devices, or Windows process memory.
- Claiming compatibility before real Linux binaries and relevant behaviors pass tests.

## Current repository state

The existing C/CMake files are early scaffolding, not the kernel bridge. The current archnt.exe prototype is a Windows command wrapper; it cannot boot Arch or execute Linux ELF programs.

## Development principles

- Turn the concept into small, testable milestones.
- Use real Linux-built ELF fixtures and reproducible tests.
- Document supported behavior, limitations, and security boundaries.
- Keep Windows-specific implementation details behind internal interfaces.
- Treat boot-source support, filesystem integration, isolation, and cross-boundary features as explicit engineering work.
- Never claim that Arch or pacman works until the required runtime, ABI behavior, filesystems, and dependencies actually work.

## The central engineering challenge

A normal Arch installation expects Linux kernel services. ArchNT's central challenge is to supply the kernel-facing behavior Arch userspace needs while Windows NT remains the real host kernel, without using a conventional VM, WSL, or a replacement Linux kernel. This is the defining research and implementation problem for the project.
