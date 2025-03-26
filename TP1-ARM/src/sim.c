#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int opcode;
    int lenght;
} tuple_t;

tuple_t opcode_list[30] = {
        {10101011001, 11},  //ADDS extended
        {1011000100, 10},   //ADDS inmediate (ultimos 2 valores son el shift. hay que implementarlo)
        {1101000100, 10},   //SUBS inmediate (ultimos 2 valores son el shift. Hay que implentarlos)
        {11001011001, 11},  //SUBS extended 
        {11010100010, 11},  //HLT 
        {11101011001, 11},  //CMP extended
        {1111000100, 10},   //CMP inmediate (ultimos 2 valores son el shift. Hay que implementarlos)
        {11101010000, 11},  //ANDS shifted (NO HAY QUE IMPLEMENTAR EL SHIFT. son los anteultimos 2)
        {11001010000, 11},  //EOR shifted
        {11110001100, 11},  //ORR shifted
        {000101, 6},        //B branch
        {1101011, 7}        //BL branch to register
    };

typedef struct {
    u_int32_t opcode;
    u_int32_t rd;
    u_int32_t rn;
    u_int32_t rm;
    uint32_t cond;
    u_int32_t imm6;
    u_int32_t imm12;
    u_int32_t imm19;
    u_int32_t imm26;
    u_int32_t immhi;
    u_int32_t shift;
    u_int8_t type; // 0 = register, 1 = immediate, 2 = data transfer, 3 = branch, 4 = conditional branch
} decoded_instruction;

decoded_instruction decode_instruction(u_int32_t instruction){

    decoded_instruction decoded;

    for (int i = 0; i < 30; i++){
        if ((instruction >> (32-opcode_list[i].lenght)) == opcode_list[i].opcode){
            decoded.opcode = opcode_list[i].opcode;
            decoded.rd = (instruction >> 0) & 0b11111;
            decoded.rn = (instruction >> 5) & 0b11111;
            decoded.rm = (instruction >> 16) & 0b11111;
            decoded.cond = instruction & 0b1111;
            decoded.imm6 = (instruction >> 10) & 0b111111;
            decoded.imm12 = (instruction >> 10) & 0b111111111111;
            decoded.imm19 = (instruction >> 5) & 0b1111111111111111111;
            decoded.imm26 = (instruction >> 0) & 0b11111111111111111111111111;
            decoded.immhi = (instruction >> 5) & 0b111111111111111111111111111;
            decoded.shift = (instruction >> 22) & 0b11;

            if(opcode_list[i].lenght == 11){
                decoded.type = 0;
            } else if(opcode_list[i].lenght == 10){
                decoded.type = 1;
            } else {
                decoded.type = 2;
            }
            break;
        }
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

