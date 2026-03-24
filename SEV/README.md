# Deflection: AMD SEV Module

This module introduces Virtual Machine-based TEE support to the Deflection framework, leveraging **AMD Secure Encrypted Virtualization (SEV)** technologies. 

Transitioning to a VM-based TEE allows Deflection to overcome the memory constraints (EPC limits) associated with traditional process-level enclaves, while providing a seamless "Lift and Shift" execution environment for unmodified C/C++ applications and containerized workloads.

## Architecture & Design

Unlike the process-level shielding in the SGX module, the SEV module enforces confidentiality through hardware-assisted main memory encryption. Each virtual machine is assigned a unique memory encryption key managed by the AMD Secure Processor (AMD-SP), isolating the guest VM from the hypervisor and other tenants.

## Prerequisites

To run the SEV module, the host environment must support AMD SEV functionalities:

* An **AMD EPYC™ processor** with SEV, SEV-ES, or SEV-SNP enabled in BIOS.
* A compatible Linux kernel with KVM and SEV host drivers enabled.
* OVMF (Open Virtual Machine Firmware) built with SEV support for guest boot.

## Quick Start

Deflection will move to VM-based TEE soon.
