module id_tb;
  id id (
      .a  (a),
      .b  (b),
      .sel(sel),
      .out(out)
  );

  localparam period = 1;
  integer i;
  initial begin
    $dumpfile("id_tb.vcd");
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
