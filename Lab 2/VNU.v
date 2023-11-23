`timescale 1ns / 1ps
module VNU(S, Cs);
    output reg [5:0] Cs;
    input [2:0] S;
    reg [5:0] G [2:0];
    integer i, j;

    initial begin
        G[2] = 6'b100101;
        G[1] = 6'b010111;
        G[0] = 6'b001011;
    end

    always @(S) begin
        Cs = 0;
        for (i=0; i<6; i=i+1) begin
            for (j=0; j<3; j=j+1) begin
                if (G[j][i]) Cs[i] = Cs[i] ^ S[j];
            end
        end
    end
endmodule