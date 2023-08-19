module rom (
    wire [15:0] addr,
    wire [31:0] data
);

  reg [31:0] memory[0:64*1024-1];
  initial begin
    $readmesh("rom.hex", memory);
  end

  assign data = memory[addr];

endmodule
