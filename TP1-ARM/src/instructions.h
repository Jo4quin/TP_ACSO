#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "shell.h"


//estructura generica para las instrucciones
typedef struct {
    uint32_t opcode;
    uint32_t rm;
    uint32_t shamt;
    uint32_t rn;
    uint32_t rd;
    uint32_t ALU_immediate;
    uint32_t DT_address;
    uint32_t op;
    uint32_t rt;
    uint32_t BR_address;
    uint32_t cond_branch_address;
    uint32_t MOV_inmediate;
    uint8_t type; // 0 = register, 1 = immediate, 2 = data transfer, 3 = branch, 4 = conditional branch, 5 = inmediate wide
} decoded_instruction;

// Funciones para llamar las instrucciones
void arithmetic_instruction(decoded_instruction decoded_opcode);
void arithmetic_immediate_instruction(decoded_instruction decoded_opcode);
void data_transfer_instruction(decoded_instruction decoded_opcode);
void branch_instruction(decoded_instruction decoded_opcode);
void conditional_branch_instruction(decoded_instruction decoded_opcode);
void inmediate_wide_instruction(decoded_instruction decoded_opcode);

// Instrucciones 
void ADDS_Extended(decoded_instruction decoded_opcode);
void ADDS_Immediate(decoded_instruction decoded_opcode);
void SUBS_Immediate(decoded_instruction decoded_opcode);
void SUBS_Extended(decoded_instruction decoded_opcode);
void HLT(decoded_instruction decoded_opcode);
void CMP_Immediate(decoded_instruction decoded_opcode);
void CMP_Extended(decoded_instruction decoded_opcode);
void ANDS_Shifted(decoded_instruction decoded_opcode);
void ORR_Shifted(decoded_instruction decoded_opcode);
void EOR_Shifted(decoded_instruction decoded_opcode);
void ORR_Shifted(decoded_instruction decoded_opcode);
void B(decoded_instruction decoded_opcode);
void BR(decoded_instruction decoded_opcode);
void B_cond(decoded_instruction decoded_opcode);
void B_equal(decoded_instruction decoded_opcode);
void B_not_equal(decoded_instruction decoded_opcode);
void B_greater(decoded_instruction decoded_opcode);
void B_less(decoded_instruction decoded_opcode);
void B_greater_equal(decoded_instruction decoded_opcode);
void B_less_equal(decoded_instruction decoded_opcode);
void ADD_Extended(decoded_instruction decoded_opcode);
void ADD_Immediate(decoded_instruction decoded_opcode);
void MOVZ(decoded_instruction decoded_opcode);
void LDURB(decoded_instruction decoded_opcode);
void LDUR(decoded_instruction decoded_opcode);
void LDURH(decoded_instruction decoded_opcode);
void STURH(decoded_instruction decoded_opcode);
void STURB(decoded_instruction decoded_opcode);
void STUR(decoded_instruction decoded_opcode);
void shift(decoded_instruction decoded_opcode);
void BLE(decoded_instruction decoded_opcode);
void MUL(decoded_instruction decoded_opcode);
void CBZ(decoded_instruction decoded_instruction);
void CBNZ(decoded_instruction decoded_instruction);

