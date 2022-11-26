cat ../lib.rk main.rk > a.rk
rkasm a.rk
rkemu -ot -d main.dump -i main.intr a.rk.bin
# rkemu -t -i main.intr a.rk.bin
