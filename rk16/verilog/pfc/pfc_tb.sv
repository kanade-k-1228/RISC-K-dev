module pfc_tb;

  logic clk_pc;

  logic [15:0] cond;
  logic [15:0] alu;
  logic pfc_ctrl;
  logic intr;

  wire [15:0] pc;
  wire [15:0] pc_inc;
  wire [15:0] intr_ra;

  pfc pfc (
      .clk_pc(clk_pc),
      .cond(cond),
      .alu(alu),
      .pfc_ctrl(pfc_ctrl),
      .intr(intr),
      .pc(pc),
      .pc_inc(pc_inc),
      .intr_ra(intr_ra)
  );

  localparam period = 1;
  integer i;
  initial begin
    $dumpfile("build/pfc_tb.vcd");
    $dumpvars(0, pfc);
    $monitor("%16b", pc);

    clk_pc = 0;

    cond = 0;
    alu = 16'h1234;
    pfc_ctrl = 0;
    intr = 1;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    intr   = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc   = 0;

    pfc_ctrl = 1;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc   = 0;

    pfc_ctrl = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    intr   = 1;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

    intr   = 0;

    #period;
    clk_pc = 1;
    #period;
    clk_pc = 0;

  end

endmodule
