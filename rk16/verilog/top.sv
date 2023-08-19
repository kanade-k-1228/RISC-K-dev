module cpu (
    input wire clk,
    input wire
);

  alu alu ();

  pcf pfc ();

  wire [15:0] pc;
  wire [15:0] data;

  rom rom (
      .addr(pc),
      .data(data)
  );

endmodule
