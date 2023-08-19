module stg_tb;

  logic clk = 0;
  always #1 clk = ~clk;

  stg stg (.clk(clk));

  initial begin
    $dumpfile("build/stg_tb.vcd");
    $dumpvars(0, stg_tb);
    repeat (100) @(posedge clk);
    $finish;
  end

endmodule
