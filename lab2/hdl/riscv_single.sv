// riscvsingle.sv

// RISC-V single-cycle processor
// From Section 7.6 of Digital Design & Computer Architecture
// 27 April 2020
// David_Harris@hmc.edu 
// Sarah.Harris@unlv.edu

// run 210
// Expect simulator to print "Simulation succeeded"
// when the value 25 (0x19) is written to address 100 (0x64)

//   Instruction  opcode    funct3    funct7
//   add          0110011   000       0000000
//   sub          0110011   000       0100000
//   and          0110011   111       0000000
//   or           0110011   110       0000000
//   slt          0110011   010       0000000
//   addi         0010011   000       immediate
//   andi         0010011   111       immediate
//   ori          0010011   110       immediate
//   slti         0010011   010       immediate
//   beq          1100011   000       immediate
//   lw	          0000011   010       immediate
//   sw           0100011   010       immediate
//   jal          1101111   immediate immediate

module testbench();

   logic        clk;
   logic        reset;

   logic [31:0] WriteData;
   logic [31:0] DataAdr;
   logic        MemWrite;

   // instantiate device to be tested
   top dut(clk, reset, WriteData, DataAdr, MemWrite);

   initial
     begin
	string memfilename;
        //memfilename = {"../riscvtest/riscvtest.memfile"};
        //memfilename = {"../othertests/lui.memfile"};
        memfilename = {"../lab1tests/bgeu.memfile"};
        $readmemh(memfilename, dut.imem.RAM);
     end

   
   // initialize test
   initial
     begin
	reset <= 1; # 22; reset <= 0;
     end

   // generate clock to sequence tests
   always
     begin
	clk <= 1; # 5; clk <= 0; # 5;
     end

   // check results
   always @(negedge clk)
     begin
	if(MemWrite) begin
           if(DataAdr === 100 & WriteData === 25) begin
              $display("Simulation succeeded");
              $stop;
           end else if (DataAdr !== 96) begin
              $display("Simulation failed");
              $stop;
           end
	end
     end
endmodule // testbench

module riscvsingle (input  logic        clk, reset,
		    output logic [31:0] PC,
		    input  logic [31:0] Instr,
		    output logic 	MemWrite,
		    output logic [31:0] ALUResult, WriteData,
		    input  logic [31:0] ReadData,
        output logic [3:0]  ByteMask);
   
   logic 				ALUSrc, RegWrite, Jump, Zero, Negative, Overflow; //negative and overflow added by us
   logic [1:0] 				ResultSrc;
   logic [2:0]        ImmSrc;
   logic [3:0] 				ALUControl;

   logic              AddPC;

   controller c (Instr[6:0], Instr[14:12], Instr[30], Zero, Negative, Overflow,
		 ResultSrc, MemWrite, PCSrc,
		 ALUSrc, RegWrite, Jump,
		 ImmSrc, ALUControl,AddPC);
   datapath dp (clk, reset, ResultSrc, PCSrc,
		ALUSrc, RegWrite,
		ImmSrc, ALUControl,
		Zero, Negative, Overflow, PC, Instr,
		ALUResult, WriteData, ReadData,AddPC,ByteMask);
   
endmodule // riscvsingle

module controller (input  logic [6:0] op,
		   input  logic [2:0] funct3,
		   input  logic       funct7b5,
		   input  logic       Zero, Negative, Overflow,
		   output logic [1:0] ResultSrc,
		   output logic       MemWrite,
		   output logic       PCSrc, ALUSrc,
		   output logic       RegWrite, Jump,
		   output logic [2:0] ImmSrc,
		   output logic [3:0] ALUControl,
       output logic       AddPC);
   
   logic [1:0] 			      ALUOp;
   logic 			      Branch;
   logic            BranchControl;

    //TODO: this flag system might be too x86 like, this isnt x86 silly!!
   always_comb //TODO: might change from switch statement to more condensed logic, use ~funct3[0] for exlusive instruction pairs beq, bne etc
    case(funct3)
    3'b000: BranchControl = Zero;                     //beq
    3'b001: BranchControl = ~Zero;                    //bne
    3'b100: BranchControl = ~(Negative == Overflow);  //blt
    3'b101: BranchControl = Negative == Overflow;     //bge
    3'b110: BranchControl = (Negative | Overflow);    //bltu
    3'b111: BranchControl = (~Negative & ~Overflow) | Zero;   //bgeu
    default: BranchControl = 1'b0;
    endcase
   
   maindec md (op, ResultSrc, MemWrite, Branch,
	       ALUSrc, RegWrite, Jump, ImmSrc, ALUOp);
   aludec ad (op[5], funct3, funct7b5, ALUOp, ALUControl);
   assign PCSrc = Branch & BranchControl | Jump;
   assign AddPC = (op == 7'b0010111); //for auipc
   
endmodule // controller

module maindec (input  logic [6:0] op,
		output logic [1:0] ResultSrc,
		output logic 	   MemWrite,
		output logic 	   Branch, ALUSrc,
		output logic 	   RegWrite, Jump,
		output logic [2:0] ImmSrc,
		output logic [1:0] ALUOp);
   
   logic [11:0] 		   controls;
   
   assign {RegWrite, ImmSrc, ALUSrc, MemWrite,
	   ResultSrc, Branch, ALUOp, Jump} = controls;
   
   always_comb
     case(op)
       // RegWrite_ImmSrc_ALUSrc_MemWrite_ResultSrc_Branch_ALUOp_Jump
       7'b0000011: controls = 12'b1_000_1_0_01_0_00_0; // lw
       7'b0100011: controls = 12'b0_001_1_1_00_0_00_0; // sw
       7'b0110011: controls = 12'b1_xxx_0_0_00_0_10_0; // R–type
       7'b1100011: controls = 12'b0_010_0_0_00_1_01_0; // B-type
       7'b0010011: controls = 12'b1_000_1_0_00_0_10_0; // I–type ALU
       7'b1101111: controls = 12'b1_011_0_0_10_0_00_1; // jal
       7'b1100111: controls = 12'b1_000_1_0_10_0_00_1; // jalr
       7'b0110111: controls = 12'b1_100_1_0_00_0_11_0; // lui
       7'b0010111: controls = 12'b1_100_1_0_00_0_00_0; // auipc
       default: controls = 11'bx_xx_x_x_xx_x_xx_x; // ???
     endcase // case (op)
   
endmodule // maindec

module aludec (input  logic       opb5,
	       input  logic [2:0] funct3,
	       input  logic 	  funct7b5,
	       input  logic [1:0] ALUOp,
	       output logic [3:0] ALUControl);
   
   logic 			  RtypeSub;
   
   assign RtypeSub = funct7b5 & opb5; // TRUE for R–type subtract
   always_comb
     case(ALUOp)
       2'b00: ALUControl = 4'b0000; // addition
       2'b01: ALUControl = 4'b0001; // subtraction
       2'b10: case(funct3) // R–type or I–type ALU
		  3'b000: if (RtypeSub)
		    ALUControl = 4'b0001; // sub
		  else
		    ALUControl = 4'b0000; // add, addi
      
      3'b001: ALUControl = 4'b0110; // sll
      3'b010: ALUControl = 4'b0101; // slt, slti
      3'b011: ALUControl = 4'b0111; // sltu
      3'b100: ALUControl = 4'b0100; // xor
      3'b101: ALUControl = funct7b5 ? 4'b1001 : 4'b1000; // sra, srl
		  3'b110: ALUControl = 4'b0011; // or, ori
		  3'b111: ALUControl = 4'b0010; // and, andi

		  default: ALUControl = 4'bxxxx; // ???
		endcase // case (funct3) 
    default: ALUControl = 4'b1010;   //2'b11, other functions      
     endcase // case (ALUOp)
   
endmodule // aludec

module datapath (input  logic        clk, reset,
		 input  logic [1:0]  ResultSrc,
		 input  logic 	     PCSrc, ALUSrc,
		 input  logic 	     RegWrite,
		 input  logic [2:0]  ImmSrc,
		 input  logic [3:0]  ALUControl,
		 output logic 	     Zero, Negative, Overflow,
		 output logic [31:0] PC,
		 input  logic [31:0] Instr,
		 output logic [31:0] ALUResult, WriteData,
		 input  logic [31:0] ReadData,
     input  logic        AddPC,
     output logic [3:0]  ByteMask);
   
   logic [31:0] 		     PCNext, PCPlus4, PCTarget, PCTargetSrcA; //added by us
   logic [31:0] 		     ImmExt;
   logic [31:0]          RD1;
   logic [31:0] 		     SrcA, SrcB;
   logic [31:0] 		     Result;
   logic [31:0]          ToWrite; //added by us
   logic [31:0]          FromRead; //added by us
   
   // next PC logic
   flopr #(32) pcreg (clk, reset, PCNext, PC);
   adder  pcadd4 (PC, 32'd4, PCPlus4);
   adder  pcaddbranch (PCTargetSrcA, ImmExt, PCTarget);
   mux2 #(32)  pcmux (PCPlus4, PCTarget, PCSrc, PCNext);
   mux2 #(32)  pctargetmux(PC, SrcA, ALUSrc, PCTargetSrcA); //added by us
   // register file logic
   regfile  rf (clk, RegWrite, Instr[19:15], Instr[24:20],
	       Instr[11:7], Result, RD1, ToWrite);
   extend  ext (Instr[31:7], ImmSrc, ImmExt);
   // ALU logic
   mux2 #(32)  srcamux(RD1, PC, AddPC, SrcA); //added by us
   mux2 #(32)  srcbmux (ToWrite, ImmExt, ALUSrc, SrcB);
   alu  alu (SrcA, SrcB, ALUControl, ALUResult, Zero, Negative, Overflow);
   mux3 #(32) resultmux (ALUResult, FromRead, PCPlus4,ResultSrc, Result);
   // Read/WriteLogic
   subwordwrite sww(ToWrite,Instr[13:12],ALUResult[1:0],WriteData,ByteMask); //added by us
   subwordread swr(ReadData,Instr[14:12],ALUResult[1:0],FromRead);  //added by us

endmodule // datapath

module adder (input  logic [31:0] a, b,
	      output logic [31:0] y);
   
   assign y = a + b;
   
endmodule

module extend (input  logic [31:7] instr,
	       input  logic [2:0]  immsrc,
	       output logic [31:0] immext);
   
   always_comb
     case(immsrc)
       // I−type
       3'b000:  immext = {{20{instr[31]}}, instr[31:20]};
       // S−type (stores)
       3'b001:  immext = {{20{instr[31]}}, instr[31:25], instr[11:7]};
       // B−type (branches)
       3'b010:  immext = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};       
       // J−type (jal)
       3'b011:  immext = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};
       //U-type (lui and auipc)
       3'b100:  immext = {instr[31:12], 12'b0};
       default: immext = 32'bx; // undefined
     endcase // case (immsrc)
   
endmodule // extend

module flopr #(parameter WIDTH = 8)
   (input  logic             clk, reset,
    input logic [WIDTH-1:0]  d,
    output logic [WIDTH-1:0] q);
   
   always_ff @(posedge clk, posedge reset)
     if (reset) q <= 0;
     else  q <= d;
   
endmodule // flopr

module flopenr #(parameter WIDTH = 8)
   (input  logic             clk, reset, en,
    input logic [WIDTH-1:0]  d,
    output logic [WIDTH-1:0] q);
   
   always_ff @(posedge clk, posedge reset)
     if (reset)  q <= 0;
     else if (en) q <= d;
   
endmodule // flopenr

module mux2 #(parameter WIDTH = 8)
   (input  logic [WIDTH-1:0] d0, d1,
    input logic 	     s,
    output logic [WIDTH-1:0] y);
   
  assign y = s ? d1 : d0;
   
endmodule // mux2

module mux3 #(parameter WIDTH = 8)
   (input  logic [WIDTH-1:0] d0, d1, d2,
    input logic [1:0] 	     s,
    output logic [WIDTH-1:0] y);
   
  assign y = s[1] ? d2 : (s[0] ? d1 : d0);
   
endmodule // mux3

module top (input  logic        clk, reset,
	    output logic [31:0] WriteData, DataAdr,
	    output logic 	MemWrite);
   
   logic [31:0] 		PC, Instr, ReadData;
   logic [3:0]      ByteMask; //added by us
   
   // instantiate processor and memories
   riscvsingle rv32single (clk, reset, PC, Instr, MemWrite, DataAdr,
			   WriteData, ReadData,ByteMask);
   imem imem (PC, Instr);
   dmem dmem (clk, MemWrite, DataAdr, WriteData, ByteMask, ReadData);
   
endmodule // top

module imem (input  logic [31:0] a,
	     output logic [31:0] rd);
   
   //logic [31:0] 		 RAM[63:0];
   logic [31:0] 		 RAM[2047:0];
   
   assign rd = RAM[a[31:2]]; // word aligned
   
endmodule // imem

module dmem (input  logic        clk, we,
	     input  logic [31:0] a, wd,
       input  logic [3:0] ByteMask, //added by us
	     output logic [31:0] rd);
   
   logic [31:0] 		 RAM[255:0];
   logic [31:0]      BitMask; //added by us

   assign BitMask = {{8{ByteMask[3]}},{8{ByteMask[2]}},{8{ByteMask[1]}},{8{ByteMask[0]}}};
   
   assign rd = RAM[a[31:2]]; // word aligned
   /*always_ff @(posedge clk)
     if (we) RAM[a[31:2]] <= wd;*/
    always_ff @(posedge clk)
      if (we) RAM[a[31:2]] <= (rd & ~BitMask) | wd;
   
endmodule // dmem

module subwordwrite(input   logic [31:0]  ToWrite,  //added by us
                    input   logic [1:0]   Funct3_2, ByteAdr,
                    output  logic [31:0]  WriteData,
                    output  logic [3:0]   ByteMask);
    
    always_comb begin
      case(Funct3_2)
        2'b00: WriteData = {4{ToWrite[7:0]}};
        2'b01: WriteData = {2{ToWrite[15:0]}};
        2'b10: WriteData = ToWrite;
        default: WriteData = ToWrite;
      endcase

      case({Funct3_2,ByteAdr})
        4'b00_00: ByteMask = 4'b0001;
        4'b00_01: ByteMask = 4'b0010;
        4'b00_10: ByteMask = 4'b0100;
        4'b00_11: ByteMask = 4'b1000;
        4'b01_0x: ByteMask = 4'b0011;
        4'b01_1x: ByteMask = 4'b1100;
        4'b10_xx: ByteMask = 4'b1111;
        default:  ByteMask = 4'b1111;
      endcase
    end
                
endmodule

module subwordread(input  logic [31:0]  ReadData, //added by us
                   input  logic [2:0]   Funct3, 
                   input  logic [1:0]   ByteAdr,
                   output logic [31:0]  FromRead);
              
    logic [7:0]   Byte;
    logic [15:0]  Halfword;
    logic [31:0]  Word;

    always_comb begin
      case(ByteAdr)
      2'b00: Byte = ReadData[7:0];
      2'b01: Byte = ReadData[15:8];
      2'b10: Byte = ReadData[23:16];
      2'b11: Byte = ReadData[31:24];
      default: Byte = 8'hxx;
      endcase
      Halfword = (ByteAdr[1]) ? ReadData[31:16] : ReadData[15:0];
      Word = ReadData;

      case(Funct3)
      3'b000: FromRead = {{24{Byte[7]}},Byte};          //lb
      3'b100: FromRead = {{24{1'b0}},Byte};             //lbu
      3'b001: FromRead = {{16{Halfword[15]}},Halfword}; //lh
      3'b101: FromRead = {{16{1'b0}},Halfword};         //lhu
      3'b010: FromRead = Word;                          //lw
      default: FromRead = 32'hxxxxxxxx;
      endcase
    end

endmodule

module alu (input  logic [31:0] a, b,
            input  logic [3:0] 	alucontrol,
            output logic [31:0] result,
            output logic 	zero, negative, overflow);

   logic [31:0] 	       condinvb, sum;
   logic 		       v;              // overflow
   logic 		       isAddSub;       // true when is add or subtract operation
   logic signed [31:0] signedA;    //for shift right arithmetic

   assign signedA = a;
   assign condinvb = alucontrol[0] ? ~b : b;
   assign sum = a + condinvb + alucontrol[0];
   assign isAddSub = ~alucontrol[2] & ~alucontrol[1] |
                     ~alucontrol[1] & alucontrol[0];   

   always_comb
     case (alucontrol)
       4'b0000:  result = sum;                // add
       4'b0001:  result = sum;                // subtract
       4'b0010:  result = a & b;              // and
       4'b0011:  result = a | b;              // or
       4'b0100:  result = a ^ b;              // xor
       4'b0110:  result = a << b[4:0];        // sll
       4'b0111:  result = a < b;              // sltu
       4'b0101:  result = sum[31] ^ v;        // slt
       4'b1000:  result = a >> b[4:0];        // srl
       4'b1001:  result = signedA >>> b[4:0]; // sra
       4'b1010:  result = b;                  // lui
       default: result = 32'bx;
     endcase

   assign zero = (result == 32'b0);
   assign negative = (result[31] == 1); //MSB is 1, two complement
   assign overflow = v;
   assign v = ~(alucontrol[0] ^ a[31] ^ b[31]) & (a[31] ^ sum[31]) & isAddSub;
   
endmodule // alu

module regfile (input  logic        clk, 
		input  logic 	    we3, 
		input  logic [4:0]  a1, a2, a3, 
		input  logic [31:0] wd3, 
		output logic [31:0] rd1, rd2);

   logic [31:0] 		    rf[31:0];

   // three ported register file
   // read two ports combinationally (A1/RD1, A2/RD2)
   // write third port on rising edge of clock (A3/WD3/WE3)
   // register 0 hardwired to 0

   always_ff @(posedge clk)
     if (we3) rf[a3] <= wd3;	

   assign rd1 = (a1 != 0) ? rf[a1] : 0;
   assign rd2 = (a2 != 0) ? rf[a2] : 0;
   
endmodule // regfile

