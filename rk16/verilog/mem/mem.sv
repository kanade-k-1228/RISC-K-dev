module mem (
    input wire clk,
    input wire [15:0] addr,
    input wire [15:0] st_data,
    output logic [15:0] ld_data,

    input wire [15:0] ira,
    input wire [15:0] pc,
    input wire sp_ctrl
);

  reg [15:0] ram[0:64*1024-1];

  always_comb begin
    case (addr)
      16'h0000: ld_data = 16'h0000;
      16'h0001: ld_data = ira;
      16'h0002: ld_data = pc;
      default:  ld_data = ram[addr];
    endcase
  end

  always @(posedge clk) begin
    case (addr)
      16'h0000: ram[addr] <= 16'h0000;
      default:  ram[addr] <= st_data;
    endcase
  end

  wire [15:0] reg_ira = ira;
  wire [15:0] reg_pc = pc;
  wire [15:0] reg_sp = ram[3];
  wire [15:0] reg_ra = ram[4];
  wire [15:0] reg_fp = ram[5];
  wire [15:0] reg_a0 = ram[6];
  wire [15:0] reg_a1 = ram[7];
  wire [15:0] reg_t0 = ram[8];
  wire [15:0] reg_t1 = ram[9];
  wire [15:0] reg_t2 = ram[10];
  wire [15:0] reg_t3 = ram[11];
  wire [15:0] reg_s0 = ram[12];
  wire [15:0] reg_s1 = ram[13];
  wire [15:0] reg_s2 = ram[14];
  wire [15:0] reg_s3 = ram[15];

endmodule
