# CAT-SGX

Although SGX can provide strong isolation and integrity assurance, code privacy may raise some concerns when using it in cloud environments.

So in this project, we are aiming at a new problem: how service providers build a Practical TEE that can ensure the privacy of both data providers and code providers, i.e., a solution that enables a user to verify whether a remote service (such as MLasS) has the properties of confidentiality (and integrity) without touching the binary/source code.

We also provide a compiling toolset (currently supporting C code) which is applicable in the confidential verification/attestation (for the code producer).

We combine both SGX and PCC to design a new verification framework that makes the TCB as small as possible. Since Intel SGX only has a limit regarding memory usage - a very limited area, up to 128M in size, that can be protected by the processor at one time, making the TCB small becomes significant.

Specifically, the proof is (partially) generated from the outside of the enclave during the compiling and can be verified at runtime inside. Of course, the inside checker can cooperate with the outside compiler to make the checker as lightweight as possible.

Moreover, our method provides a new paradigm for PCC to use a TEE (such as Intel SGX) as an execution environment for trusted verification. We remove the VCGen (the compiler) from the trusted computing base.

## Quick start

Make sure,

you have an 'SGX machine' (a machine whose CPU supports Intel SGX, with SGX Driver installed, and with SGXSDK installed in /opt/intel);

you have at least 10G memory (Swap could be included) and 100G disk space before having a try, which means the most suitable place for having fun with cat-sgx is a satisfiable Linux server machine.

The installation paths of other dependencies are in the same directory as cat-sgx by default.

```
./install.sh
```
