simu: build/id_tb.vcd

build/id_tb.vvp: *.sv
	iverilog -g 2012 -s id_tb -o $@ $^

build/id_tb.vcd: build/id_tb.vvp
	vvp $^ > build/id_tb.log
