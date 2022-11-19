#!/bin/sh

cd ~/RISC-K/rkasm/build
cmake ..
make
cd ~/RISC-K/rkemu/build
cmake ..
make
cd ~/RISC-K/rkcc/build
cmake ..
make

export PATH=$HOME/RISC-V/bin:$PATH
