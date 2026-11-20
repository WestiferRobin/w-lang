# Hydra Compiler

**A native Python compiler written in C++20.**

*Write Python. Compile Native. Run Anywhere.*

## Overview

Hydra Compiler is an experimental compiler project aiming to transform standard Python applications into native machine code for multiple architectures, beginning with x86-64 and ARM64.

The long-term goal is to make Python a practical language for native applications, systems programming, robotics, scientific computing, and high-performance software without requiring developers to rewrite their applications in another language.

## Core Goals

1. **Native compilation** — Compile Python source into native machine code and executable applications.
2. **Python compatibility** — Preserve standard Python behavior and support the existing Python package ecosystem.
3. **Cross-platform compilation** — Target multiple CPU architectures and operating systems.
4. **Performance** — Investigate optimization, native memory representations, and multicore execution.
5. **Systems capabilities** — Explore optional low-level memory and hardware interfaces using Python-compatible syntax.
6. **Portable execution** — Investigate optional bytecode and JIT compilation alongside ahead-of-time compilation.

## Architecture

Hydra is designed around a Python frontend, semantic analysis, an intermediate representation, optimization passes, native code generation, and a compatibility runtime.

The compiler is implemented in modern C++.

## Project Status

**Status: Research and architectural design.**

Hydra is not yet an implemented Python compiler. The features described above are project goals, not currently supported functionality.

The first milestone is compiling a small, defined subset of standard Python into a working native executable.

## Origins

Hydra is the successor in spirit to W-lang, a personal compiler and programming-language project originally built from scratch to explore C++, parsing, abstract syntax trees, bytecode generation, and virtual-machine execution.

W-lang remains preserved as an independent historical project.

Hydra builds on those lessons while pursuing a different goal: native compilation of Python.