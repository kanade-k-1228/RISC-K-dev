rkcc test.cyy > test.rk
rkasm -d test.rk > test.rk.bin
rkemu -at test.rk.bin
