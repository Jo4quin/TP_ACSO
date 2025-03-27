#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "shell.h"

typedef struct {
    u_int32_t opcode;
    int length;
    int type;  // 0 = register, 1 = immediate, 2 = data transfer, 3 = branch, 4 = conditional branch, 5 = inmediate wide
} tuple_t;

typedef struct {
    u_int32_t opcode;
    u_int32_t rm;
    u_int32_t shamt;
    u_int32_t rn;
    u_int32_t rd;
    u_int32_t ALU_immediate;
    u_int32_t DT_address;
    u_int32_t op;
    u_int32_t rt;
    u_int32_t BR_address;
    u_int32_t cond_branch_address;
    u_int32_t MOV_inmediate;
    u_int8_t type; // 0 = register, 1 = immediate, 2 = data transfer, 3 = branch, 4 = conditional branch, 5 = inmediate wide
} decoded_instruction;

tuple_t opcode_list[26] = {
    {10001011001, 11, 0}, // ADD Extended
    {10101011000, 11, 0}, // ADDS Extended
    {10011011000, 11, 0}, // MUL (Scalar Multiplication)
    {11001011001, 11, 0}, // SUBS Extended
    {11101011001, 11, 0}, // CMP Extended
    {11101010000, 11, 0}, // ANDS Shifted
    {11001010000, 11, 0}, // EOR Shifted
    {11110001100, 11, 0}, // ORR Shifted
    {1001000100, 10, 1},  // ADD Immediate
    {1101001101, 10, 1},  // LSR Immediat                                       e
    {1101001101, 10, 1},  // LSL Immediate
    {1111000100, 10, 1},  // CMP Immediate
    {1011000100, 10, 1},  // ADDS Immediate
    {1101000100, 10, 1},  // SUBS Immediate
    {11111000000, 11, 2}, // STUR (Store)
    {00111000000, 11, 2}, // STURB (Store Byte)
    {01111000000, 11, 2}, // STURH (Store Halfword)
    {1111000010, 10, 2},  // LDUR (Load)
    {01111000010, 11, 2}, // LDURH (Load Halfword)
    {00111000010, 11, 2}, // LDURB (Load Byte)
    {11010100010, 11, 3}, // HLT (Halt)
    {1101011, 7, 3},      // BR (Branch to Register)
    {000101, 6, 3}        // B (Branch)
    {10111001, 8, 4},     // CBNZ (Conditional Branch)
    {10110100, 8, 4},     // CBZ (Conditional Branch)
    {11010010100, 11, 5}, // MOVZ (Inmediate Wide)
};

decoded_instruction decode_instruction(u_int32_t instruction){

    decoded_instruction decoded;

    for (int i = 0; i < 26; i++){
        if ((instruction >> (32-opcode_list[i].length)) == opcode_list[i].opcode){
            if(opcode_list[i].type == 0){
                decoded.opcode = opcode_list[i].opcode;
                decoded.rm = (instruction >> 15) & 0b11111;
                decoded.shamt = (instruction >> 10) & 0b11111;
                decoded.rn = (instruction >> 5) & 0b11111;
                decoded.rd = instruction & 0b11111;
                decoded.type = 0;
                return decoded;
            }
            if(opcode_list[i].type == 1){
                decoded.opcode = opcode_list[i].opcode;
                decoded.ALU_immediate = (instruction >> 10) & 0b111111111111;
                decoded.rn = (instruction >> 5) & 0b11111;
                decoded.rd = instruction & 0b11111;
                decoded.type = 1;
                return decoded;
            }
            if(opcode_list[i].type == 2){
                decoded.opcode = opcode_list[i].opcode;
                decoded.DT_address = (instruction >> 12) & 0b111111111;
                decoded.op = (instruction >> 10) & 0b11;
                decoded.rn = (instruction >> 5) & 0b11111;
                decoded.rt = instruction & 0b11111;
                decoded.type = 2;
                return decoded;
            }
            if(opcode_list[i].type == 3){
                decoded.opcode = opcode_list[i].opcode;
                decoded.BR_address = instruction & 0b11111111111111111111111111;
                decoded.type = 3;
                return decoded;
            }
            if(opcode_list[i].type == 4){
                decoded.opcode = opcode_list[i].opcode;
                decoded.cond_branch_address = (instruction >> 5) & 0b1111111111111111111;
                decoded.rt = (instruction >> 5) & 0b11111;
                decoded.type = 4;
                return decoded;
            }
            if(opcode_list[i].type == 5){
                decoded.opcode = opcode_list[i].opcode;
                decoded.MOV_inmediate = (instruction >> 5) & 0b1111111111111111;
                decoded.rd = instruction & 0b11111;
                decoded.type = 5;
                return decoded;
            }
        }
    }
    return decoded;
}

void run_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.type){
        case 0:
            arithmetic_instruction(decoded_opcode);
            break;
        case 1:
            arithmetic_immediate_instruction(decoded_opcode);
            break;
        case 2:
            data_transfer_instruction(decoded_opcode);
            break;
        case 3:
            branch_instruction(decoded_opcode);
            break;
        case 4:
            conditional_branch_instruction(decoded_opcode);
            break;
        case 5:
            inmediate_wide_instruction(decoded_opcode);
            break;
    }
}

void arithmetic_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.opcode){
        case 10001011001:
            ADD_Extended(decoded_opcode);
            break;
        case 10101011001:
            ADDS_Extended(decoded_opcode);
            break;
        case 10011011000:
            MUL_Scalar_Multiplication(decoded_opcode);
            break;
        case 11001011001:
            SUBS_Extended(decoded_opcode);
            break;
        case 11101011001:
            CMP_Extended(decoded_opcode);
            break;
        case 11101010000:
            ANDS_Shifted(decoded_opcode);
            break;
        case 11001010000:
            EOR_Shifted(decoded_opcode);
            break;
        case 11110001100:
            ORR_Shifted(decoded_opcode);
            break;
    }
}

void arithmetic_immediate_instruction(decoded_instruction decoded_opcode){  //hay que ver el tema LSR y LSL
    switch (decoded_opcode.opcode){
        case 1001000100:
            ADD_Immediate(decoded_opcode);
            break;
        case 1101001101:
            LSR_Immediate(decoded_opcode);
            break;
        case 1101001101:
            LSL_Immediate(decoded_opcode);
            break;
        case 1111000100:
            CMP_Immediate(decoded_opcode);
            break;
        case 1011000100:
            ADDS_Immediate(decoded_opcode);
            break;
        case 1101000100:
            SUBS_Immediate(decoded_opcode);
            break;
    }
}

void data_transfer_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.opcode){
        case 11111000000:
            STUR(decoded_opcode);
            break;
        case 00111000000:
            STURB(decoded_opcode);
            break;
        case 01111000000:
            STURH(decoded_opcode);
            break;
        case 1111000010:
            LDUR(decoded_opcode);
            break;
        case 01111000010:
            LDURH(decoded_opcode);
            break;
        case 00111000010:
            LDURB(decoded_opcode);
            break;
    }
}

void branch_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.opcode){
        case 11010100010:
            HLT(decoded_opcode);
            break;
        case 1101011:
            BR(decoded_opcode);
            break;
        case 000101:
            B(decoded_opcode);
            break;
    }
}

void conditional_branch_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.opcode){
        case 10111001:
            CBNZ(decoded_opcode);
            break;
        case 10110100:
            CBZ(decoded_opcode);
            break;
    }
}

void inmediate_wide_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.opcode){
        case 11010010100:
            MOVZ(decoded_opcode);
            break;
    }
}



void process_instruction()
{
    u_int32_t instruction = mem_read_32(CURRENT_STATE.PC);

    decoded_instruction decoded = decode_instruction(instruction);

    run_instruction(decoded);



}

void ADDS_Extended(decoded_instruction decoded_opcode){
    CURRENT_STATE.REGS[decoded_opcode.rd] = CURRENT_STATE.REGS[decoded_opcode.rn] + CURRENT_STATE.REGS[decoded_opcode.rm];
    CURRENT_STATE.FLAG_N = CURRENT_STATE.REGS[decoded_opcode.rd] < 0;
    CURRENT_STATE.FLAG_Z = CURRENT_STATE.REGS[decoded_opcode.rd] == 0;
}

void ADD_Extended(decoded_instruction decoded_opcode){
    CURRENT_STATE.REGS[decoded_opcode.rd] = CURRENT_STATE.REGS[decoded_opcode.rn] + CURRENT_STATE.REGS[decoded_opcode.rm];
}

void HLT(decoded_instruction decoded_opcode){
    RUN_BIT = 0;
}

int main()
{
    process_instruction();
    return 0;
}
