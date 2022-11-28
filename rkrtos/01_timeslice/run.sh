cat ../lib.rk main.rk > a.rk
rkasm a.rk
# rkemu -d main.dump -i main.intr a.rk.bin
# rkemu a.rk.bin
# rkemu -to -d main.dump a.rk.bin
rkemu -i main.intr a.rk.bin