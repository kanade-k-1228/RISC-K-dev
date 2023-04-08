rkasm main.rk
rkasm error_noopc.rk
rkasm error_noarg.rk
rkemu -ta main.rk.bin
rkasm label.rk
rkemu -ta label.rk.bin
