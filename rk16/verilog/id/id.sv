module id (
    input wire [31:0] inst,
    input wire [3:0] stage,
    output logic pfc_ctrl,
    output logic alu_ctrl,
    output logic s2_sel,
    output logic [15:0] imm,
    output logic [2:0] din_sel,
    output logic [15:0] sa1,
    output logic [15:0] sa2,
    output logic [15:0] da,
    output logic [5:0] addr_sel,
    output logic sp_ctrl
);

  always_comb begin
    case (inst[3:0])
      4'b0000: begin
        imm = inst[31:16];
      end
      default: begin
        imm = inst[31:16];
      end
    endcase
  end

endmodule
