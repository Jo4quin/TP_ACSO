#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int opcode;
    int largo;
} tuple_t;

tuple_t opcode_list[30] = {
        {10101011001, 11}, //ADDS extended
        {3, 4},
        {5, 6}
    };

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

