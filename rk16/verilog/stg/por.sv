module por #(
    parameter N = 6
) (
    input  wire clk,
    output wire resetn
);
  reg [N-1:0] rst_cnt = 0;
  assign resetn = &rst_cnt;
  always @(posedge clk) rst_cnt <= rst_cnt + !resetn;
endmodule
