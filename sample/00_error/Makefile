main: main.rk.bin
	rkemu -ta $^

main.rk.bin: main.rk
	rkasm $^

.PHONY: main
.INTERMEDIATE: main.rk.bin
