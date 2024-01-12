simu: build/rom.hex build/top_tb.vcd
#	gtkwave $^
	gtkwave top_tb.gtkw

build/rom.inv.hex: ../../sample/00_test_asm/ok.rk.bin
	xxd -c4 -ps $^ > $@

build/rom.hex: build/rom.inv.hex
	python3 fixhex.py > $@

build/top_tb.vvp: */*.sv *.sv
	iverilog -g 2012 -s top_tb -o $@ $^

build/top_tb.vcd: build/top_tb.vvp
	vvp $^ > build/top_tb.log
