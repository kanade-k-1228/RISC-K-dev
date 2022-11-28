../make.sh
cat ../build/lib.rk main.rk > build/out.rk
rkasm build/out.rk
rkemu -i main.intr build/out.rk.bin