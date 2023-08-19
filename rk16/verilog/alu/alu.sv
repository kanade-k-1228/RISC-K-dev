module alu (
    input  wire  [15:0] a,
    input  wire  [15:0] b,
    input  wire  [ 3:0] sel,
    output logic [15:0] out
);
  wire a_15 = a[15];
  wire a_0 = a[0];
  wire [14:0] a_15_1 = a[15:1];

  always_comb begin
    case (sel)

      4'b0000: out = a + b;
      4'b0001: out = ~a;
      4'b0010: out = a + a;
      4'b0011: out = a + a + a_15;

      4'b0100: out = a & b;
      4'b0101: out = a ^ b;
      4'b0110: out = a | b;
      4'b0111: out = a - b;

      // Compare
      4'b1000: out = a == b;
      4'b1001: out = a != b;
      4'b1010: out = a < b;
      4'b1011: out = a < b;

      // R Shift
      4'b1100: out = {1'b0, a_15_1};
      4'b1101: out = {a_15, a_15_1};
      4'b1110: out = {a_0, a_15_1};
      4'b1111: out = {a_0, a_15_1};

      default: out = 0;

    endcase
  end
endmodule
