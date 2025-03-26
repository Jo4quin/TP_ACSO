#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct {
    u_int32_t opcode;
    u_int32_t rd;
    u_int32_t rn;
    u_int32_t rm;
    u_int32_t imm12;
    u_int32_t imm19;
    u_int32_t imm26;
    u_int32_t shift;
    u_int8_t type; // 0 = register, 1 = immediate, 2 = branch, 3 = conditional branch
} decoded_instruction;

decoded_instruction decode_instruction(u_int32_t instruction){
    decoded_instruction decoded;

    uint32_t opcode_6 = (instruction >> 26) & 0x3F;

    if (opcode_6 == 0b000101) { // 📌 Branch incondicional (B)
        decoded.type = 2; // Tipo Branch
        decoded.imm26 = instruction & 0x3FFFFFF; // Offset de 26 bits
    } 
    else if (opcode_6 == 0b01010100) { // 📌 Branch condicional (B.cond)
        decoded.type = 3; // Tipo Branch Condicional
        decoded.imm19 = (instruction >> 5) & 0x7FFFF; // Offset de 19 bits
        decoded.rd = instruction & 0x1F; // Registro a evaluar
    } 
    else if ((instruction >> 30) == 0b00) { // 📌 Instrucciones con inmediato (ADD/SUB imm)
        decoded.type = 1; // Tipo Inmediato
        decoded.rd = instruction & 0x1F;
        decoded.rn = (instruction >> 5) & 0x1F;
        decoded.imm12 = (instruction >> 10) & 0xFFF;
    } 
    else { // 📌 Instrucciones de registro (ADD/SUB/EOR/ORR/etc.)
        decoded.type = 0; // Tipo Registro
        decoded.rd = instruction & 0x1F;
        decoded.rn = (instruction >> 5) & 0x1F;
        decoded.rm = (instruction >> 16) & 0x1F;
    }

    return decoded;
}

void process_instruction()
{

}

int main()
{
    process_instruction();
    return 0;
}

