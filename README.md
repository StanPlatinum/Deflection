# Deflection: Verifying a program's privacy policy within a TEE environment

Deflection is a TEE shielding runtime (TEE middleware) that can run nearly unmodified C/C++ code securely. Other than Gramine, SCONE, Occlum, etc., it can protect code secrecy and data confidentiality at the same time.
It can be used in a TEE-based data capsule framework.

Deflection will move to VM-based TEE soon.

*** 

This project is derived from elf-respect (https://github.com/StanPlatinum/elf-respect) and its compiler (https://github.com/StanPlatinum/elf-respect-compiler).

Our paper: Practical and efficient in-enclave verification of Privacy Compliance has been accepted by DSN'21. This research paper highlights the effectiveness of verifying a program's privacy policy within a TEE environment.

```
@INPROCEEDINGS{deflection,
  author={Liu, Weijie and Wang, Wenhao and Chen, Hongbo and Wang, XiaoFeng and Lu, Yaosong and Chen, Kai and Wang, Xinyu and Shen, Qintao and Chen, Yi and Tang, Haixu},
  booktitle={2021 51st Annual IEEE/IFIP International Conference on Dependable Systems and Networks (DSN)}, 
  title={Practical and Efficient in-Enclave Verification of Privacy Compliance}, 
  year={2021},
  volume={},
  number={},
  pages={413-425},
  keywords={Privacy;Data privacy;Runtime;Software;Generators;Data models;Security;Intel SGX;Confidential Computing;Proof-Carrying Code;Enclave Shielding Runtime},
  doi={10.1109/DSN48987.2021.00052}}
```
Check the paper [here](https://arxiv.org/pdf/2007.10513).

***

Although SGX can provide strong isolation and integrity assurance, code privacy may raise some concerns when using it in cloud environments.

So in this project, we are aiming at a new problem: how service providers build a Practical TEE that can ensure the privacy of both data providers and code providers, i.e., a solution that enables a user to verify whether a remote service (such as MLaaS) has the properties of confidentiality (and integrity) without touching the binary/source code.

We also provide a compiling toolset (currently supporting C code) which is applicable in the confidential verification/attestation (for the code producer).

We combine both SGX and PCC to design a new verification framework that makes the TCB as small as possible. Since Intel SGX only has a limit regarding memory usage - a very limited area, up to 128M in size, that can be protected by the processor at one time, making the TCB small becomes significant.

Specifically, the proof is (partially) generated from the outside of the enclave during the compiling and can be verified at runtime inside. Of course, the inside checker can cooperate with the outside compiler to make the checker as lightweight as possible.

Moreover, our method provides a new paradigm for PCC to use a TEE (such as Intel SGX) as an execution environment for trusted verification. We remove the VCGen (the compiler) from the trusted computing base.

## Quick start

Make sure,

you have an 'SGX machine' (a machine whose CPU supports Intel SGX, with SGX Driver installed, and with SGXSDK installed in /opt/intel);

you have at least 10G memory (Swap could be included) and 100G disk space before having a try. It means probably the most suitable place for having fun with Deflection is a satisfiable Linux server machine.

```
./install.sh
```

### A hello world example

```
cd dynamic-loader-checker
make
./app
```

### Other examples

More sample applications and benchmarks (e.g., nbench) can be found at `loader/target-program` or `dynamic-loader-checker/target-program`.
