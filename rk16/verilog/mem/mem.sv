module mem (
    input wire clk,
    input wire [15:0] addr,
    input wire [15:0] st_data,
    output logic [15:0] ld_data
);

  reg [15:0] ram[0:64*1024-1];

  always_comb begin
    case (addr)
      16'h0000: ld_data = 16'h0000;
      default:  ld_data = ram[addr];
    endcase
  end

  always @(posedge clk) begin
    case (addr)
      16'h0000: ram[addr] <= 16'h0000;
      default:  ram[addr] <= st_data;
    endcase
  end

endmodule
