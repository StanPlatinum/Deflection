import subprocess
import sys
import os

llvmBinDir = ""
llvmPassPath = ""
codePath = ""
CFISrcPath = ""
entryLabelPath = ""
codeDir = ""
CFISrcDir = ""
codeName = ""
CFISrcName = ""
linkPath = ""
optPath = ""

def chanegSrcFileName(file,old_str,new_str):
    file_data = ""
    with open(file, "r", encoding="utf-8") as f:
        for line in f:
            if old_str in line:
                line = line.replace(old_str,new_str)
            file_data += line
    with open(file,"w",encoding="utf-8") as f:
        f.write(file_data)

def mergeEntryName(fileThis, fileTotal):
    file_data = ""
    with open(fileThis, "r", encoding="utf-8") as f:
        for line in f:
            file_data += line
    with open(fileTotal, "a", encoding="utf-8") as f:
        f.write(file_data)

#init
llvmBinDir = sys.argv[1]
llvmPassPath = sys.argv[2]
codePath = sys.argv[3]
CFISrcPath = sys.argv[4]
#entryLabelPath = sys.argv[5]
isNewCFISrc = sys.argv[5]

llvmBinDir += "/"
codeName = codePath.split("/")[-1].split(".")[0]
CFISrcName = CFISrcPath.split("/")[-1].split(".")[0]
codeDir = codePath.replace(codePath.split("/")[-1], "")
CFISrcDir = CFISrcPath.replace(CFISrcPath.split("/")[-1], "")
print("\nshell: " + "mkdir " + codeDir + codeName + " :")
print(subprocess.call("mkdir " + codeDir + codeName, shell=True))
firstPath = codeDir + codeName + "/" + codeName + ".ll"
linkPath = codeDir + codeName + "/" + codeName + "_link.ll"
optPath = codeDir + codeName + "/" + codeName + "_opt.ll"

#llvm
print("\nshell: " + llvmBinDir + "clang -emit-llvm -fno-asynchronous-unwind-tables -S " + codePath + " -o " + firstPath + " :")
print(subprocess.call(llvmBinDir + "clang -emit-llvm -fno-asynchronous-unwind-tables -S " + codePath + " -o " + firstPath, shell=True))

if isNewCFISrc == "n":
    print("\nshell: " + llvmBinDir + "clang -emit-llvm -fno-asynchronous-unwind-tables -S " + CFISrcPath + " :")
    print(subprocess.call(llvmBinDir + "clang -emit-llvm -fno-asynchronous-unwind-tables -S " + CFISrcPath, shell=True))
elif isNewCFISrc == "o":
    if os.path.isfile(CFISrcDir + CFISrcName + ".ll"):
        pass
    else:
        print("\nshell: " + llvmBinDir + "clang -emit-llvm -fno-asynchronous-unwind-tables -S " + CFISrcPath + " :")
        print(subprocess.call(llvmBinDir + "clang -emit-llvm -fno-asynchronous-unwind-tables -S " + CFISrcPath, shell=True))

print("\nshell: " + llvmBinDir + "llvm-link " + CFISrcDir + CFISrcName + ".ll " + firstPath + " -S -o " + linkPath + " :")
print(subprocess.call(llvmBinDir + "llvm-link " + CFISrcDir + CFISrcName + ".ll " + firstPath + " -S -o " + linkPath, shell=True))

chanegSrcFileName(linkPath, "llvm-link", codeName)

print("\nshell: " + llvmBinDir + "opt -load " + llvmPassPath + " -cfihello -S <" + linkPath + "> " +optPath + " :")
print(subprocess.call(llvmBinDir + "opt -load " + llvmPassPath + " -cfihello -S <" + linkPath + "> " +optPath, shell=True))

print("\nshell: " + llvmBinDir + "llc " + optPath + " :")
print(subprocess.call(llvmBinDir + "llc " + optPath, shell=True))

#mergeEntryName(codeDir + codeName + "/" + codeName +".txt", entryLabelPath)

print("\nDone.")