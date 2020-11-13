#!/bin/bash

Curr_Path=`pwd`
echo $Curr_Path

Bash_Dir=`dirname $0`
echo $Bash_Dir

if [ "$Curr_Path" = "$Bash_Dir" ]
then
        echo "Wrong uninstall path! Please make sure you are in a right elf-respect repo!"
        exit 1
fi

cd ..
Install_Path=`pwd`
echo "entering "$Install_Path

echo "Deleting SGXSDK..."
rm -rf linux-sgx

echo "Deleting Libelf..."
rm -rf elfutils

echo "Deleting Capstone..."
rm -rf capstone

echo "R U sure U want to uninstall ProofGen?"
read -p "Type y or n" var
if [ var == "y" ]
then
	echo "Deleting ProofGen..."
	rm -rf proofGen
fi

echo "R U sure U want to uninstall llvm-mc?"
read -p "Type y or n" var
if [ var == "y" ]
then
	echo "Deleting LLVM-MC..."
	rm -rf llvm-mc
fi

# project name
cd Deflection

cd loader
make clean
rm Makefile
cd target-program
make clean-all
rm Makefile
cd ../..

cd dynamic-loader-checker
make clean
rm Makefile
cd target-program
make clean-all
rm Makefile
cd ../sc-resil-tg
make clean-all
rm Makefile
cd ../..

echo "Uninstall successfully!"

