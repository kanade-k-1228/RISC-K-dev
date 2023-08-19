simu: build/alu_tb.vcd

build/alu_tb.vvp: *.sv
	iverilog -g 2012 -s alu_tb -o $@ $^

build/alu_tb.vcd: build/alu_tb.vvp
	vvp $^ > build/alu_tb.log
