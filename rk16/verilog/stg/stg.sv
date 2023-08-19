module stg (
    input wire clk,
    output logic [3:0] stage,
    output logic [3:0] stg_clk,
    output logic rst_stg,
    output logic rst_clk
);

  wire resetn;
  reg [1:0] rst_cnt;
  wire rst = &rst_cnt;

  assign rst_stg = ~(resetn & rst);
  assign rst_clk = ^rst_cnt;

  por por (
      .resetn(resetn),
      .clk(clk)
  );

  logic [2:0] cnt;

  assign stage   = (resetn & rst) ? 1 << cnt[2:1] : 0;
  assign stg_clk = cnt[0] ? (1 << cnt[2:1]) : 0;

  always @(posedge clk) begin
    if (!resetn) begin
      cnt <= 0;
      rst_cnt <= 2'b00;
    end else begin
      if (~&rst_cnt) begin
        rst_cnt <= rst_cnt + 1;
      end else begin
        cnt <= cnt + 1;
      end
    end
  end

endmodule
