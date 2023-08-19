module stg (
    input wire resetn,
    input wire clk,
    output logic [3:0] stage,
    output logic [3:0] stg_clk
);

  logic [2:0] cnt;

  assign stage   = resetn ? 1 << cnt[2:1] : 0;
  assign stg_clk = cnt[0] ? (1 << cnt[2:1]) : 0;

  always @(posedge clk) begin
    if (!resetn) cnt <= 0;
    else cnt <= cnt + 1;
  end

endmodule
