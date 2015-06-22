/********************************************************
	King Abdullah University of Science and Technology
	CS 282 Project: Load_Stall.v
	Module Code: CU009
	Spring 2013
	
	Please fill your names here:
	Group Member 1: Monther Busbait
	Group Member 2: Shahzeb Siddiqui
	
	WARNING: Only add code in allowed sections. 
********************************************************/
	 
module Load_Stall_v2(RDaddr_EX, RSaddr_ID, RTaddr_ID, WE_EX, DMC_EX, WBmux_EX, Stall, BranchPredictSel, PC_EN, IFID_EN);

input	 	 [4:0] 	RDaddr_EX;		// Destination address of Instruction in EX stage
input	 	 [4:0] 	RSaddr_ID;		// Register A read address of Instruction in ID stage
input	 	 [4:0] 	RTaddr_ID;		// Register B read address of Instruction in ID stage
input				WE_EX; 			// EX Stage Write Enable for Register File
input   	 [1:0]	DMC_EX;			// EX Stage Data Memory Controller Signal
input			    WBmux_EX;		// EX Stage Selector for Mux in Write-Back Stage
input 				BranchPredictSel;
output reg   Stall;					// Stall signal 
output reg   PC_EN;					// Enable signal for PC
output reg   IFID_EN;				// Enable signal for IFID pipeline register

// Your Code Begins Here
always @ (*)
begin
	if (BranchPredictSel == 1)							// Instruction After Branch Prediction must be flushed in case of Branch Taken.
																// This implementation will need to be implemented 
		begin
		
			//FLUSH Next Instruction
		end
	else if(BranchPredictSel == 1 && (RSaddr_ID == RDaddr_EX) 	|| (RTaddr_ID == RDaddr_EX))	
		begin
			// PCmuxOverride = 0					// need to prevent branching in this case due to data dependency. Refer to example in BranchPredictor.v for more details
		end
	
	// Data Dependency	in BranchPrediction
	else if (DMC_EX == 2'b0 && WBmux_EX == 0 && WE_EX == 1 && (RDaddr_EX == RSaddr_ID || RDaddr_EX == RTaddr_ID))
		begin
			Stall = 1;	
			PC_EN = 0;
			IFID_EN = 0;
		end
	else	
		begin
			Stall = 0;	
			PC_EN = 1;
			IFID_EN = 1;
		end
end
// Your Code Ends Here

endmodule 