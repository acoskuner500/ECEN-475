`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: TAMU
// Engineer:
//
// Create Date:   
// Design Name:   CNU
// Module Name:   CNUTest_v
// Project Name:  LDPC
// Target Device:  
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

`define STRLEN 15
module CNUTest_v;

	initial begin
	$dumpfile("CNU.vcd");
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
  	reg [5:0] Cs;
	reg [7:0] passed;

	// Outputs
	wire [2:0] SO;
	wire err;

	// Instantiate the Unit Under Test (UUT)
	CNU uut (
		.SO(SO), 
		.Cs(Cs),
		.err(err)
	);

	initial begin
		// Initialize Inputs
		Cs = 0;
		passed = 0;

		// Add stimulus here

		#90; Cs = 6'b110010; #10; passTest({err,SO}, 4'b0000, "CNU Input 0", passed);
		#90; Cs = 6'b010011; #10; passTest({err,SO}, 4'b1100, "CNU Input 1", passed);
		#90; Cs = 6'b100101; #10; passTest({err,SO}, 4'b0000, "CNU Input 2", passed);
		#90; Cs = 6'b111010; #10; passTest({err,SO}, 4'b1011, "CNU Input 3", passed);
		#90;
		
		allPassed(passed, 4);
	end
endmodule

