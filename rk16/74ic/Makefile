simu: IC_7400_tb.vcd
	gtkwave $^

IC_7400_tb.vvp: IC_7400.v IC_7400_tb.v
	iverilog -g 2012 -s IC_7400_tb -o $@ $^

IC_7400_tb.vcd: IC_7400_tb.vvp
	vvp $^ > IC_7400_tb.log
