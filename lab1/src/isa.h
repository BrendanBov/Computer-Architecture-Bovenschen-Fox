/***************************************************************/
/*                                                             */
/*   RISC-V RV32 Instruction Level Simulator                   */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#ifndef _SIM_ISA_H_
#define _SIM_ISA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
//
// MACRO: Check sign bit (sb) of (v) to see if negative
//   if no, just give number
//   if yes, sign extend (e.g., 0x80_0000 -> 0xFF80_0000)
//
#define SIGNEXT(v, sb) ( v & (1 << (sb - 1)) ? ~((1 << (sb - 1)) - 1) | v : v & ((1 << (sb - 1)) - 1)  ) 

int ADD(int Rd, int Rs1, int Rs2) {
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] + CURRENT_STATE.REGS[Rs2];
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}

int ADDI(int Rd, int Rs1, int Imm)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}

int BNE(int Rs1, int Rs2, int Imm)
{
    int cur = 0;
    Imm = Imm << 1;
    if (CURRENT_STATE.REGS[Rs1] != CURRENT_STATE.REGS[Rs2])
        NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm, 13));
    return 0;
}

//
// I Instructions
//

//Imm acts as effective address
int LB(int Rd, int Imm, int Rs1)
{
    int mask = 0x3; //000...0011, each bit in address represents a byte in memory
    int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12); //get val at rs1 + imm sign extended
    int alignedAdr = effAdr & ~mask; //align address with word using mask
    int read = mem_read_32(alignedAdr); //read word from memory at aligned adr
    int offset = effAdr & mask; //offset is last 2 bits of effAdr, aka byte address

    //shift bytes in place by shifting 8 bits, sign extend from bit 8
    NEXT_STATE.REGS[Rd] = SIGNEXT(read >> 8 * offset, 8);
    return 0;
}
int LH(int Rd, int Imm, int Rs1)
{
    int mask = 0x3;
    int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
    int alignedAdr = effAdr & ~mask;
    int read = mem_read_32(alignedAdr);
    int offset = effAdr & mask;

    NEXT_STATE.REGS[Rd] = SIGNEXT(read >> 8 * offset, 16);
    return 0;
}
int LW(int Rd, int Imm, int Rs1)
{
    int read = mem_read_32(CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12));
    NEXT_STATE.REGS[Rd] = read;
    return 0;
}
int LBU(int Rd, int Imm, int Rs1)
{
    int mask = 0x3;
    int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
    int alignedAdr = effAdr & ~mask;
    int read = mem_read_32(alignedAdr);
    int offset = effAdr & mask;
    int maskU = 0xff; //makes all bits above bit 8 zeros
    NEXT_STATE.REGS[Rd] = maskU & (read >> 8 * offset);
    return 0;
}
int LHU(int Rd, int Imm, int Rs1)
{
    int mask = 0x3;
    int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
    int alignedAdr = effAdr & ~mask;
    int read = mem_read_32(alignedAdr);
    int offset = effAdr & mask;
    int maskU = 0xffff; //makes all bits above bit 16 zeros

    NEXT_STATE.REGS[Rd] = maskU & (read >> 8 * offset);
    return 0;
}
int SLLI(int Rd, int Rs1, int Imm)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] << (31 & SIGNEXT(Imm, 12));
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SLTI(int Rd, int Rs1, int Imm)
{
    int cur = 0;
    cur = (signed int)CURRENT_STATE.REGS[Rs1] < (signed int)SIGNEXT(Imm, 12);
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SLTIU(int Rd, int Rs1, int Imm)
{
    int cur = 0;
    cur = (unsigned int)CURRENT_STATE.REGS[Rs1] < (unsigned int)SIGNEXT(Imm, 12);
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int XORI(int Rd, int Rs1, int Imm)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] ^ SIGNEXT(Imm, 12);
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SRLI(int Rd, int Rs1, int Imm)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] >> (31 & SIGNEXT(Imm, 12));
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SRAI(int Rd, int Rs1, int Imm)
{
    int cur = CURRENT_STATE.REGS[Rs1];
    int shift = (31 & SIGNEXT(Imm, 12)); // 31 is 0x1f 
    cur = cur >> shift; //do shift
    //sign exten from the most significant bit of shifted register value
    NEXT_STATE.REGS[Rd] = SIGNEXT(cur, 32 - shift);
    return 0;
}
int ORI(int Rd, int Rs1, int Imm)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] | SIGNEXT(Imm, 12);
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int ANDI(int Rd, int Rs1, int Imm)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] & SIGNEXT(Imm, 12);
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}

// U Instruction
int AUIPC(int Rd, int UpImm)
{
    int cur = UpImm << 12;
    NEXT_STATE.REGS[Rd] = cur + CURRENT_STATE.PC;
    return 0;
}
int LUI(int Rd, int UpImm)
{
    int cur = UpImm << 12;
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}

// S Instruction
int SB(int Rs2, int Imm, int Rs1)
{
    int mask = 0x3; //bottom 2 lsb
    int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
    int alignedAdr = effAdr & ~mask; //align to words for memory
    int read = mem_read_32(alignedAdr);
    int offset = effAdr & mask; //last 2 bits of effAdr
    int maskRW = 0xff; //mask read write, controls space to be read and written
    int write = CURRENT_STATE.REGS[Rs2] & maskRW; //only lower 8 bits of register 2
    write = write << (offset * 8); //move mask and write by offset; 
    maskRW = maskRW << (offset * 8);

    int cutData = read & ~maskRW; //cut out byte to write to in read data
    //append cutData and write, then write at aligned address
    mem_write_32(alignedAdr, (cutData | write));

    return 0;
}
int SH(int Rs2, int Imm, int Rs1)
{
    int mask = 0x3; //bottom 2 lsb
    int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
    int alignedAdr = effAdr & ~mask; //align to words
    int read = mem_read_32(alignedAdr);
    int offset = effAdr & mask;
    int maskRW = 0xffff; //mask read write
    int write = CURRENT_STATE.REGS[Rs2] & maskRW; //only lower 8 bits 
    write = write << (offset * 8); //move mask and write by offset; 
    maskRW = maskRW << (offset * 8);

    int cutData = read & ~maskRW;
    //write low bits in read before masking for write
    mem_write_32(alignedAdr, (cutData | write));
    return 0;
}
int SW(int Rs2, int Imm, int Rs1)
{
    int adr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
    mem_write_32(adr, CURRENT_STATE.REGS[Rs2]);

    return 0;
}

// R instruction

int SUB(int Rd, int Rs1, int Rs2)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] - CURRENT_STATE.REGS[Rs2];
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SLL(int Rd, int Rs1, int Rs2) //31 and mask used to mask outside of last 5 bits, 00...0011111
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] << (31 & CURRENT_STATE.REGS[Rs2]);
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SLT(int Rd, int Rs1, int Rs2)
{
    int cur = 0;
    cur = (signed int)CURRENT_STATE.REGS[Rs1] < (signed int)CURRENT_STATE.REGS[Rs2];
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SLTU(int Rd, int Rs1, int Rs2)
{
    int cur = 0;
    cur = (unsigned int)CURRENT_STATE.REGS[Rs1] < (unsigned int)CURRENT_STATE.REGS[Rs2];
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int XOR(int Rd, int Rs1, int Rs2)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] ^ CURRENT_STATE.REGS[Rs2];
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SRL(int Rd, int Rs1, int Rs2)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] >> (31 & CURRENT_STATE.REGS[Rs2]);
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SRA(int Rd, int Rs1, int Rs2)
{
    int cur = CURRENT_STATE.REGS[Rs1];
    int shift = (31 & CURRENT_STATE.REGS[Rs2]); //use mask to limit shift by 31
    cur = cur >> shift;
    NEXT_STATE.REGS[Rd] = SIGNEXT(cur, 32 - shift);
    return 0;
}
int OR(int Rd, int Rs1, int Rs2)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] | CURRENT_STATE.REGS[Rs2];
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int AND(int Rd, int Rs1, int Rs2)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] & CURRENT_STATE.REGS[Rs2];
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}

// B instructions
int BEQ(int Rs1, int Rs2, int Imm)
{
    int cur = 0;
    Imm = Imm << 1;
    if (CURRENT_STATE.REGS[Rs1] == CURRENT_STATE.REGS[Rs2])
        NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm, 13));
    return 0;
}
int BLT(int Rs1, int Rs2, int Imm)
{
    int cur = 0;
    Imm = Imm << 1;
    if ((signed int)CURRENT_STATE.REGS[Rs1] < (signed int)CURRENT_STATE.REGS[Rs2])
        NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm, 13));
    return 0;
}
int BGE(int Rs1, int Rs2, int Imm)
{
    int cur = 0;
    Imm = Imm << 1;
    if ((signed int)CURRENT_STATE.REGS[Rs1] >= (signed int)CURRENT_STATE.REGS[Rs2])
        NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm, 13));
    return 0;
}
int BLTU(int Rs1, int Rs2, int Imm)
{
    int cur = 0;
    Imm = Imm << 1;
    if ((unsigned int)CURRENT_STATE.REGS[Rs1] < (unsigned int)CURRENT_STATE.REGS[Rs2])
        NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm, 13));
    return 0;
}
int BGEU(int Rs1, int Rs2, int Imm)
{
    int cur = 0;
    Imm = Imm << 1;
    if ((unsigned int)CURRENT_STATE.REGS[Rs1] >= (unsigned int)CURRENT_STATE.REGS[Rs2])
        NEXT_STATE.PC = (CURRENT_STATE.PC - 4) + (SIGNEXT(Imm, 13));
    return 0;
}

// I instruction
int JALR(int Rd, int Rs1, int Imm)
{
    int rd1 = CURRENT_STATE.REGS[Rs1];
    int immSE = SIGNEXT(Imm, 12);
    int adr = rd1 + immSE;
    NEXT_STATE.PC = adr - 4;
    NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
    return 0;
}

// J instruction
int JAL(int Rd, int Imm)
{
    NEXT_STATE.PC = CURRENT_STATE.PC + SIGNEXT(Imm, 12) - 4; //go back 1 address, start new address from PC 0
    NEXT_STATE.REGS[Rd] = CURRENT_STATE.PC + 4;
    return 0;
}

int ECALL(char* i_) { return 0; }

#endif
