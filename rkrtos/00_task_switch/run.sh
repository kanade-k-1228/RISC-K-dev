cat ../lib.rk main.rk > a.rk
rkasm a.rk
# rkemu -ot -d main.dump -i main.intr a.rk.bin
rkemu a.rk.bin
# rkemu -i main.intr a.rk.bin
