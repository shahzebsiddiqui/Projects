/********************************************************
	King Abdullah University of Science and Technology
	CS 282 Project: Forward.v
	Module Code: CU008
	Spring 2013
	
	Please fill your names here:
	Group Member 1: YourFirstName1 YourLastName1
	Group Member 2: YourFirstName2 YourLastName2
	
	WARNING: Only add code in allowed sections. 
********************************************************/

module Forward(RA_EX, RB_EX, RD_MEM, RD_WB, WE_MEM, WE_WB, sFORA, sFORB);

input	 	 [4:0] 	RA_EX;		// Register A read address of Instruction in EX stage
input	 	 [4:0] 	RB_EX;		// Register B read address of Instruction in EX stage
input	 	 [4:0] 	RD_MEM;		// Destination address of Instruction in MEM stage
input	 	 [4:0]	RD_WB;		// Destination address of Instruction in WB stage
input				WE_MEM;		// Write Enable of Instruction in MEM stage
input				WE_WB;		// Write Enable of Instruction in WB stage
output 	reg	 [1:0] 	sFORA;		// Selection for Forwarding Mux for Data from Register A
output 	reg	 [1:0]	sFORB;		// Selection for Forwarding Mux for Data from Register B

// Your Code Begins Here

always @ ( )
	begin

	end

// Your Code Ends Here

endmodule 