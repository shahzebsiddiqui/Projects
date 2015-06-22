module BranchPredict_BranchJumpSel (BranchPredictTarget,BranchJumpTarget,BranchMux,BranchPredictSel,PCmux_ID,TargetAddress);

input [31:0] BranchPredictTarget;
input [31:0] BranchJumpTarget;
input  BranchMux;
input BranchPredictSel;
input PCmux_ID;
output reg [31:0] TargetAddress;

always @(*)
	begin
		if (PCmux_ID == 0)											// PCmux_ID = 0 implies Instruction in Decode (BranchPrediction) is not Branch/Jump then select BranchJumpTarget as new address
			TargetAddress = BranchJumpTarget;				
		else														// if PCmux_ID = 1 implies branch instruction in Decode
			if(BranchPredictSel == 0 && BranchMux == 0)				// BranchMux = 0 implies PC = PCPlus4_EX, BranchPredictSel = 0 implies PC = PCPlus4_ID 
				TargetAddress = BranchPredictTarget;
			else if (BranchPredictSel == 0 && BranchMux == 1)			// Branch Instruction in Decode & Execute then give precedence to EX Branch			
				TargetAddress = BranchJumpTarget;
			else if (BranchPredictSel == 1 && BranchMux == 0)			// BranchPrediction Taken vs PCPlus4_ID then give precedence to Branch
				TargetAddress = BranchPredictTarget;
			else if (BranchPredictSel == 1 && BranchMux == 1)			// Two Branch Jumps, give precedence to first instruction which is in EX stage
				TargetAddress = BranchJumpTarget;
	end
endmodule

