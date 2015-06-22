/********************************************************
	King Abdullah University of Science and Technology
	CS 282 Project: Load_Stall.v
	Module Code: CU009
	Spring 2013
	
	Please fill your names here:
	Group Member 1: YourFirstName1 YourLastName1
	Group Member 2: YourFirstName2 YourLastName2
	
	WARNING: Only add code in allowed sections. 
********************************************************/

module Load_Stall(RDaddr_EX, RSaddr_ID, RTaddr_ID, WE_EX, DMC_EX, WBmux_EX, Stall, PC_EN, IFID_EN);

input	 	 [4:0] 	RDaddr_EX;		// Destination address of Instruction in EX stage
input	 	 [4:0] 	RSaddr_ID;		// Register A read address of Instruction in ID stage
input	 	 [4:0] 	RTaddr_ID;		// Register B read address of Instruction in ID stage
input				WE_EX; 			// EX Stage Write Enable for Register File
input   	 [1:0]	DMC_EX;			// EX Stage Data Memory Controller Signal
input			    WBmux_EX;		// EX Stage Selector for Mux in Write-Back Stage
output reg   Stall;					// Stall signal 
output reg   PC_EN;					// Enable signal for PC
output reg   IFID_EN;				// Enable signal for IFID pipeline register

// Your Code Begins Here
always @ (*)
begin


end
// Your Code Ends Here

endmodule 