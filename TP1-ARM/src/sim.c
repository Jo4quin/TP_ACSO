#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "shell.h"
#include "instructions.h"

void print_binary(uint32_t n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 4 == 0)  // Opcional: agrega un espacio cada 4 bits para mejorar la legibilidad
            printf(" ");
    }
    printf("\n");
}

typedef struct {
    uint32_t opcode;
    int length;
    int type;  // 0 = register, 1 = immediate, 2 = data transfer, 3 = branch, 4 = conditional branch, 5 = inmediate wide
} tuple_t;

// Lista de tuplas con los opcodes de las instrucciones
tuple_t opcode_list[25] = {
    {0b10001011001, 11, 0}, // ADD Extended
    {0b10101011000, 11, 0}, // ADDS Extended
    {0b10011011000, 11, 0}, // MUL (Scalar Multiplication)
    {0b11101011000, 11, 0}, // SUBS Extended
    {0b11101011001, 11, 0}, // CMP Extended
    {0b11101010000, 11, 0}, // ANDS Shifted
    {0b11001010000, 11, 0}, // EOR Shifted
    {0b11110001100, 11, 0}, // ORR Shifted
    {0b1001000100, 10, 1},  // ADD Immediate
    {0b1101001101, 9, 1},   // shift
    {0b1011000100, 10, 1},  // ADDS Immediate
    {0b1101000100, 10, 1},  // SUBS Immediate
    {0b11111000000, 11, 2}, // STUR (Store)
    {0b00111000000, 11, 2}, // STURB (Store Byte)
    {0b01111000000, 11, 2}, // STURH (Store Halfword)
    {0b1111000010, 10, 2},  // LDUR (Load)
    {0b01111000010, 11, 2}, // LDURH (Load Halfword)
    {0b00111000010, 11, 2}, // LDURB (Load Byte)
    {0b11010100010, 11, 3}, // HLT (Halt)
    {0b1101011, 7, 3},      // BR (Branch to Register)
    {0b000101, 6, 3},       // B (Branch)
    {0b01010100, 8, 4},     // B.cond (Branch Conditional)
    {0b10111001, 8, 4},     // CBNZ (Conditional Branch)
    {0b10110100, 8, 4},     // CBZ (Conditional Branch)
    {0b11010010100, 11, 5}, // MOVZ (Inmediate Wide)
};

decoded_instruction decode_instruction(uint32_t instruction){

    decoded_instruction decoded;

    for (int i = 0; i < 26; i++){
        if ((instruction >> (32-opcode_list[i].length)) == opcode_list[i].opcode){
            if(opcode_list[i].type == 0){
                decoded.opcode = opcode_list[i].opcode;
                decoded.rm = (instruction >> 16) & 0b11111;
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
        case 0b10001011001:
            ADD_Extended(decoded_opcode);
            break;
        case 0b10101011000:
            ADDS_Extended(decoded_opcode);
            break;
        case 0b10011011000:
            MUL(decoded_opcode);
            break;
        case 0b11101011000:
            SUBS_Extended(decoded_opcode);
            break;
        case 0b11101011001:
            CMP_Extended(decoded_opcode);
            break;
        case 0b11101010000:
            ANDS_Shifted(decoded_opcode);
            break;
        case 0b11001010000:
            EOR_Shifted(decoded_opcode);
            break;
        case 0b11110001100:
            ORR_Shifted(decoded_opcode);
            break;
    }
}

void arithmetic_immediate_instruction(decoded_instruction decoded_opcode){  //hay que ver el tema LSR y LSL
    switch (decoded_opcode.opcode){
        case 0b1001000100:
            ADD_Immediate(decoded_opcode);
            break;
        case 0b1101001101:
            shift(decoded_opcode);
            break;
        case 0b1111000100:
            CMP_Immediate(decoded_opcode);
            break;
        case 0b1011000100:
            printf("ADD Immediate\n");
            ADDS_Immediate(decoded_opcode);
            break;
        case 0b1101000100:
            SUBS_Immediate(decoded_opcode);
            break;
    }
}

void data_transfer_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.opcode){
        case 0b11111000000:
            STUR(decoded_opcode);
            break;
        case 0b00111000000:
            STURB(decoded_opcode);
            break;
        case 0b01111000000:
            STURH(decoded_opcode);
            break;
        case 0b1111000010:
            LDUR(decoded_opcode);
            break;
        case 0b00111000010:
            LDURB(decoded_opcode);
            break;
    }
}

void branch_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.opcode){
        case 0b11010100010:
            HLT(decoded_opcode);
            break;
        case 0b1101011:
            BR(decoded_opcode);
            break;
        case 0b000101:
            B(decoded_opcode);
            break;
    }
}

void conditional_branch_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.opcode){
        case 0b10111001:
            CBNZ(decoded_opcode);
            break;
        case 0b10110100:
            CBZ(decoded_opcode);
            break;
        case 0b01010100:
            B_cond(decoded_opcode);
            break;
    }
}

void inmediate_wide_instruction(decoded_instruction decoded_opcode){
    switch (decoded_opcode.opcode){
        case 0b11010010100:
            MOVZ(decoded_opcode);
            break;
    }
}


void process_instruction()
{
    uint32_t instruction = mem_read_32(CURRENT_STATE.PC);

    printf("Instruction: ");
    print_binary(instruction);

    decoded_instruction decoded = decode_instruction(instruction);

    printf("Decoded: ");            //sims ok
    print_binary(decoded.opcode);
    printf("rm: ");                 //
    print_binary(decoded.rm);
    printf("shamt: ");
    print_binary(decoded.shamt);
    printf("rn: ");
    print_binary(decoded.rn);
    printf("rd: ");
    print_binary(decoded.rd);
    printf("ALU_immediate: ");
    print_binary(decoded.ALU_immediate);
    printf("DT_address: ");
    print_binary(decoded.DT_address);
    printf("op: ");
    print_binary(decoded.op);
    printf("rt: ");
    print_binary(decoded.rt);
    printf("BR_address: ");
    print_binary(decoded.BR_address);
    printf("cond_branch_address: ");
    print_binary(decoded.cond_branch_address);
    printf("MOV_inmediate: ");
    print_binary(decoded.MOV_inmediate);
    printf("type: %d\n", decoded.type);

    run_instruction(decoded);
}

