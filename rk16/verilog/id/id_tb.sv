module id_tb;
  logic [31:0] inst;
  logic [ 3:0] stage = 0;
  wire  [15:0] imm;
  id id (
      .inst (inst),
      .stage(stage),
      .imm  (imm)
  );

  localparam period = 1;
  initial begin
    $dumpfile("build/id_tb.vcd");
    $dumpvars(0, id);

    inst = 32'h0000_0000;
    #period;
    inst = 32'h1234_1234;
    #period;
    inst = 32'h4567_4567;
    #period;
    inst = 32'h89AB_89AB;
    #period;
    inst = 32'hCDEF_CDEF;

  end

endmodule
