/********************************************************
	King Abdullah University of Science and Technology
	CS 282 Project: mips.h
	Spring 2013
	
	Declaration List for Control Unit.
	
	This is just to help in using names instead of binary code. 
	
	DO NOT MODIFY.
********************************************************/

/********************************************************
	Opcode Values: (INS[31:26])
********************************************************/

`define	SPECIAL		6'b000000
`define	REGIMM		6'b000001
`define	J			6'b000010
`define	JAL			6'b000011
`define	BEQ			6'b000100
`define	BNE			6'b000101
`define	BLEZ		6'b000110
`define	BGTZ		6'b000111

`define	ADDI		6'b001000
`define	ADDIU		6'b001001
`define	SLTI		6'b001010
`define	SLTIU		6'b001011
`define	ANDI		6'b001100
`define	ORI			6'b001101
`define	XORI		6'b001110
`define	LUI			6'b001111

`define	LB			6'b100000
`define	LH			6'b100001
`define	LWL			6'b100010
`define	LW			6'b100011
`define	LBU			6'b100100
`define	LHU			6'b100101
`define	LWR			6'b100110
`define	SB			6'b101000
`define	SH			6'b101001
`define	SW			6'b101011

/********************************************************
	Function field values in case of SPECIAL (INS[5:0])
********************************************************/

`define	SLL			6'b000000
`define	SRL			6'b000010
`define	SRA			6'b000011
`define	SLLV		6'b000100
`define	SRLV		6'b000110
`define	SRAV		6'b000111
`define	JR			6'b001000
`define	JALR		6'b001001
`define	ADD			6'b100000
`define	ADDU		6'b100001
`define	SUB			6'b100010
`define	SUBU		6'b100011
`define	AND			6'b100100
`define	OR			6'b100101
`define	XOR			6'b100110
`define	NOR			6'b100111
`define	SLT			6'b101010
`define	SLTU		6'b101011

/********************************************************
	BR_type Values for REGIMM instructions: 
********************************************************/

`define	BLTZ		5'b00000
`define	BGEZ		5'b00001
`define	BLTZAL		5'b10000
`define	BGEZAL		5'b10001

/********************************************************
	Miscellaneous 
********************************************************/

// Various width don't care and invalid values	
`define	dc26		26'bx
`define	dc16		16'bx
`define	dc6			6'bxxxxxx
`define	dc5			5'bxxxxx
`define ze5			5'b0
