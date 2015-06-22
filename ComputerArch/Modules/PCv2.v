/********************************************************
	King Abdullah University of Science and Technology
	CS 282 Project: PCv2.v
	Module Code: DM007
	Spring 2013
	
	Please fill your names here:
	Group Member 1: YourFirstName1 YourLastName1
	Group Member 2: YourFirstName2 YourLastName2
	
	WARNING: DO NOT MODIFY. 
********************************************************/

module PCv2(CLK, EN, RESET, PCin, PCout);
input			CLK, RESET, EN;
input 	[31:0]	PCin;
output	[31:0]	PCout;


NegRegsEN #(32) pc_reg(.CLK(CLK),
                       .RESET(RESET),  
                       .EN(EN),		 
                       .In(PCin),		 
                       .Out(PCout)    
					   );





endmodule
