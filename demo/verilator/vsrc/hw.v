module hw (
  input  clk,
  output A,
  output B,
  output C,
  output D,
  output E,
  output F,
  output G
);

  // Flip-flop: X and Y
  reg X, Y;
  wire X1, Y1;

  always @(posedge clk) begin
    X <= X1;
    Y <= Y1;
  end

  initial begin
    X = 1'b0;
    Y = 1'b0;
  end

  assign X1                    = ~X & Y;
  assign Y1                    = ~(X | Y);

  // Output
  assign {A, B, C, D, E, F, G} = {~Y, 1'b1, ~X, ~Y, ~Y, Y1, X};

endmodule
