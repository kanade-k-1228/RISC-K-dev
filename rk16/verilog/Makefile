simu: build/top_tb.vcd
	gtkwave $^

build/top_tb.vvp: */*.sv *.sv
	iverilog -g 2012 -s top_tb -o $@ $^

build/top_tb.vcd: build/top_tb.vvp
	vvp $^ > build/top_tb.log
