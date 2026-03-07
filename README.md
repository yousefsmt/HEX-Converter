
# HEX Converter

## Introduction

This repository implements a base number converter for four basis octal, hex, decimal and binary.
This code is a CLI and can be installed in Linux distro 

---

## Requirement

CMake >= 3.16
GCC >= v9.5
Each Linux distribution with supported above POSIX.1-2008.

---

## Build

You can build from source and do step by step below command.

```
git clone https://github.com/yousefsmt/HEX-Converter.git
cd HEX-Converter
mkdir build
cd build
cmake ..
make -j $(nproc)
# If you can install on OS (host)
# you need root permissions in build folder
# then do below command
sudo make install
```

---

## NOTE

You must use each below prefix for each base:
1- "0x" for hexadecimal number
2- "0b" for binary number
3- "0o" for octal number
4- "0d" for decimal number

maximum number for each value is 32 bit and cannot set above this value.

```
HEX Converter v0.1.0 Developed by: Yousef.smt
   Usage: ./hexconvert [OPTION [VALUE]]
  -x, --hex
        Set hex number. [0x00000000-0xFFFFFFFF]
  -o, --octal
        Set octal number. [0o00000000000-0o37777777777]
  -d, --decimal 
        Set decimal number. [0-4294967295]
  -b, --binary 
        Set binary number. [0b0...00-0b1...11 max 32 Bit]
  -h, --help
        Display help message and exit.
  --license
        Display the license information.
```

---

## Application

Application this code for me is embedded system because when I need to write bare-metal programming I need to write number with hex value and sometimes I need to convert to value for check true are false, before implement this code I use with online tools and in Iran sometimes disconnect global internet.

## Contribute

Everyone can improve this code and I happy to contibute with just you need fork this project create branch and fix bug are add feature then create pull request and I check it, Is it good? If yes, I merge it.
Good Luck.


