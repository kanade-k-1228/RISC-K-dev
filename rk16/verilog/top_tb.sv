`timescale 1 ns / 1 ps

module top_tb;

  logic clk = 0;
  always #10 clk = ~clk;

  top top (.clk(clk));

  initial begin
    $dumpfile("build/top_tb.vcd");
    $dumpvars(0, top_tb);
    repeat (1000) @(posedge clk);
    $finish;
  end

endmodule
