# eflection: Intel SGX Module

This module contains the Intel SGX implementation of the Deflection shielding runtime. It focuses on providing process-level isolation while minimizing the Trusted Computing Base (TCB) through cryptographic proof verification.

This implementation is derived from [elf-respect](https://github.com/StanPlatinum/elf-respect) and its [compiler](https://github.com/StanPlatinum/elf-respect-compiler).

## Architecture & Design

We combine Intel SGX and Proof-Carrying Code (PCC) to design a verification framework that strictly limits the TCB. Since traditional Intel SGX architectures face memory usage constraints (Enclave Page Cache - EPC limit, typically up to 128MB protected by the processor concurrently), maintaining a minimal TCB is of significant importance.

Specifically, the verification proof is partially generated outside the enclave during the compilation phase and is rigorously verified at runtime inside the enclave. The internal checker cooperates with the external compiler to ensure the checker remains as lightweight as possible.

This method introduces a new paradigm for PCC, utilizing a TEE as an execution environment for trusted verification, effectively removing the VCGen (the compiler) from the trusted computing base.

## Prerequisites

Before deploying the SGX module, ensure your environment meets the following requirements:

* An **SGX-enabled machine** (CPU supports Intel SGX).
* Intel SGX Driver installed.
* Intel SGX SDK installed (default path: `/opt/intel`).
* At least **10GB memory** (Swap included) and **100GB disk space**. A satisfiable Linux server machine is highly recommended.

## Quick Start

Navigate to this directory and run the installation script:

```bash
./install.sh
```

### A Hello World Example

To compile and run the fundamental dynamic loader checker:

```bash
cd dynamic-loader-checker
make
./app
```

### Advanced Examples and Benchmarks

Further sample applications and performance benchmarks (e.g., nbench) are located in the following directories:

- `loader/target-program`
- `dynamic-loader-checker/target-program`
