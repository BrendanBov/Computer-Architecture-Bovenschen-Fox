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
#define SIGNEXT(v, sb) ( v & (1 << (sb - 1)) ? ~((1 << (sb)) - 1) : v)

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
// int LB (char* i_);
//Imm acts as effective address
int LB(int Rd, int Imm, int Rs1)
{
    int mask = 0xfffffffc; //11...100
    int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12); //get val at rs1 + imm sign extended
    int alignedAdr = effAdr & mask; //align adress with mask
    int read = mem_read_32(alignedAdr); //read word from memory at aligned adr
    int offset = effAdr & ~mask; //set offset from asked effective address
    //shift bytes in place by shifting 8 bits, sign extend from byte 0
    NEXT_STATE.REGS[Rd] = SIGNEXT(read >> 8 * offset, 7);
    return 0;
}
int LH(int Rd, int Imm, int Rs1)
{
    int mask = 0xfffffffc;
    int effAdr = CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12);
    int alignedAdr = effAdr & mask;
    int read = mem_read_32(alignedAdr);
    int offset = effAdr & ~mask;

    NEXT_STATE.REGS[Rd] = SIGNEXT(read >> 8 * offset, 15);
    return 0;
}
int LW(int Rd, int Imm, int Rs1)
{
    int cur = 0;
    cur = mem_read_32(CURRENT_STATE.REGS[Rs1] + SIGNEXT(Imm, 12));
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int LBU(char* i_);
int LHU(char* i_);
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
    cur = CURRENT_STATE.REGS[Rs1] < SIGNEXT(Imm, 12);
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
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] >>= (31 & SIGNEXT(Imm, 12));
    NEXT_STATE.REGS[Rd] = cur;
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
int AUIPC(char* i_);
int LUI(char* i_);

// S Instruction
int SB(char* i_);
int SH(char* i_);
int SW(char* i_);

// R instruction

int SUB(int Rd, int Rs1, int Rs2)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] - CURRENT_STATE.REGS[Rs2];
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SLL(int Rd, int Rs1, int Rs2) //31 and mask used to mask outside of last 5 bits
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] << (31 & CURRENT_STATE.REGS[Rs2]);
    NEXT_STATE.REGS[Rd] = cur;
    return 0;
}
int SLT(int Rd, int Rs1, int Rs2)
{
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] < CURRENT_STATE.REGS[Rs2];
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
    int cur = 0;
    cur = CURRENT_STATE.REGS[Rs1] >>= (31 & CURRENT_STATE.REGS[Rs2]);
    NEXT_STATE.REGS[Rd] = cur;
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
int BEQ(char* i_);
int BLT(char* i_);
int BGE(char* i_);
int BLTU(char* i_);
int BGEU(char* i_);

// I instruction
int JALR(char* i_);

// J instruction
int JAL(char* i_);

int ECALL(char* i_) { return 0; }

#endif
