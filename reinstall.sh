#!/bin/bash
# Only can be used when musl-libc already being built!

Curr_Path=`pwd`
echo "Current Path: "$Curr_Path

Bash_Dir=$(cd "$(dirname "$0")";pwd)
echo "Bash file Path: "$Bash_Dir

if [ "$Curr_Path" != "$Bash_Dir" ]
then
        echo "Wrong install path! Please make sure you are in a right elf-respect repo!"
        exit 1
fi

cd ..
Install_Path=`pwd`
echo "Re-install Path: "$Install_Path

if [ ! -d "./linux-sgx" ]
then
        echo "No SGXSDK!"
        exit 1
fi
SGXSDK_Source_Path=`pwd`"/linux-sgx"

if [ ! -d "./elfutils" ]
then
        echo "No Libelf!"
        exit 1
fi
Libelf_Path=`pwd`"/elfutils"

if [ ! -d "./capstone" ]
then
        echo "No Capstone!"
        exit 1
fi
Capstone_Path=`pwd`"/capstone"

if [ ! -d "./llvm-mc" ]
then
	echo "No llvm-mc!"
	exit 1
fi
LLVM_MC_Path=`pwd`"/llvm-mc"

if [ ! -d "./proofGen" ]
then
        echo "No proofGen!"
        exit 1
fi
ProofGen_Path=`pwd`"/proofGen"

cd cat-sgx

echo "Configuring loader..."
cd loader
echo "Generating new Makefile header..."
rm -f Makefile
echo "SGX_SRC_PATH = "$SGXSDK_Source_Path > Makefile_header4checker
echo "ELFUTILS_PATH = "$Libelf_Path >> Makefile_header4checker
echo "CAPSTONE_PATH = "$Capstone_Path >> Makefile_header4checker
cat Makefile_header4checker Makefile_template4checker > Makefile
make clean
rm Makefile_header4checker
cd target-program
echo "Generating new Makefile header..."
rm -f Makefile
CC4AS=$LLVM_MC_Path"/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig"
echo "CC = "$CC4AS > Makefile_header4target
cat Makefile_header4target Makefile_template4target > Makefile
rm Makefile_header4target
make clean

cd mbedtls
echo "Generating new Makefile header..."
rm -f Makefile
echo "LLVM_MC_PATH = "$LLVM_MC_Path > Makefile_header4https
cat Makefile_header4https Makefile_template4https > Makefile
rm Makefile_header4https
cd ../../..


echo "Configuring dynamic-loader-checker..."
cd dynamic-loader-checker
echo "Generating new Makefile header..."
rm -f Makefile
echo "SGX_SRC_PATH = "$SGXSDK_Source_Path > Makefile_header4checker
echo "ELFUTILS_PATH = "$Libelf_Path >> Makefile_header4checker
echo "CAPSTONE_PATH = "$Capstone_Path >> Makefile_header4checker
cat Makefile_header4checker Makefile_template4checker > Makefile
make clean
rm Makefile_header4checker
cd target-program
echo "Generating new Makefile header..."
rm -f Makefile
echo "LLVM_PATH = "$ProofGen_Path > Makefile_header4target
echo "Our_AS_Path = "$LLVM_MC_Path >> Makefile_header4target
cat Makefile_header4target Makefile_template4target > Makefile
rm Makefile_header4target
make clean

cd mbedtls
echo "Generating new Makefile header..."
rm -f Makefile
echo "LLVM_MC_PATH = "$LLVM_MC_Path > Makefile_header4https
echo "LLVM_PATH = "$ProofGen_Path >> Makefile_header4https
cat Makefile_header4https Makefile_template4https > Makefile
rm Makefile_header4https
cd ../..

cd sc-resil-tg
echo "Generating new Makefile header..."
rm -f Makefile
echo "LLVM_PATH = "$ProofGen_Path > Makefile_header4target
echo "Our_AS_Path = "$LLVM_MC_Path >> Makefile_header4target
echo "SGX_SRC_PATH = "$SGXSDK_Source_Path >> Makefile_header4target
cat Makefile_header4target Makefile_template4target > Makefile
rm Makefile_header4target
make clean

cd mbedtls
echo "Generating new Makefile header..."
rm -f Makefile
echo "LLVM_MC_PATH = "$LLVM_MC_Path > Makefile_header4https
echo "LLVM_PATH = "$ProofGen_Path >> Makefile_header4https
cat Makefile_header4https Makefile_template4https > Makefile
rm Makefile_header4https
cd ../../..

echo "Re-installed!"
