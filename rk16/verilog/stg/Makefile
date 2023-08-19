simu: build/stg_tb.vcd
	gtkwave $^

build/stg_tb.vvp: *.sv
	iverilog -g 2012 -s stg_tb -o $@ $^

build/stg_tb.vcd: build/stg_tb.vvp
	vvp $^ > build/stg_tb.log
