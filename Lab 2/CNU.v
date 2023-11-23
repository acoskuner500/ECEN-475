`timescale 1ns / 1ps
module CNU(SO, err, Cs);
    input [5:0] Cs;
    output reg [2:0] SO;
    output reg err; // error bit, 1 if SO ≠ 0, 0 if SO = 0. The default value is 0
    reg [5:0] H [2:0];
    integer i,j;
    
    initial begin
        H[2] = 6'b110100;
        H[1] = 6'b011010;
        H[0] = 6'b111001;
    end
    always @(Cs) begin
        SO = 0;
        for (i=0; i<6; i=i+1) begin
            for (j=0; j<3; j=j+1) begin
                if (Cs[i]) SO[j] = SO[j] ^ H[j][i];
            end
        end
        err = (SO != 0);
    end
endmodule