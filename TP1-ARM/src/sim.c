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
        {10101011001, 11}, //ADDS extended
<<<<<<< HEAD
        {3, 4},
        {5, 6},
        {0b1101001101,10}, // LSL (immediate)
        {0b1101001101,10}, //  LSR (immediate)MISMO QUE EL LSL SOLO CAMBIA EL IMMR 
        {0b11111000000,11}, //STUR
        {0b00111000000,11}, //STURB
        {0b01111000000,11},//STURH
        {0b1111000010,10},//LDUR
        {0b01111000010,11},//LDURH
        {0b00111000010}, // LDURB
        {0b11010010100,11}, // MOVZ
        {0b10011011000,11}, // MUL(Escalares)
        {0b1011000, 7}, // CBZ
        {0b1011100, 7}, // CBNZ
=======
        {1011000100, 10}, //ADDS inmediate (ultimos 2 valores son el shift. hay que implementarlo)
        {1101000100, 10}, //SUBS inmediate (ultimos 2 valores son el shift. Hay que implentarlos)
        {11001011001, 11}, //SUBS extended 
        {11010100010, 11}, //HLT 
        {11101011001, 11}, //CMP extended
        {1111000100, 10}, //CMP inmediate (ultimos 2 valores son el shift. Hay que implementarlos)
        {11101010000, 11}, //ANDS shifted (NO HAY QUE IMPLEMENTAR EL SHIFT. son los anteultimos 2)
        {11001010000, 11}, //EOR shifted
        {11110001100, 11}, //ORR shifted
        {}
>>>>>>> e616a1b1c5c528f12c990fbae6ee59586b99f704
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
    for (int i = 0; i < 30; i++){
        if ((instruction >> (32-opcode_list[i].lenght)) == opcode_list[i].opcode){
            decoded.opcode = opcode_list[i].opcode;
            decoded.type = i;
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

