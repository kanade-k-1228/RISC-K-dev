module alu_tb;
  logic [15:0] a;
  logic [15:0] b;
  logic [ 3:0] sel;
  logic [15:0] out;

  alu alu (
      .a  (a),
      .b  (b),
      .sel(sel),
      .out(out)
  );

  localparam period = 1;
  integer i;
  initial begin
    $dumpfile("alu_tb.vcd");
    $monitor("%4b, %16b, %16b, %16b", sel, a, b, out);

    for (i = 0; i <= 4'hf; i = i + 1) begin

      sel = i;

      a   = 16'h0000;
      b   = 16'h0000;
      #period;

      a = 16'hffff;
      b = 16'h0000;
      #period;

      a = 16'h0000;
      b = 16'hffff;
      #period;

      a = 16'hffff;
      b = 16'hffff;
      #period;

      a = 16'h1234;
      b = 16'h5678;
      #period;

      $display("");

    end

  end

endmodule
