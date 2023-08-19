module top (
    input wire clk
);

  stg stg (
      .clk(clk),
      .stage(stage),
      .stg_clk(stg_clk),
      .rst_stg(rst_stg),
      .rst_clk(rst_clk)
  );

  wire [3:0] stage;
  wire [3:0] stg_clk;
  wire rst_stg, rst_clk;

  pfc pfc (
      .intr(rst_stg),
      .clk_pc(rst_clk | stg_clk[3]),
      .cond(16'b0),
      .alu(16'b0),
      .pfc_ctrl(1'b0),
      .pc(pc)
  );

  wire [15:0] pc;

  rom rom (
      .addr(pc),
      .data(data)
  );

  wire [31:0] data;

  id id (
      .inst(data),
      .stage(stage),
      .s2_sel(s2_sel),
      .imm(imm),
      .alu_ctrl(alu_ctrl)
  );

  wire [15:0] imm;
  wire s2_sel;

  wire [3:0] alu_ctrl;

  reg [15:0] rs1, rs2;

  wire [15:0] alu_out;

  alu alu (
      .a  (rs1),
      .b  (s2_sel ? rs2 : imm),
      .sel(alu_ctrl),
      .out(alu_out)
  );

endmodule
