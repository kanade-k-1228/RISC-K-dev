module mem (
    input wire clk,
    input wire [15:0] addr,
    input wire [15:0] st_data,
    output wire [15:0] ld_data
);
  reg [15:0] ram[0:64*1024-1];
endmodule
