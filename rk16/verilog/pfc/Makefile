simu: build/pfc_tb.vcd
	gtkwave $^

build/pfc_tb.vvp: *.sv
	iverilog -g 2012 -s pfc_tb -o $@ $^

build/pfc_tb.vcd: build/pfc_tb.vvp
	vvp $^ > build/pfc_tb.log

