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
      .pfc_ctrl(pfc_ctrl),
      .pc(pc),
      .pc_inc(ra),
      .intr_ra(ira)
  );

  wire [15:0] pc, ra, ira;

  rom rom (
      .addr(pc),
      .data(data)
  );

  wire [31:0] data;

  id id (
      .inst(data),
      .stage(stage),
      .sa1(sa1),
      .sa2(sa2),
      .da(da),
      .imm(imm),

      .alu_ctrl(alu_ctrl),

      .pfc_ctrl(pfc_ctrl),

      .s2_sel  (s2_sel),
      .din_sel (din_sel),
      .addr_sel(addr_sel),
      .sp_ctrl (sp_ctrl)
  );

  wire pfc_ctrl;

  wire [4:0] addr_sel;
  wire [3:0] sa1, sa2, da;
  wire [15:0] addr = addr_sel == 5'b00001 ? alu_out
                   : addr_sel == 5'b00010 ? {12'b0, sa1}
                   : addr_sel == 5'b00100 ? {12'b0, sa2}
                   : addr_sel == 5'b01000 ? {12'b0, da}
                   : 16'b0;

  wire [2:0] din_sel;
  wire [15:0] st_data = din_sel == 3'b001 ? rs2
                      : din_sel == 3'b010 ? alu_out
                      : din_sel == 3'b100 ? ra
                      : 0;

  wire [15:0] imm;
  wire s2_sel;
  wire [3:0] alu_ctrl;
  wire [15:0] alu_out;

  reg [15:0] rs1, rs2;

  always @(posedge stg_clk[0]) rs1 <= ld_data;
  always @(posedge stg_clk[1]) rs2 <= ld_data;

  wire [15:0] ld_data;

  alu alu (
      .a  (rs1),
      .b  (s2_sel ? rs2 : imm),
      .sel(alu_ctrl),
      .out(alu_out)
  );

  mem mem (
      .clk(stg_clk[2]),
      .addr(addr),
      .st_data(st_data),
      .ld_data(ld_data),
      .ira(ira),
      .pc(pc)
  );

endmodule
