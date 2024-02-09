/***************************************************************/
/*                                                             */
/*   RISC-V RV32 Instruction Level Simulator                   */
/*                                                             */
/*   ECEN 4243                                                 */
/*   Oklahoma State University                                 */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "isa.h"

char* byte_to_binary(int x) {

    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

char* byte_to_binary32(int x) {

    static char b[33];
    b[0] = '\0';

    unsigned int z;
    for (z = 2147483648; z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

int bchar_to_int(char* rsa) {

    int i = 0;
    int result = 0;
    int t = 0;
    while (rsa[i] != '\0')i++;
    while (i > 0)
    {
        --i;
        // printf("%d\n", (rsa[i]-'0')<<t);
        result += (rsa[i] - '0') << t;
        t++;
    }
    return result;
}

int r_process(char* i_) {

    char d_opcode[8];
    d_opcode[0] = i_[31 - 6];
    d_opcode[1] = i_[31 - 5];
    d_opcode[2] = i_[31 - 4];
    d_opcode[3] = i_[31 - 3];
    d_opcode[4] = i_[31 - 2];
    d_opcode[5] = i_[31 - 1];
    d_opcode[6] = i_[31 - 0];
    d_opcode[7] = '\0';
    char rs1[6]; rs1[5] = '\0';
    char rs2[6]; rs2[5] = '\0';
    char rd[6]; rd[5] = '\0';
    char funct3[4]; funct3[3] = '\0';
    char funct7[8]; funct7[7] = '\0';
    for (int i = 0; i < 5; i++) {
        rs1[i] = i_[31 - 19 + i];
        rs2[i] = i_[31 - 24 + i];
        rd[i] = i_[31 - 11 + i];
    }
    for (int i = 0; i < 3; i++) {
        funct3[i] = i_[31 - 14 + i];
    }
    for (int i = 0; i < 7; i++) {
        funct7[i] = i_[31 - 31 + i];
    }
    int Rs1 = bchar_to_int(rs1);
    int Rs2 = bchar_to_int(rs2);
    int Rd = bchar_to_int(rd);
    int Funct3 = bchar_to_int(funct3);
    int Funct7 = bchar_to_int(funct7);
    printf("Opcode = %s\n Rs1 = %d\n Rs2 = %d\n Rd = %d\n Funct3 = %d\n Funct7 = %d\n",
        d_opcode, Rs1, Rs2, Rd, Funct3, Funct7);
    printf("\n");

    if (!strcmp(d_opcode, "0110011")) //handle r types with opcode 51
    {
        switch (Funct3)
        {
        case 0:
            if (Funct7 == 0)
            {
                printf("--- This is an ADD instruction. \n");
                ADD(Rd, Rs1, Rs2);
            }
            else if (Funct7 == 32) //0100000
            {
                printf("--- This is a SUB instruction. \n");
                SUB(Rd, Rs1, Rs2);
            }
            break;

        case 1:
            printf("--- This is an SLL instruction. \n");
            SLL(Rd, Rs1, Rs2);
            break;

        case 2:
            printf("--- This is an SLT instruction. \n");
            SLT(Rd, Rs1, Rs2);
            break;

        case 3:
            printf("--- This is an SLTU instruction. \n");
            SLTU(Rd, Rs1, Rs2);
            break;

        case 4:
            printf("--- This is an XOR instruction. \n");
            XOR(Rd, Rs1, Rs2);
            break;

        case 5:
            if (Funct7 == 0)
            {
                printf("--- This is an SRL instruction. \n");
                SRL(Rd, Rs1, Rs2);
            }
            else if (Funct7 == 32) //0100000
            {
                printf("--- This is a SRA instruction. \n");
                SRA(Rd, Rs1, Rs2);
            }
            break;

        case 6:
            printf("--- This is a OR instruction. \n");
            OR(Rd, Rs1, Rs2);
            break;

        case 7:
            printf("--- This is a AND instruction. \n");
            AND(Rd, Rs1, Rs2);
            break;

        default:
            return 1;
            break;
        }

        return 0;
    }

    return 1;
}

int i_process(char* i_) {

    char d_opcode[8];
    d_opcode[0] = i_[31 - 6];
    d_opcode[1] = i_[31 - 5];
    d_opcode[2] = i_[31 - 4];
    d_opcode[3] = i_[31 - 3];
    d_opcode[4] = i_[31 - 2];
    d_opcode[5] = i_[31 - 1];
    d_opcode[6] = i_[31 - 0];
    d_opcode[7] = '\0';
    char rs1[6]; rs1[5] = '\0';
    char rd[6]; rd[5] = '\0';
    char funct3[4]; funct3[3] = '\0';
    char funct7[8]; funct7[7] = '\0';
    char imm[13]; imm[12] = '\0';
    for (int i = 0; i < 5; i++) {
        rs1[i] = i_[31 - 19 + i];
        rd[i] = i_[31 - 11 + i];
    }
    for (int i = 0; i < 12; i++) {
        imm[i] = i_[31 - 31 + i];
    }
    for (int i = 0; i < 7; i++) {
        funct7[i] = i_[31 - 31 + i];
    }
    for (int i = 0; i < 3; i++) {
        funct3[i] = i_[31 - 14 + i];
    }
    int Rs1 = bchar_to_int(rs1);
    int Rd = bchar_to_int(rd);
    int Funct3 = bchar_to_int(funct3);
    int Funct7 = bchar_to_int(funct7);
    int Imm = bchar_to_int(imm);
    printf("Opcode = %s\n Rs1 = %d\n Imm = %d\n Rd = %d\n Funct3 = %d\n Funct7 = %d\n",
        d_opcode, Rs1, Imm, Rd, Funct3, Funct7);
    printf("\n");


    if (!strcmp(d_opcode, "0000011"))
    {
        switch (Funct3)
        {
        case 0:
            printf("--- This is an LB instruction. \n");
            LB(Rd, Imm, Rs1);
            break;

        case 1:
            printf("--- This is an LH instruction. \n");
            LH(Rd, Imm, Rs1);
            break;

        case 2:
            printf("--- This is an LW instruction. \n");
            LW(Rd, Imm, Rs1);
            break;

        case 4:
            printf("--- This is an LBU instruction. \n");
            LBU(Rd, Imm, Rs1);
            break;

        case 5:
            printf("--- This is an LHU instruction. \n");
            LHU(Rd, Imm, Rs1);
            break;

        default:
            return 1;
            break;
        }
        return 0;
    }
    /* This is an Add Immediate Instruciton */
    else if (!strcmp(d_opcode, "0010011")) //opcode 19
    {
        switch (Funct3)
        {
        case 0:
            printf("--- This is an ADDI instruction. \n");
            ADDI(Rd, Rs1, Imm);
            break;

        case 1:
            printf("--- This is an SLLI instruction. \n");
            SLLI(Rd, Rs1, Imm);
            break;

        case 2:
            printf("--- This is an SLTI instruction. \n");
            SLTI(Rd, Rs1, Imm);
            break;

        case 3:
            printf("--- This is an STLIU instruction. \n");
            SLTIU(Rd, Rs1, Imm);
            break;

        case 4:
            printf("--- This is an XORI instruction. \n");
            XORI(Rd, Rs1, Imm);
            break;

        case 5:
            if (Funct7 == 0) //funct7 = 0000000
            {
                printf("--- This is an SRLI instruction. \n");
                SRLI(Rd, Rs1, Imm);
            }
            else if (Funct7 == 32) //funct7 = 0100000
            {
                printf("--- This is an SRAI instruction. \n");
                SRAI(Rd, Rs1, Imm);
            }
            break;

        case 6:
            printf("--- This is an ORI instruction. \n");
            ORI(Rd, Rs1, Imm);
            break;

        case 7:
            printf("--- This is an ANDI instruction. \n");
            ANDI(Rd, Rs1, Imm);
            break;

            return 0;
        }
    }

    return 1;
}

int b_process(char* i_) {

    /* This function execute branch instruction */

    char d_opcode[8];
    d_opcode[0] = i_[31 - 6];
    d_opcode[1] = i_[31 - 5];
    d_opcode[2] = i_[31 - 4];
    d_opcode[3] = i_[31 - 3];
    d_opcode[4] = i_[31 - 2];
    d_opcode[5] = i_[31 - 1];
    d_opcode[6] = i_[31 - 0];
    d_opcode[7] = '\0';
    char rs1[6]; rs1[5] = '\0';
    char rs2[6]; rs2[5] = '\0';
    char funct3[4]; funct3[3] = '\0';
    char imm[13];
    for (int i = 0; i < 5; i++) {
        rs1[i] = i_[31 - 19 + i];
        rs2[i] = i_[31 - 24 + i];
    }
    // Old-fashioned method but works :)
    imm[0] = i_[31 - 31];
    imm[1] = i_[31 - 7];
    imm[2] = i_[31 - 30];
    imm[3] = i_[31 - 29];
    imm[4] = i_[31 - 28];
    imm[5] = i_[31 - 27];
    imm[6] = i_[31 - 26];
    imm[7] = i_[31 - 25];
    imm[8] = i_[31 - 11];
    imm[9] = i_[31 - 10];
    imm[10] = i_[31 - 9];
    imm[11] = i_[31 - 8];
    imm[12] = '\0';

    for (int i = 0; i < 3; i++) {
        funct3[i] = i_[31 - 14 + i];
    }
    int Rs1 = bchar_to_int(rs1);
    int Rs2 = bchar_to_int(rs2);
    int Funct3 = bchar_to_int(funct3);
    int Imm = bchar_to_int(imm);
    printf("Opcode = %s\n Rs1 = %d\n Rs2 = %d\n Imm = %d\n Funct3 = %d\n",
        d_opcode, Rs1, Rs2, Imm, Funct3);
    printf("\n");

    /* Add branch instructions here */

    if (!strcmp(d_opcode, "1100011")) { //opcode 99

        switch (Funct3) {
        case 0:
            printf("--- This is a BEQ instruction. \n");
            BEQ(Rs1, Rs2, Imm);
            break;

        case 1:
            printf("--- This is a BNE instruction. \n");
            BNE(Rs1, Rs2, Imm);
            break;

        case 4:
            printf("--- This is a BLT instruction. \n");
            BLT(Rs1, Rs2, Imm);
            break;

        case 5:
            printf("--- This is a BGE instruction. \n");
            BGE(Rs1, Rs2, Imm);
            break;

        case 6:
            printf("--- This is a BLTU instruction. \n");
            BLTU(Rs1, Rs2, Imm);
            break;

        case 7:
            printf("--- This is a BGEU instruction. \n");
            BGEU(Rs1, Rs2, Imm);
            break;
            return 0;
        }
    }

    return 1;

}

/* This function execute S type instructions */
int s_process(char* i_)
{
    char d_opcode[8];
    d_opcode[0] = i_[31 - 6];
    d_opcode[1] = i_[31 - 5];
    d_opcode[2] = i_[31 - 4];
    d_opcode[3] = i_[31 - 3];
    d_opcode[4] = i_[31 - 2];
    d_opcode[5] = i_[31 - 1];
    d_opcode[6] = i_[31 - 0];
    d_opcode[7] = '\0';
    char rs1[6]; rs1[5] = '\0';
    char rs2[6]; rs2[5] = '\0';
    char funct3[4]; funct3[3] = '\0';
    char imm[13]; imm[12] = '\0';
    for (int i = 0; i < 5; i++)
    {
        rs1[i] = i_[31 - 19 + i];
        rs2[i] = i_[31 - 24 + i];
    }
    //TODO: look back at this!!! Could be wrong and cause all SORTS of problems!
    for (int i = 0; i < 7; i++) { //bits 11:5
        imm[i] = i_[31 - 31 + i]; //msb 7 bits
    }
    for (int i = 0; i < 5; i++) { //bits 0:4
        imm[i + 7] = i_[31 - 11 + i]; //lsb 5 bits
    }
    for (int i = 0; i < 3; i++) {
        funct3[i] = i_[31 - 14 + i];
    }
    int Rs1 = bchar_to_int(rs1);
    int Rs2 = bchar_to_int(rs2);
    int Funct3 = bchar_to_int(funct3);
    int Imm = bchar_to_int(imm);
    printf("Opcode = %s\n Rs2 = %d\n Imm = %d\n Rs1 = %d\n Funct3 = %d\n",
        d_opcode, Rs2, Imm, Rs1, Funct3);
    printf("\n");

    switch (Funct3)
    {
    case 0:
        printf("--- This is an SB instruction. \n");
        SB(Rs2, Imm, Rs1);
        break;

    case 1:
        printf("--- This is an SH instruction. \n");
        SH(Rs2, Imm, Rs1);
        break;
    case 2:
        printf("--- This is an SW instruction. \n");
        SW(Rs2, Imm, Rs1);
        break;

    default:
        return 1;
        break;

        return 0;
    }

    return 1;

}

int j_process(char* i_) {

    /* This function execute Jump instructions */

    /* Add jump instructions here */

    return 1;

}

int u_process(char* i_) {

    /* This function execute U type instructions */
    char d_opcode[8];
    d_opcode[0] = i_[31 - 6];
    d_opcode[1] = i_[31 - 5];
    d_opcode[2] = i_[31 - 4];
    d_opcode[3] = i_[31 - 3];
    d_opcode[4] = i_[31 - 2];
    d_opcode[5] = i_[31 - 1];
    d_opcode[6] = i_[31 - 0];
    d_opcode[7] = '\0';
    char rd[6]; rd[5] = '\0';
    char upimm[21]; upimm[20] = '\0';
    for (int i = 0; i < 5; i++) {
        rd[i] = i_[31 - 11 + i];
    }
    for (int i = 0; i < 20; i++)
    {
        upimm[i] = i_[31 - 31 + i];
    }
    int UpImm = bchar_to_int(upimm);
    int Rd = bchar_to_int(rd);
    printf("Opcode = %s\n Rd = %d\n UpImm = %x\n",
        d_opcode, Rd, UpImm);
    printf("\n");

    if (!strcmp(d_opcode, "0110111")) { //opcode 55
        printf("--- This is an LUI instruction. \n");
        LUI(Rd, UpImm);
        return 0;
    }
    else if (!strcmp(d_opcode, "0010111")) { //opcode 23
        printf("--- This is an AUIPC instruction. \n");
        AUIPC(Rd, UpImm);
        return 0;
    }

    return 1;

}

int interruption_process(char* i_) {

    ECALL(i_);
    RUN_BIT = 0;
    return 0;

}

int decode_and_execute(char* i_) {

    /*
       This function decode the instruction and update
       CPU_State (NEXT_STATE)
    */

    //opcode 3 and 19
    if (((i_[25] == '0') && (i_[26] == '0') &&
        /*(i_[27] == '1') &&*/ (i_[28] == '0') && //dont check bit 27 to include opcode 3 and 19
        (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1'))) {
        printf("- This is an Immediate Type Instruction. \n");
        i_process(i_);
    }
    if ((i_[25] == '0') && (i_[26] == '1') &&
        (i_[27] == '1') && (i_[28] == '0') &&
        (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
        printf("- This is an R Type Instruction. \n");
        r_process(i_);
    }
    if ((i_[25] == '1') && (i_[26] == '1') &&
        (i_[27] == '0') && (i_[28] == '0') &&
        (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
        printf("- This is a B Type Instruction. \n");
        b_process(i_);
    }
    if ((i_[25] == '0') && (i_[26] == '1') &&
        (i_[27] == '0') && (i_[28] == '0') &&
        (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
        printf("- This is a S Type Instruction. \n");
        s_process(i_);
    }
    if ((i_[25] == '1') && (i_[26] == '1') &&
        (i_[27] == '0') && (i_[28] == '1') &&
        (i_[29] == '1') && (i_[30] == '1') && (i_[31] == '1')) {
        printf("- This is a J Type Instruction. \n");
        j_process(i_);
    }
    if ((i_[25] == '0') && /*(i_[26] == '0') &&*/ //ignore to handle addui and lui
        (i_[27] == '1') && (i_[28] == '0') &&
        (i_[29] == '1') && (i_[30] == '1') && (i_[31] == '1')) {
        printf("- This is a U Type Instruction. \n");
        u_process(i_);
    }
    if ((i_[25] == '1') && (i_[26] == '1') &&
        (i_[27] == '1') && (i_[28] == '0') &&
        (i_[29] == '0') && (i_[30] == '1') && (i_[31] == '1')) {
        printf("- This is a Software Interruption Instruction. \n");
        interruption_process(i_);
    }

    return 0;

}

unsigned int OPCODE(unsigned int i_word) {

    return ((i_word << 27) >> 27);

}

void process_instruction() {

    /*
       execute one instruction here. You should use CURRENT_STATE and modify
       values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
       access memory.
    */

    printf("--------------------------------\n");
    unsigned int inst_word = mem_read_32(CURRENT_STATE.PC);
    printf("\nThe instruction is: %x \n", inst_word);
    //printf("33222222222211111111110000000000\n");
    //printf("10987654321098765432109876543210\n");
    //printf("--------------------------------\n");
    printf("%s \n", byte_to_binary32(inst_word));
    printf("\n");
    decode_and_execute(byte_to_binary32(inst_word));

    NEXT_STATE.PC += 4;

}
