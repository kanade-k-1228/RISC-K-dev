module IC_7400 #(
  parameter DELAY = 0
)(
  input  A0, A1, A2, A3,
  input  B0, B1, B2, B3,
  output Y0, Y1, Y2, Y3
);

wire _Y0 = ~(A0 & B0);
wire _Y1 = ~(A1 & B1);
wire _Y2 = ~(A2 & B2);
wire _Y3 = ~(A3 & B3);

assign #DELAY Y0 = _Y0;
assign #DELAY Y1 = _Y1;
assign #DELAY Y2 = _Y2;
assign #DELAY Y3 = _Y3;

endmodule
