#!/bin/bash

echo "Welcome to Deflection/CAT-SGX/S(gx)elf-Respect!"

Curr_Path=`pwd`
echo "Current Path: "$Curr_Path

Bash_Dir=$(cd "$(dirname "$0")";pwd)
echo "Bash file Path: "$Bash_Dir

if [ "$Curr_Path" != "$Bash_Dir" ]
then
	echo "Wrong install path! Please make sure you are in a right repo!"
	exit 1
fi

cd ..
Install_Path=`pwd`
echo "Install Path: "$Install_Path


if [ ! -d "./linux-sgx-cat" ]
then
	echo "Building SGXSDK..."
	git clone https://github.com/StanPlatinum/linux-sgx-cat.git
	cd linux-sgx-cat
	SGXSDK_Source_Path=`pwd`
	cd ..
else
	echo "SGXSDK already exists!"
fi
SGXSDK_Source_Path=`pwd`"/linux-sgx-cat"

if [ ! -d "./elfutils" ]
then
	echo "Building Libelf..."
	git clone https://github.com/StanPlatinum/elfutils.git
	cd elfutils
	Libelf_Path=`pwd`
	autoheader
	aclocal -I m4
	autoconf
	automake -a -c
	./configure
	make
	if [ $? -ne 0 ]
	then
		echo "Libelf install failed!"
	else
		echo "Libelf install succeed!"
	fi
	cd ..
else
	echo "Libelf already exists!"
fi
Libelf_Path=`pwd`"/elfutils"
echo "Libelf Path: "$Libelf_Path


if [ ! -d "./capstone" ]
then
	echo "Building Capstone..."
	git clone https://github.com/StanPlatinum/capstone.git
	cd capstone
	Capstone_Path=`pwd`
	echo "Capstone Path: "$Capstone_Path
	mkdir build
	mkdir install-x86
	cd build
	cmake .. -DCMAKE_INSTALL_PREFIX=../install-x86 -DCAPSTONE_ARCHITECTURE_DEFAULT=OFF -DCAPSTONE_X86_SUPPORT=1
	make
	if [ $? -ne 0 ]
	then
		echo "Capstone install failed!"
	else
		echo "Capstone install succeed!"
	fi
	cd ../..
else
	echo "Capstone already exist!"
fi
Capstone_Path=`pwd`"/capstone"
echo "Capstone Path: "$Capstone_Path

if [ ! -d "./llvm-mc" ]
then
	echo "Building LLVM-MC..."
	git clone https://github.com/StanPlatinum/llvm-mc.git
	cd llvm-mc
	LLVM_MC_Path=`pwd`
	echo "LLVM-MC Path: "$LLVM_MC_Path
	mkdir build
	cd build
	cmake -DLLVM_ENABLE_PROJECTS=clang -DLLVM_TARGETS_TO_BUILD="X86" -G "Unix Makefiles" ../llvm
	make -j4
	if [ $? -ne 0 ]
	then
		echo "LLVM-MC install failed!"
	else
		echo "LLVM-MC install succeed!"
	fi
	cd ../..
else
	echo "LLVM-MC already exists!"
fi
LLVM_MC_Path=`pwd`"/llvm-mc"
LLVM_MC_Bin_Path="$LLVM_MC_Path/build/bin"
echo "LLVM-MC Bin Path: "$LLVM_MC_Bin_Path


if [ ! -d "./proofGen" ]
then
	echo "Building ProofGen..."
	git clone https://github.com/StanPlatinum/proofGen.git
	cd proofGen
	ProofGen_Path=`pwd`
	echo "LLVM Path: "$ProofGen_Path
	mkdir build
	cd build
	cmake -DLLVM_ENABLE_PROJECTS=clang -DLLVM_TARGETS_TO_BUILD="X86" -G "Unix Makefiles" ../llvm
	make
	if [ $? -ne 0 ]
	then
		echo "ProofGen install failed!"
	else
		echo "ProofGen install succeed!"
	fi
	cd ../..
else
	echo "ProofGen already exists!"
fi
ProofGen_Path=`pwd`"/proofGen"
ProofGen_Bin_Path="$ProofGen_Path/build/bin"
echo "ProofGen Bin Path: "$ProofGen_Bin_Path

echo "Dependencies install successfully!"
cd $Bash_Dir


echo "Configuring loader..."
cd loader
echo "Generating Makefile header..."
echo "SGX_SRC_PATH = "$SGXSDK_Source_Path > Makefile_header4checker
echo "ELFUTILS_PATH = "$Libelf_Path >> Makefile_header4checker
echo "CAPSTONE_PATH = "$Capstone_Path >> Makefile_header4checker
cat Makefile_header4checker Makefile_template4checker > Makefile
make
rm Makefile_header4checker
cd target-program
echo "Generating Makefile header..."
CC4AS=$LLVM_MC_Path"/build/bin/clang -fPIC -fno-asynchronous-unwind-tables -fno-addrsig"
echo "CC = "$CC4AS > Makefile_header4target
cat Makefile_header4target Makefile_template4target > Makefile
rm Makefile_header4target
echo "Generating all including musl-libc"
make clean-all
make CC="$CC4AS"
if [ $? -ne 0 ]
then
	echo "Musl-libc install failed!"
	make clean-all
else
	echo "Musl-libc install succeed!"
fi
make clean
cd ../..


echo "Configuring dynamic-loader-checker..."
cd dynamic-loader-checker
echo "Generating Makefile header..."
echo "SGX_SRC_PATH = "$SGXSDK_Source_Path > Makefile_header4checker
echo "ELFUTILS_PATH = "$Libelf_Path >> Makefile_header4checker
echo "CAPSTONE_PATH = "$Capstone_Path >> Makefile_header4checker
cat Makefile_header4checker Makefile_template4checker > Makefile
make
rm Makefile_header4checker
cd target-program
echo "Generating Makefile header..."
echo "LLVM_PATH = "$ProofGen_Path > Makefile_header4target
echo "Our_AS_Path = "$LLVM_MC_Path >> Makefile_header4target
cat Makefile_header4target Makefile_template4target > Makefile
rm Makefile_header4target
make clean-all
echo "Generating musl-libc"
cp -r ../../loader/target-program/musl-libc .
cd ..
cd sc-resil-tg
echo "Generating Makefile header..."
echo "CC = "$CC4AS > Makefile_header4target
echo "LLVM_PATH = "$ProofGen_Path > Makefile_header4target
echo "Our_AS_Path = "$LLVM_MC_Path >> Makefile_header4target
echo "SGX_SRC_PATH = "$SGXSDK_Source_Path >> Makefile_header4target
cat Makefile_header4target Makefile_template4target > Makefile
rm Makefile_header4target
echo "Generating all including musl-libc"
make clean-all
cp -r ../target-program/musl-libc .
cd ../..


echo "Installed!"
