`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: TAMU
// Engineer:
//
// Create Date:   
// Design Name:   VNU
// Module Name:   VNUTest_v
// Project Name:  LDPC
// Target Device:  
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

`define STRLEN 15
module VNUTest_v;

	initial begin
	$dumpfile("VNU.vcd");
	$dumpvars;
	end

	task passTest;
		input [3:0] actualOut, expectedOut;
		input [`STRLEN*8:0] testType;
		inout [7:0] passed;
	
		if(actualOut == expectedOut) begin $display ("%s passed", testType); passed = passed + 1; end
		else $display ("%s failed: %d should be %d", testType, actualOut, expectedOut);
	endtask
	
	task allPassed;
		input [7:0] passed;
		input [7:0] numTests;
		
		if(passed == numTests) $display ("All tests passed");
		else $display("Some tests failed");
	endtask
	
	// Inputs
	reg [2:0] S;
	reg [7:0] passed;

	// Outputs
	wire [5:0] Cs;

	// Instantiate the Unit Under Test (UUT)
	VNU uut (
		.S(S), 
		.Cs(Cs)
	);

	initial begin
		// Initialize Inputs
		S = 0;
		passed = 0;

		// Add stimulus here
        #90; S = 3'b110; #10; passTest(Cs, 6'b110010, "VNU Input 0", passed);
		#90; S = 3'b101; #10; passTest(Cs, 6'b101110, "VNU Input 1", passed);
		#90; S = 3'b000; #10; passTest(Cs, 6'b000000, "VNU Input 2", passed);
		#90; S = 3'b100; #10; passTest(Cs, 6'b100101, "VNU Input 3", passed);
		#90;
		
		allPassed(passed, 4);

	end
      
endmodule

