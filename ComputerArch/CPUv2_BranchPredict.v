/********************************************************
	King Abdullah University of Science and Technology
	CS 282 Project: CPUv2.v
	Spring 2013
	
	Please fill your names here:
	Group Member 1: Monther Busbait
	Group Member 2: Shahzeb Siddiqui
	
	WARNING: Only add code in allowed sections. 
********************************************************/

module CPU(CLK, RESET, Iin, Din, Iaddr, req_i_valid, DMCout, Daddr, req_d_valid, Dout);

input			CLK;			// Global Clock
input			RESET;			// Global Reset (Active High)
input	[31:0]	Iin;			// Instruction input from external memory
input	[31:0]	Din;			// Data input from external memory
output	[31:0]	Iaddr;			// Address from which Instruction is fetched
output			req_i_valid;	// Instruction memory request valid bit
output	 [1:0]	DMCout;			// Data Memory Controller
output	[31:0]	Daddr;			// Address to/from which Data must be written/read
output			req_d_valid;	// Data memory request valid bit
output	[31:0]	Dout;			// Data output to external memory

/********************************************************
	WIRE DECLARATIONS
********************************************************/

// Declarations have already been completed for you.
// You should not need any more declarations than the ones provided.
// Please follow the naming convention in the Datapath Diagram. 
// You should not need any 'always' blocks in this module. 

wire	[31:0]	PC_IF, PC_ID, PC_EX, PC_MEM, PC_WB;				// PC
wire	[31:0]	INS_IF, INS_ID, INS_EX, INS_MEM, INS_WB;		// Instruction
wire			PCmux_ID, PCmux_EX;								// Selector for Mux Before PC register
wire	[31:0]	nextPC_IF;										// Output of the PC Mux
wire	[31:0]	Branch_NextPC_EX;								// New PC Value in case of Branch
wire			BranchMux_EX;									// Selector to choose between Branch/No Branch
wire	[31:0]	PCplus4_IF, PCplus4_ID, PCplus4_EX;				// PC + 4

wire	 [4:0]	RSaddr_ID, RSaddr_EX;							// Read Address 1
wire	 [4:0]	RTaddr_ID, RTaddr_EX;							// Read Address 2
wire 	 [4:0]	RDaddr_ID, RDaddr_EX, RDaddr_MEM, RDaddr_WB;	// Write Address
wire	[31:0]	RSbus_ID, RSbus_EX;								// Output A of Register File
wire	[31:0]	RTbus_ID, RTbus_EX;								// Output B of Register File
wire	[31:0]	RDbus_WB;										// Data to be written back to Register File
wire			WE_ID, WE_EX, WE_MEM, WE_WB;					// Write Enable for Register File
wire			WBmux_ID, WBmux_EX, WBmux_MEM, WBmux_WB;		// Selector for Mux in Write-Back Stage
wire	 [1:0]	RDSEL_ID;										// Register Store address Selector for Register File

//########### Added Signals for CPUv2
wire	[31:0]	FWD_A_EX;										// A value i.e. RSbus, after Forwarding
wire	[31:0]	FWD_B_EX;										// B value i.e. RTbus, after Forwarding
wire	 [1:0]	sFORA_EX;										// Selector for Forwarding Mux of Data from Register A
wire	 [1:0]	sFORB_EX;										// Selector for Forwarding Mux of Data from Register B

wire    		Stall;											// Stall signal from Load_Stall unit
wire			PC_EN;											// Enable signal for the PC register						
wire			IFID_EN;										// Enable signal for IFID register
//##########

wire	[31:0]	SHAMT_ID, SHAMT_EX;								// Shift Amount
wire	 [1:0] 	ASEL_ID, ASEL_EX;								// Selector for Input A for ALU
wire	 	 	BSEL_ID, BSEL_EX;								// Selector for Input B for ALU
wire	[31:0]	ALU_A_EX;										// Input A to the ALU
wire	[31:0] 	ALU_B_EX;										// Input B to the ALU
wire	[31:0]	ALU_OUT_EX;										// Output of the ALU
wire	 [3:0] 	ALUC_ID, ALUC_EX;								// ALU Control Input
wire	[31:0]	ALUR_EX, ALUR_MEM, ALUR_WB;						// Final Output of EX Stage

wire	 [3:0]	BR_ID, BR_EX;									// Branch Control Unit Selector
wire	[25:0]	TARGET_ID, TARGET_EX;							// Target Value for Jumps
wire			IMM_ctrl_ID;									// Selector for Sign/Zero extension of Immediate
wire	[31:0]	IMM_ID, IMM_EX;									// Extended Immediate Field
wire	[31:0]	PCplus8_EX;										// PC + 8
wire			ALUMux_EX;										// Selector for MUX after ALU	
wire	[31:0]	BranchJump_EX;									// After the Branch Mux in the EX stage

wire	 [2:0]  Lmode_ID, Lmode_EX, Lmode_MEM;					// Loading Mode for Load Parse Block
wire	 [1:0]	DMC_ID, DMC_EX, DMC_MEM;						// Data Memory Controller Signal
wire	[31:0]	MDATA_MEM, MDATA_WB;							// Memory Data Result After Load Parse
wire	[31:0]	WDATA_EX, WDATA_MEM;							// Data to be written to Memory

//************* Added Wires for BranchPrediction
wire    [31:0] BR_Predict_Addr;
wire		   BR_Predict_Sel;
wire 	[31:0] FinalBranchJumpAddr;
/********************************************************
	IF: INSTRUCTION FETCH STAGE
********************************************************/

adder PCp4
	(
	.A		(PC_IF),
	.B		(32'h4),
	.Out	(PCplus4_IF)
	);

mux_2_1_32 PC_mux
	(
	.sel	(PCmux_EX),
	.A		(PCplus4_IF),
	.B		(FinalBranchJumpAddr),
	.Out	(nextPC_IF)
	);

PCv2 ProgramCounter
	(
	.CLK	(CLK),
	.RESET	(RESET),
	.EN		(PC_EN),
	.PCin		(nextPC_IF),
	.PCout	(PC_IF)
	);

assign Iaddr 		= PC_IF;
assign INS_IF 		= Iin;
assign req_i_valid 	= 1'b1;

/********************************************************
	IF-ID Pipeline Registers
********************************************************/

PosRegsEN	#(32)	 PC_IFID(CLK, RESET, IFID_EN, PC_IF, PC_ID);
PosRegsEN	#(32)	INS_IFID(CLK, RESET, IFID_EN, INS_IF, INS_ID);
PosRegsEN	#(32)	PC4_IFID(CLK, RESET, IFID_EN, PCplus4_IF, PCplus4_ID);


/********************************************************
	ID: INSTRUCTION DECODE STAGE
********************************************************/

Register_File RF
	(
	.CLK 	(CLK), 
	.RESET 	(RESET), 
	.RS		(RSaddr_ID), 
	.RT		(RTaddr_ID), 
	.RD		(RDaddr_WB), 
	.WE		(WE_WB), 
	.A		(RSbus_ID), 
	.B		(RTbus_ID), 
	.W		(RDbus_WB)
	);

Control_UnitV2 CU
	(
	.INS		(INS_ID), 
	.Stall		(Stall),
	.BR			(BR_ID), 
	.ALUC		(ALUC_ID), 
	.Lmode		(Lmode_ID), 
	.DMC		(DMC_ID), 
	.RDSEL		(RDSEL_ID), 
	.ASEL		(ASEL_ID), 
	.BSEL		(BSEL_ID), 
	.WBmux		(WBmux_ID), 
	.WE			(WE_ID), 
	.PCmux		(PCmux_ID), 
	.IMM_ctrl	(IMM_ctrl_ID)
	);
	
assign RSaddr_ID = INS_ID[25:21];
assign RTaddr_ID = INS_ID[20:16];
assign TARGET_ID = INS_ID[25:0];

z_ext_32 ShiftAmt
	(
	.In		(INS_ID[10:6]),
	.Out	(SHAMT_ID)
	);

imm_ext_32 Immediate_Extend
	(
	.sel	(IMM_ctrl_ID),
	.In		(INS_ID[15:0]),
	.Out	(IMM_ID)
	);

mux_3_1_05 Sel_RD
	(
	.sel	(RDSEL_ID),
	.A		(INS_ID[15:11]),
	.B		(INS_ID[20:16]),
	.C		(5'b11111),
	.Out	(RDaddr_ID)
	);
// new Load_Stall with BranchPrediction Signal
Load_Stall_v2 LStallv2
	(
	.RDaddr_EX(RDaddr_EX), 
	.RSaddr_ID(RSaddr_ID), 
	.RTaddr_ID(RTaddr_ID), 
	.WE_EX(WE_EX),
	.DMC_EX(DMC_EX),
	.WBmux_EX(WBmux_EX), 
	.BranchPredictSel(BR_Predict_Sel),
	.Stall(Stall), 
	.PC_EN(PC_EN), 
	.IFID_EN(IFID_EN)
	);
// Added Module: Branch Predictor	
BranchPredictor	BrPredict
		(
		.BR					(BR_ID),
		.PCPlus4 			(PCplus4_ID),
		.IMM				(IMM_ID),
		.BranchMux			(BranchMux_EX),
		.NextPCPredict		(BR_Predict_Addr),	
		.BranchPredictSel	(BR_Predict_Sel)
		);
// Added Module: Final Mux for BranchJumpPrediction Address Line			
BranchPredict_BranchJumpSel	FinalBranchJumpPredictMux
			(
				.BranchPredictTarget	(BR_Predict_Addr),
				.BranchJumpTarget		(Branch_NextPC_EX),
				.BranchMux				(BranchMux_EX),
				.BranchPredictSel		(BR_Predict_Sel),
				.PCmux_ID				(PCmux_ID),
				.TargetAddress			(FinalBranchJumpAddress)
			);


/********************************************************
	ID-EX Pipeline Registers
********************************************************/

PosRegsEN	#(32)	   PC_IDEX(CLK, RESET, 1'b1, PC_ID, PC_EX);
PosRegsEN	#(32)	  INS_IDEX(CLK, RESET, 1'b1, INS_ID, INS_EX);
PosRegsEN	#(32)	  PC4_IDEX(CLK, RESET, 1'b1, PCplus4_ID, PCplus4_EX);
PosRegsEN	#(32)	   RS_IDEX(CLK, RESET, 1'b1, RSbus_ID, RSbus_EX);
PosRegsEN	#(32)	   RT_IDEX(CLK, RESET, 1'b1, RTbus_ID, RTbus_EX);
PosRegsEN	#(32)	  IMM_IDEX(CLK, RESET, 1'b1, IMM_ID, IMM_EX);
PosRegsEN	#(32)	SHAMT_IDEX(CLK, RESET, 1'b1, SHAMT_ID, SHAMT_EX);
PosRegsEN	#(26)	  TGT_IDEX(CLK, RESET, 1'b1, TARGET_ID, TARGET_EX);
PosRegsEN	#( 5)	 RSad_IDEX(CLK, RESET, 1'b1, RSaddr_ID, RSaddr_EX);
PosRegsEN	#( 5)	 RTad_IDEX(CLK, RESET, 1'b1, RTaddr_ID, RTaddr_EX);
PosRegsEN	#( 5)	 RDad_IDEX(CLK, RESET, 1'b1, RDaddr_ID, RDaddr_EX);
PosRegsEN	#( 2)	 ASEL_IDEX(CLK, RESET, 1'b1, ASEL_ID, ASEL_EX);
PosRegsEN	#( 1)	 BSEL_IDEX(CLK, RESET, 1'b1, BSEL_ID, BSEL_EX);
PosRegsEN	#( 4)	   BR_IDEX(CLK, RESET, 1'b1, BR_ID, BR_EX);
PosRegsEN	#( 4)	 ALUC_IDEX(CLK, RESET, 1'b1, ALUC_ID, ALUC_EX);
PosRegsEN	#( 2)	  DMC_IDEX(CLK, RESET, 1'b1, DMC_ID, DMC_EX);
PosRegsEN	#( 1)	WBmux_IDEX(CLK, RESET, 1'b1, WBmux_ID, WBmux_EX);
PosRegsEN	#( 1)	   WE_IDEX(CLK, RESET, 1'b1, WE_ID, WE_EX);
PosRegsEN	#( 3)	Lmode_IDEX(CLK, RESET, 1'b1, Lmode_ID, Lmode_EX);
PosRegsEN	#( 1)	PCmux_IDEX(CLK, RESET, 1'b1, PCmux_ID, PCmux_EX);

/********************************************************
	EX: EXECUTION STAGE
********************************************************/

Branch_Jump BrJu_Ctrl
	(
	.PCplus4		(PCplus4_EX),
	.TARGET			(TARGET_EX), 
	.A				(FWD_A_EX), 
	.B				(FWD_B_EX),
	.IMM			(IMM_EX),
	.BR				(BR_EX),
	.PCplus8		(PCplus8_EX), 
	.ALUMux 		(ALUMux_EX),
	.BranchMux		(BranchMux_EX), 
	.Branch_NextPC	(Branch_NextPC_EX)
	);

MIPS_ALU ALU
	(
	.aluc	(ALUC_EX),
	.A		(ALU_A_EX),
	.B		(ALU_B_EX),
	.OUT	(ALU_OUT_EX)
	);

Forward	FU
	(
	.RA_EX	(RSaddr_EX), 
	.RB_EX	(RTaddr_EX), 
	.RD_MEM	(RDaddr_MEM), 
	.RD_WB	(RDaddr_WB), 
	.WE_MEM	(WE_MEM), 
	.WE_WB	(WE_WB), 
	.sFORA	(sFORA_EX), 
	.sFORB	(sFORB_EX)
	);

// YOUR CODE BEGINS HERE.
assign WDATA_EX = FWD_B_EX; //Redundant Definition to Comply with Names

//New Component for selecting forwarded A
mux_3_1_32	fwdA
	(
	.A		(RSaddr_EX),
	.B		(ALUR_MEM),
	.C		(RDbus_WB),
	.sel	(sFORA_EX),
	.Out	(FWD_A_EX)
	);

//New Component for selecting forwarded B
mux_3_1_32	fwdB
	(
	.A		(RTaddr_EX),
	.B		(ALUR_MEM),
	.C		(RDbus_WB),
	.sel	(sFORB_EX),
	.Out	(FWD_B_EX)
	);

mux_3_1_32	muxA
	(
	.A		(FWD_A_EX),
	.B		(SHAMT_EX),
	.C		(32'b10000),
	.sel	(ASEL_EX),
	.Out	(ALU_A_EX)
	);
mux_2_1_32	muxB
	(
	.A		(FWD_B_EX),
	.B		(IMM_EX),	
	.sel	(BSEL_EX),
	.Out	(ALU_B_EX)
	);
mux_2_1_32	BranchJumpPCmux
	(
	.A		(PCplus4_ID),
	.B		(Branch_NextPC_EX),
	.sel	(BranchMux_EX),
	.Out	(BranchJump_EX)
	);
mux_2_1_32	BranchJumpALUmux
	(
	.A		(ALU_OUT_EX),
	.B		(PCplus8_EX),
	.sel	(ALUMux_EX),
	.Out	(ALUR_EX)
	);
// YOUR CODE ENDS HERE.

/********************************************************
	EX-MEM Pipeline Registers
********************************************************/
PosRegsEN	#(32)	   PC_EXMEM(CLK, RESET, 1'b1, PC_EX, PC_MEM);
PosRegsEN	#(32)	  INS_EXMEM(CLK, RESET, 1'b1, INS_EX, INS_MEM);
PosRegsEN	#(32)	 ALUR_EXMEM(CLK, RESET, 1'b1, ALUR_EX, ALUR_MEM);
PosRegsEN	#(32)	WDATA_EXMEM(CLK, RESET, 1'b1, WDATA_EX, WDATA_MEM);
PosRegsEN	#( 5)	 RDad_EXMEM(CLK, RESET, 1'b1, RDaddr_EX, RDaddr_MEM);
PosRegsEN	#( 2)	  DMC_EXMEM(CLK, RESET, 1'b1, DMC_EX, DMC_MEM);
PosRegsEN	#( 1)	   WE_EXMEM(CLK, RESET, 1'b1, WE_EX, WE_MEM);
PosRegsEN	#( 1)	WBmux_EXMEM(CLK, RESET, 1'b1, WBmux_EX, WBmux_MEM);
PosRegsEN	#( 3)	Lmode_EXMEM(CLK, RESET, 1'b1, Lmode_EX, Lmode_MEM);

/********************************************************
	MEM: MEMORY ACCESS STAGE
********************************************************/
assign req_d_valid 	= 1'b1;
assign Dout 		= WDATA_MEM;
assign Daddr 		= ALUR_MEM;
assign DMCout 		= DMC_MEM;

Load_Parse LP
	(
	.DataIn		(Din),
	.WData_In	(WDATA_MEM),
	.Addr		(ALUR_MEM),
	.Mode		(Lmode_MEM), 
	.Out		(MDATA_MEM)
	);

/********************************************************
	MEM-WB Pipeline Registers
********************************************************/

PosRegsEN	#(32)	   PC_MEMWB(CLK, RESET, 1'b1, PC_MEM, PC_WB);
PosRegsEN	#(32)	  INS_MEMWB(CLK, RESET, 1'b1, INS_MEM, INS_WB);
PosRegsEN	#(32)	 ALUR_MEMWB(CLK, RESET, 1'b1, ALUR_MEM, ALUR_WB);
PosRegsEN	#(32)	MDATA_MEMWB(CLK, RESET, 1'b1, MDATA_MEM, MDATA_WB);
PosRegsEN	#( 5)	 RDad_MEMWB(CLK, RESET, 1'b1, RDaddr_MEM, RDaddr_WB);
PosRegsEN	#( 1)	WBmux_MEMWB(CLK, RESET, 1'b1, WBmux_MEM, WBmux_WB);
PosRegsEN	#( 1)	   WE_MEMWB(CLK, RESET, 1'b1, WE_MEM, WE_WB);

/********************************************************
	WB: WRITE BACK STAGE
********************************************************/
mux_2_1_32 RegFile_Input
	(
	.sel	(WBmux_WB),
	.A		(MDATA_WB),
	.B		(ALUR_WB),
	.Out	(RDbus_WB)
	);	

endmodule
