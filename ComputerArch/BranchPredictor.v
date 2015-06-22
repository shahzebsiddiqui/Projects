module BranchPredictor(BR,PCPlus4, IMM, BranchMux, NextPCPredict, BranchPredictSel);
// TODO: If Predict = 1 and Actual = 1 pass Predict to EX stage PCmux = 0
//       If Predict = 1 and Actual = 0	we need to flush the target adddress

/* 1. Describe in basic logic terms the design algorithm of the branch predictor. You
  can use pseudo code here for illustration purposes.
 
  The Branch Predictor Module must be placed in the Decode Stage and it must properly decide whether to branch or not when encountering 
  a branch instruction. 
  Inputs:
  		BR: is used to select whether its branch instruction (0 - 7) 
  		PCPlus4, IMM: are used for address calculation in case of branch since these are already present in Decode Stage
  		BranchMux: is used to verify prediction and serves as feedback for changing counter
  Outputs:
  		NextPCPredict: output 32 bit address for next instruction or branch target address
 		BranchPredictSel: Signal determining if branch taken or not
 
  BranchInstruction Counters: A 2-bit Saturating Counter for each branch instruction. The counter checks if Most Significant Bit (MSB) is 1. If its true, then it predicts branch is 
  taken and computes by NextPCPredict = PCPlus4 + IMM. Otherwise it assumes branch is not taken and sets NextPCPredict = PCPlus4. BranchPredictSel is set to value of MSB of counter.
  Counter Bit Representation:
		00: Strong Not Taken
		01: Weak Not Taken
		10: Weak Taken
		11: Strong Taken
  		
  2. Describe what happens to the branch decision and branch address calculation
  and at what stage of the pipeline they should be place?
  The Branch Decision must be placed in Decode stage in order to have branch prediction. The
  benefit of having it in Decode Stage is to fetch the target address through prediction without the need to stall in the next
  cycle. The Branch Calculation can also be placed in the Decode stage since only PCPlus4 and IMM are needed for branching according
  to MIPS Branch instruction. Both of these are calculated in Decode Stage. 
  However, with Branch Prediction comes need to validate prediction in pipeline which will be done in EX stage through the regular
  BranchJump Module. 
  
  There are four cases that can occur in the pipeline
 
  Case 1: Prediction = 0, Actual = 0				Not using BranchPredictor or Branch_Jump module
  Case 2: Prediction = 0, Actual = 1				Only using Branch_Jump module 
  Case 3: Prediction = 1, Actual = 0				Requires implementation since BranchPrediction is incorrect and need to rollback to PCPlus4 using Branch_Jump module
  Case 4: Prediction = 1, Actual = 1				Requires implementation to avoid reexecuting same instruction but we assume that is implemented already
 
   Case 1 & 2 requires no additional implementation. In Case 1 & 4 branch prediction will change PC to BranchTarget in Decode stage and the next clock cycle Branch_Jump
   will change PC to same instruction. We assume implementation is done to avoid reexecuting same instruction. 
   
	Branch Prediction impact on Stalling: 
	In the case of branch instruction, it will predict branch is taken or not. In the event of a taken branch using branch
	prediction, we must flush the next instruction. For example in Case 3, we predicted a branch, and flushed the next instruction however,
	after the EX stage in BEQ we realized branch should not be taken, thus we must flush the previous branch instruction and refetch the ADD instruction.
	
	Data dependency can occur using branch prediction
	EX. If BEQ predicts to take branch and there is RAW dependency, then we must set PCmux = 0 which in turn fetches next instruction
	even though all branchs have PCmux = 1. We need to wait for R1 from ADD instruction after EX stage to forward new value to BEQ instruction
	and then it will compute the correct branch based on new R1. This will cause prevent branching using old R1 value and it will save
	the checking of resolving incorrect branch. The implementation of this will be done in Load_Stall unit and there should be an extra output for PCmuxOveride
	which should override PCmux from Control Unit. 
	
			ADD R1, R2, R5				|IF|D |E	|M	|W	|
			BEQ R1, R6	L1				|	|IF|D	|E |	|
			AND R6, R7, R8			   | 	|  |IF|D |X | 
			.								|	|	|	|IF|X	|
			.
			L1: SUB R5, R9, R10		|	|	|	|	|IF|										
	
  3. Finally, after integration of the branch predictor with your full system, take us
   through a “virtual tour” describing what happens when executing a branch
  instruction i.e. beq. In your tour, discuss what goes on at each stage of the
  pipeline trying to cover all possible cases i.e. branch is taken or not taken.
  
  Instructions: 							CASE 1:								CASE 2:								CASE 3:								CASE 4:
    BEQ R1, R2   L1					|IF|D	|E	|M	|W	|					|IF|D	|E	|M	|W	|				      |IF|D	|E	|M	|W	|			   	|IF|D	|E	|M	|W	|
	ADD R10, R11, R12				   |	|IF|D	|E	|M	|					|	|IF|D	|X	|X	|				      |	|IF|X	|X	|X	|				   |	|IF|X	|X	|X	|
	SUB R2, R3, R4					   |	|	|IF|D	|E	|					|	|	|IF|X	|X	|				      |	|	|	|	|	|				   |	|	|	|	|	|
	.								   |	|	|	|	|	|					|  |	|	|	|	|	|	|			   |	|	|	|	|	|				   |	|	|	|	|	|
	L1: ADDI R10, 100				   |	|	|	|	|	|					|  |	|	|	|IF|D	|	|			   |	|	|IF|X	|X	|				   |	|	|IF|D	|E	|
	
																				ADD R10, R11, R12			            |	|	|	|IF|D	|				

Monther: Please describe the four  cases in detail in terms of signals. Some signals of importance:
				1. NextPCPredict
				2. BranchPredictSel																				
				3. BranchMux
	Pipeline Case 1: 
			
 */
input	 	 [3:0] 	 BR;				
input 		 [31:0]  PCPlus4;
input		 [31:0]  IMM;
input		 		 BranchMux;				
output reg		 [31:0]  NextPCPredict;	
output reg				 BranchPredictSel;	

//output reg   PC_EN;					// Enable signal for PC
//output reg   IFID_EN;	

reg [1:0] BEQ_counter;
reg [1:0] BNE_counter;
reg [1:0] BLEZ_counter;
reg [1:0] BGEZ_counter;
reg [1:0] BGTZ_counter;
reg [1:0] BLTZ_counter;
reg [1:0] BLTZAL_counter;
reg [1:0] BGEZAL_counter;

always @ (BR)
	begin
		case (BR)
			
			4'h0: 						// BEQ
			  begin
					if (BEQ_counter[1] == 1) 
							NextPCPredict = PCPlus4 + IMM;
					else
							NextPCPredict = PCPlus4;
							
					BranchPredictSel = BEQ_counter[1];
			  end
			
			4'h1:						// BNE
			 begin
					if (BNE_counter[1] == 1) 
							NextPCPredict = PCPlus4 + IMM;
					else
							NextPCPredict = PCPlus4;
					
					BranchPredictSel = BNE_counter[1];
			  end
			
			4'h2:						// BLEZ
			 begin
					if (BLEZ_counter[1] == 1) 
							NextPCPredict = PCPlus4 + IMM;
					else
							NextPCPredict = PCPlus4;
					
					BranchPredictSel = BLEZ_counter[1];
			  end
			
			4'h3:						// BGEZ
			 begin
					if (BGEZ_counter[1] == 1) 
							NextPCPredict = PCPlus4 + IMM;
					else
							NextPCPredict = PCPlus4;
					
					BranchPredictSel = BGEZ_counter[1];
			  end
			
			4'h4:						// BGTZ
			 begin
					if (BGTZ_counter[1] == 1) 
							NextPCPredict = PCPlus4 + IMM;
					else
							NextPCPredict = PCPlus4;
					
					BranchPredictSel = BGTZ_counter[1];
			  end
			
			4'h5:						// BLTZ
			begin
					if (BLTZ_counter[1] == 1) 
							NextPCPredict = PCPlus4 + IMM;
					else
							NextPCPredict = PCPlus4;
					
					BranchPredictSel = BLTZ_counter[1];
			  end
			
			4'h6:						// BLTZAL
			begin
					if (BLTZAL_counter[1] == 1) 
							NextPCPredict = PCPlus4 + IMM;
					else
							NextPCPredict = PCPlus4;
					
					BranchPredictSel = BLTZAL_counter[1];
			  end
			
			4'h7:						// BGEZAL
			begin
					if (BGEZAL_counter[1] == 1) 
							NextPCPredict = PCPlus4 + IMM;
					else
							NextPCPredict = PCPlus4;
					
					BranchPredictSel = BGEZAL_counter[1];
			  end
			  
			  default: 
			  begin 
					  NextPCPredict = PCPlus4;  
					  BranchPredictSel = 0;
			  end

		endcase

	case (BR)
	
	4'h0: 								// BEQ
		begin
			if (BEQ_counter[1] == BranchMux)
					BEQ_counter = {2{BEQ_counter[1]}}; // If counter = 00 or 01 --> 00, if = 10 or 11 --> 11 (Strong take/not take) since our prediction is true
			else
				if (BEQ_counter[1] == 0)
					BEQ_counter = BEQ_counter + 1;
				else
					BEQ_counter = BEQ_counter - 1;
		end
	4'h1:						// BNE
		begin
			if (BNE_counter[1] == BranchMux)
					BNE_counter = {2{BNE_counter[1]}}; // If counter = 00 or 01 --> 00, if = 10 or 11 --> 11 (Strong take/not take) since our prediction is true
			else
				if (BNE_counter[1] == 0)
					BNE_counter = BNE_counter + 1;
				else
					BNE_counter = BNE_counter - 1;
		end
	4'h2:						// BLEZ
		begin
			if (BLEZ_counter[1] == BranchMux)
					BLEZ_counter = {2{BLEZ_counter[1]}}; // If counter = 00 or 01 --> 00, if = 10 or 11 --> 11 (Strong take/not take) since our prediction is true
			else
				if (BLEZ_counter[1] == 0)
					BLEZ_counter = BLEZ_counter + 1;
				else
					BLEZ_counter = BLEZ_counter - 1;
		end
	4'h3:						// BGEZ
		begin
			if (BGEZ_counter[1] == BranchMux)
					BGEZ_counter = {2{BGEZ_counter[1]}}; // If counter = 00 or 01 --> 00, if = 10 or 11 --> 11 (Strong take/not take) since our prediction is true
			else
				if (BLEZ_counter[1] == 0)
					BGEZ_counter = BGEZ_counter + 1;
				else
					BGEZ_counter = BGEZ_counter - 1;
		end
	4'h4:						// BGTZ
		begin
			if (BGTZ_counter[1] == BranchMux)
					BGTZ_counter = {2{BGTZ_counter[1]}}; // If counter = 00 or 01 --> 00, if = 10 or 11 --> 11 (Strong take/not take) since our prediction is true
			else
				if (BGTZ_counter[1] == 0)
					BGTZ_counter = BGTZ_counter + 1;
				else
					BGTZ_counter = BGTZ_counter - 1;
		end
	4'h5:						// BLTZ
		begin
			if (BLTZ_counter[1] == BranchMux)
					BLTZ_counter = {2{BLTZ_counter[1]}}; // If counter = 00 or 01 --> 00, if = 10 or 11 --> 11 (Strong take/not take) since our prediction is true
			else
				if (BGTZ_counter[1] == 0)
					BLTZ_counter = BLTZ_counter + 1;
				else
					BLTZ_counter = BLTZ_counter - 1;
		end
	4'h6:						// BLTZAL
		begin
			if (BLTZAL_counter[1] == BranchMux)
					BLTZAL_counter = {2{BLTZAL_counter[1]}}; // If counter = 00 or 01 --> 00, if = 10 or 11 --> 11 (Strong take/not take) since our prediction is true
			else
				if (BLTZAL_counter[1] == 0)
					BLTZAL_counter = BLTZAL_counter + 1;
				else
					BLTZAL_counter = BLTZAL_counter - 1;
		end
	4'h7:						// BGEZAL
		begin
			if (BGEZAL_counter[1] == BranchMux)
					BGEZAL_counter = {2{BGEZAL_counter[1]}}; // If counter = 00 or 01 --> 00, if = 10 or 11 --> 11 (Strong take/not take) since our prediction is true
			else	
				if (BGEZAL_counter[1] == 0)
					BGEZAL_counter = BGEZAL_counter + 1;
				else
					BGEZAL_counter = BGEZAL_counter - 1;	
		end
		
		default: break;
		
	endcase

	end

endmodule
