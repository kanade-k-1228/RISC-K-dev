all: ../lib/start.rk main.rk
	mkdir -p build
	cat $^ > build/out.rk
	rkasm build/out.rk
	rkemu build/out.rk.bin
