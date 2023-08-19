module pfc #(
    parameter INTR_ADDR = 16'b0000_0000
) (
    input wire intr,
    input wire clk_pc,
    input wire [15:0] cond,
    input wire [15:0] alu,
    input wire pfc_ctrl,
    output logic [15:0] pc,
    output logic [15:0] pc_inc,
    output logic [15:0] intr_ra
);

  wire is_taken;
  wire [15:0] next_pc;
  assign is_taken = (cond == 0) & pfc_ctrl;
  assign next_pc  = is_taken ? alu : pc_inc;

  assign pc_inc   = pc + 1;

  always @(posedge clk_pc) begin
    if (intr) begin
      pc <= INTR_ADDR;
      intr_ra <= next_pc;
    end else begin
      pc <= next_pc;
    end
  end

endmodule
