module stg_tb;

  logic resetn;
  logic clk = 0;
  always #1 clk = ~clk;

  por por (
      .resetn(resetn),
      .clk(clk)
  );

  stg stg (
      .resetn(resetn),
      .clk(clk)
  );

  initial begin
    $dumpfile("build/stg_tb.vcd");
    $dumpvars(0, stg_tb);
    repeat (100) @(posedge clk);
    $finish;
  end

endmodule
