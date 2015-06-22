/********************************************************
	King Abdullah University of Science and Technology
	CS 282 Project: Control_UnitV2.v
	Spring 2013
	
	Please fill your names here:
	Group Member 1: YourFirstName1 YourLastName1
	Group Member 2: YourFirstName2 YourLastName2
	
	WARNING: Only add code in allowed sections. 
********************************************************/

`include "mips.h"
module Control_UnitV2(INS, Stall, BR, ALUC, Lmode, DMC, RDSEL, ASEL, BSEL, WBmux, WE, PCmux, IMM_ctrl);

input 		[31:0]	INS;			// Instruction Word
input	    		Stall;			// Stall signal from stall unit
output	reg	 [3:0] 	BR;				// Branch Control 
output	reg	 [3:0]	ALUC; 			// ALU Control   
output	reg	 [2:0]  Lmode;			// Loading Mode for Load Parse Block
output	reg	 [1:0]	DMC;			// Data Memory Controller Signal
output	reg	 [1:0]	RDSEL; 			// Register Store address Selector for Register File
output	reg	 [1:0] 	ASEL;			// Selector for Input A for ALU
output 	reg			BSEL;			// Selector for Input B of ALU
output	reg			WBmux;			// Selector for Mux in Write-Back Stage
output	reg			WE;				// Write Enable for Register File
output	reg			PCmux;			// Selector for Mux Before PC register
output	reg			IMM_ctrl;		// Selector for Sign/Zero extension of Immediate

// Your Code Begins Here
// Copy-and-Paste your code from part 2, and then modify to integerate with Stall signal

always @ ( )
	begin
		
		
	end

// Your Code Ends Here

endmodule

