all: ok

ok.rk.bin: ok.rk
	rkasm $^

ok: ok.rk.bin
	rkemu $^

.PHONY: all ok
