`timescale 1ns/1ps

module IC_7400_tb;

reg  [3:0] A;
reg  [3:0] B;
wire [3:0] Y;

IC_7400 #(.DELAY(6)) ic(
  .A0(A[0]), .A1(A[1]), .A2(A[2]), .A3(A[3]),
  .B0(B[0]), .B1(B[1]), .B2(B[2]), .B3(B[3]),
  .Y0(Y[0]), .Y1(Y[1]), .Y2(Y[2]), .Y3(Y[3])
);

initial begin
  $dumpfile("IC_7400_tb.vcd");

  A <= 4'b0000;
  B <= 4'b0000;

#10

  A <= 4'b0011;
  B <= 4'b0101;

#10

  $finish;

end

endmodule
