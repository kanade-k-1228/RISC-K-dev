module id (
    input wire [31:0] inst,
    input wire [ 3:0] stage,

    output logic [ 3:0] opc,
    output logic [ 3:0] sa1,
    output logic [ 3:0] sa2,
    output logic [ 3:0] da,
    output logic [15:0] imm,

    output logic [3:0] alu_ctrl,

    output logic pfc_ctrl,

    output logic s2_sel,
    output logic [2:0] din_sel,
    output logic [4:0] addr_sel,
    output logic sp_ctrl
);

  localparam CALC = 4'b0000, CALCI = 4'b0001, LOAD = 4'b0011, STORE = 4'b0111, CALIF = 4'b1111;
  localparam STG0 = 4'b0001, STG1 = 4'b0010, STG2 = 4'b0100, STG3 = 4'b1000;

  assign {imm, da, sa2, sa1, opc} = inst;

  assign alu_ctrl = opc == CALC ? inst[19:16] : opc == CALCI ? inst[11:8] : 4'b0000;

  assign pfc_ctrl = opc == CALIF;

  assign s2_sel = opc == CALC ? 0 : 1;

  localparam DIN_SR2 = 3'b001, DIN_ALU = 3'b010, DIN_RA = 3'b100;
  assign din_sel = (opc == CALC || opc == CALCI) ? DIN_SR2
                 : (opc == LOAD || opc == STORE) ? DIN_ALU
                 :                                 DIN_RA;

  localparam ADDR_ALU = 5'b00001, ADDR_SA1 = 5'b00010, ADDR_SA2 = 5'b00100, ADDR_DA=5'b01000, ADDR_DBG=5'b10000;
  assign addr_sel = (stage == STG0) ? (ADDR_SA1) 
                  : (stage == STG1) ? (opc == LOAD  ? ADDR_ALU : ADDR_SA2)
                  : (stage == STG2) ? (opc == STORE ? ADDR_ALU : ADDR_DA )
                  : 5'b00000;

  logic [63:0] ascii_opc;
  always_comb
    case (opc)
      CALC: ascii_opc = "CALC";
      CALCI: ascii_opc = "CALC_IMM";
      LOAD: ascii_opc = "LOAD";
      STORE: ascii_opc = "STORE";
      CALIF: ascii_opc = "CTRL";
      default: ;
    endcase

endmodule
