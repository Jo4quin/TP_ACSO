#include "instructions.h"


void ADDS_Extended(decoded_instruction decoded_opcode) {
    printf("ADD_Extended\n");
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] + (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rm];
    NEXT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;
    NEXT_STATE.FLAG_Z = (CURRENT_STATE.REGS[decoded_opcode.rd] == 0); // Flag de cero
    NEXT_STATE.FLAG_N = (CURRENT_STATE.REGS[decoded_opcode.rd] < 0); // Flag de negativo (bit más significativo)
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void ADDS_Immediate(decoded_instruction decoded_opcode) {
    printf("ADD Immediate\n");
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] + (uint64_t)decoded_opcode.ALU_immediate;
    NEXT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;
    NEXT_STATE.FLAG_Z = (CURRENT_STATE.REGS[decoded_opcode.rd] == 0); // Flag de cero
    NEXT_STATE.FLAG_N = (CURRENT_STATE.REGS[decoded_opcode.rd] < 0); // Flag de negativo (bit más significativo)
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void SUBS_Immediate(decoded_instruction decoded_opcode) {
    printf("SUBS Immediate\n");
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] - (uint64_t)decoded_opcode.ALU_immediate;
    NEXT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;
    NEXT_STATE.FLAG_Z = (CURRENT_STATE.REGS[decoded_opcode.rd] == 0); // Flag de cero
    NEXT_STATE.FLAG_N = (CURRENT_STATE.REGS[decoded_opcode.rd] < 0); // Flag de negativo (bit más significativo)
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void SUBS_Extended(decoded_instruction decoded_opcode) {
    printf("SUBS Extended\n");
    if(decoded_opcode.rd == 31) {
        printf("CMP ext\n");
        int64_t result = CURRENT_STATE.REGS[decoded_opcode.rn] - CURRENT_STATE.REGS[decoded_opcode.rm];
        NEXT_STATE.FLAG_N = (result < 0);
        NEXT_STATE.FLAG_Z = (result == 0);
    } else {
        printf("SUB\n");
        NEXT_STATE.REGS[decoded_opcode.rd] = CURRENT_STATE.REGS[decoded_opcode.rn] - CURRENT_STATE.REGS[decoded_opcode.rm];
        NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[decoded_opcode.rd] < 0);
        NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[decoded_opcode.rd] == 0);
    }
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void HLT(decoded_instruction decoded_opcode) {
    RUN_BIT = 0;
    printf("Simulación detenida. Volviendo al shell...\n");
}

void CMP_Immediate(decoded_instruction decoded_opcode) {
    printf("CMP Immediate\n");
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] - (uint64_t)decoded_opcode.ALU_immediate;
    NEXT_STATE.FLAG_Z = ((uint32_t)result == 0); // Flag de cero
    NEXT_STATE.FLAG_N = ((uint32_t)result < 0); // Flag de negativo (bit más significativo)
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}


void ANDS_Shifted(decoded_instruction decoded_opcode) {
    printf("ANDS Shifted\n");
    uint32_t result = CURRENT_STATE.REGS[decoded_opcode.rn] & CURRENT_STATE.REGS[decoded_opcode.rm];
    NEXT_STATE.REGS[decoded_opcode.rd] = result;
    NEXT_STATE.FLAG_Z = (result == 0); // Flag de cero
    NEXT_STATE.FLAG_N = (result < 0); // Flag de negativo (bit más significativo)
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void EOR_Shifted(decoded_instruction decoded_opcode) {
    printf("EOR Shifted\n");
    uint32_t result = CURRENT_STATE.REGS[decoded_opcode.rn] ^ CURRENT_STATE.REGS[decoded_opcode.rm];
    NEXT_STATE.REGS[decoded_opcode.rd] = result;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void ORR_Shifted(decoded_instruction decoded_opcode) {
    printf("ORR Shifted\n");
    uint32_t result = CURRENT_STATE.REGS[decoded_opcode.rn] | CURRENT_STATE.REGS[decoded_opcode.rm];
    NEXT_STATE.REGS[decoded_opcode.rd] = result;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void B(decoded_instruction decoded_opcode) {
    printf("B\n");
    int32_t var = decoded_opcode.BR_address;
    if(var & (1 << 25)) {
        var |= ~0x3FFFFFF; // Sign-extend
    }
    int64_t offset = (int64_t)(var) << 2;
    NEXT_STATE.PC = CURRENT_STATE.PC + (offset);
    printf("B: Branching to address 0x%016" PRIx64 "\n", NEXT_STATE.PC);
}

void BR(decoded_instruction decoded_opcode) {
    printf("BR\n");
    NEXT_STATE.PC = CURRENT_STATE.REGS[decoded_opcode.rn];
    printf("BR: Branching to address 0x%016" PRIx64 "\n", NEXT_STATE.PC);
}

void B_cond(decoded_instruction decoded_opcode) {
    printf("B_cond\n");
    switch(decoded_opcode.rt) {
        case 0b0000: 
            B_equal(decoded_opcode);
            break;
        case 0b0001:
            B_not_equal(decoded_opcode);
            break;
        case 0b1100:
            B_greater(decoded_opcode);
            break;
        case 0b1011:
            B_less(decoded_opcode);
            break;
        case 0b1010:
            B_greater_equal(decoded_opcode);
            break;
        case 0b1101:
            B_less_equal(decoded_opcode);
            break;
    }
}

void B_equal(decoded_instruction decoded_opcode){
    printf("B_equal\n");
    int32_t offset = decoded_opcode.cond_branch_address << 2; // se multiplica por 4
    if (decoded_opcode.cond_branch_address & 0x40000) {
        offset |= 0xFFFC0000; // Sign-extend
    }
    if (CURRENT_STATE.FLAG_Z == 1) {
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_not_equal(decoded_instruction decoded_opcode) {
    printf("B_not_equal\n");
    if (CURRENT_STATE.FLAG_Z == 0) {
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_greater(decoded_instruction decoded_opcode) {
    printf("B_greater\n");
    if (CURRENT_STATE.FLAG_N == 0 && CURRENT_STATE.FLAG_Z == 0) {
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_less(decoded_instruction decoded_opcode) {
    printf("B_less\n");
    if (CURRENT_STATE.FLAG_N == 1) {
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_greater_equal(decoded_instruction decoded_opcode) {
    printf("B_greater_equal\n");
    if (CURRENT_STATE.FLAG_N == 0) {
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_less_equal(decoded_instruction decoded_opcode) {
    printf("B_less_equal\n");
    if (CURRENT_STATE.FLAG_N == 1 || CURRENT_STATE.FLAG_Z == 1) {
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void ADD_Extended(decoded_instruction decoded_opcode) {
    printf("ADD Extended\n");
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] + (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rm];
    NEXT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void ADD_Immediate(decoded_instruction decoded_opcode) {
    printf("ADD Immediate\n");
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] + (uint64_t)decoded_opcode.ALU_immediate;
    NEXT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;}

void MOVZ(decoded_instruction decoded_opcode) {
    printf("MOVZ\n");
    NEXT_STATE.REGS[decoded_opcode.rd] = decoded_opcode.MOV_inmediate;
    NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[decoded_opcode.rd] < 0);
    NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[decoded_opcode.rd] == 0);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void LDURB(decoded_instruction decoded_opcode) {
    printf("LDURB\n");
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;
    uint8_t byte_value = mem_read_32(address) & 0xFF; // Solo los primeros 8 bits
    NEXT_STATE.REGS[decoded_opcode.rt] = (uint64_t)byte_value;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    printf("LDURB: Loaded byte 0x%02x from address 0x%016" PRIx64 " into W%d\n", byte_value, address, decoded_opcode.rt);
}

void LDUR(decoded_instruction decoded_opcode) {
    printf("LDUR\n");
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;
    uint64_t value = mem_read_32(address); // Lee 4 bytes (32 bits)
    value |= ((uint64_t)mem_read_32(address + 4)) << 32; // Lee los siguientes 4 bytes y combina
    NEXT_STATE.REGS[decoded_opcode.rt] = value;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    printf("LDUR: Loaded 0x%016" PRIx64 " from address 0x%016" PRIx64 " into X%d\n", value, address, decoded_opcode.rt);
}

void LDURH(decoded_instruction decoded_opcode) {
    printf("LDURH\n");
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;
    uint16_t halfword_value = mem_read_32(address) & 0xFFFF; // Solo los primeros 16 bits
    NEXT_STATE.REGS[decoded_opcode.rt] = (uint64_t)halfword_value;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    printf("LDURH: Loaded halfword 0x%04x from address 0x%016" PRIx64 " into W%d\n", halfword_value, address, decoded_opcode.rt);
}

void STURH(decoded_instruction decoded_opcode) {
    printf("STURH\n");
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;
    uint16_t halfword_value = CURRENT_STATE.REGS[decoded_opcode.rt] & 0xFFFF;
    uint8_t lower_byte = halfword_value & 0xFF;         // Byte menos significativo
    uint8_t upper_byte = (halfword_value >> 8) & 0xFF; // Byte más significativo
    mem_write_32(address, (mem_read_32(address) & 0xFFFFFF00) | lower_byte);       // Escribe el byte menos significativo
    mem_write_32(address + 1, (mem_read_32(address + 1) & 0xFFFFFF00) | upper_byte); // Escribe el byte más significativo
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    printf("STURH: Stored halfword 0x%04x from W%d into address 0x%016" PRIx64 "\n", halfword_value, decoded_opcode.rt, address);
}

void STURB(decoded_instruction decoded_opcode) {
    printf("STURB\n");
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;
    uint8_t byte_value = CURRENT_STATE.REGS[decoded_opcode.rt] & 0xFF;
    mem_write_32(address, (mem_read_32(address) & 0xFFFFFF00) | byte_value);
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    printf("STURB: Stored byte 0x%02x from X%d into address 0x%016" PRIx64 "\n", byte_value, decoded_opcode.rt, address);
}

void STUR(decoded_instruction decoded_opcode) {
    printf("STUR\n");
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;
    uint64_t value = CURRENT_STATE.REGS[decoded_opcode.rt];
    uint32_t lower_word = value & 0xFFFFFFFF;         // Parte menos significativa (32 bits)
    uint32_t upper_word = (value >> 32) & 0xFFFFFFFF; // Parte más significativa (32 bits)
    mem_write_32(address, lower_word);       // Escribe los primeros 32 bits
    mem_write_32(address + 4, upper_word);   // Escribe los siguientes 32 bits
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    printf("STUR: Stored 0x%016" PRIx64 " from X%d into address 0x%016" PRIx64 "\n", value, decoded_opcode.rt, address);
}

void shift(decoded_instruction decoded_opcode) {
    printf("Shift\n");
    uint32_t immr = (decoded_opcode.ALU_immediate >> 6) & 0b111111;    
    uint32_t imms = (decoded_opcode.ALU_immediate) & 0b111111;
    uint64_t source = CURRENT_STATE.REGS[decoded_opcode.rn];
    uint64_t result;
    if (imms != 63) {
        uint32_t shift = 63 - imms;
        result = (shift >= 64) ? 0 : (source << shift);
    } else {
        uint32_t shift = immr;
        result = (shift >= 64) ? 0 : (source >> shift);
    }
    NEXT_STATE.REGS[decoded_opcode.rd] = result;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void BLE(decoded_instruction decoded_opcode) {
    printf("BLE\n");
    if (NEXT_STATE.FLAG_Z == 1 || NEXT_STATE.FLAG_N == 1) {
        int32_t offset = (int32_t)(decoded_opcode.cond_branch_address << 13) >> 13; // Sign-extend el desplazamiento
        CURRENT_STATE.PC += (offset << 2); // El desplazamiento es en palabras, se multiplica por 4
        printf("BLE: Branching to address 0x%016" PRIx64 "\n", CURRENT_STATE.PC);
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
        printf("BLE: Condition not met, continuing to next instruction.\n");
    }
}

void MUL(decoded_instruction decoded_opcode) {
    printf("MUL\n");
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] * (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rm];
    NEXT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    printf("MUL: X%d = X%d * X%d = 0x%016" PRIx64 "\n", decoded_opcode.rd, decoded_opcode.rn, decoded_opcode.rm, result);
}

void CBZ(decoded_instruction decoded_instruction) {
    printf("CBZ\n");
    int32_t offset = decoded_instruction.cond_branch_address << 2; // Se multiplica por 4
    if (offset & 0x40000) {
        offset |= 0xFFFC0000; // Sign-extend
    }
    if (CURRENT_STATE.REGS[decoded_instruction.rt] == 0b0000) {
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void CBNZ(decoded_instruction decoded_instruction) {
    printf("CBNZ\n");
    int32_t offset = decoded_instruction.cond_branch_address << 2; // Se multiplica por 4
    if (offset & 0x40000) {
        offset |= 0xFFFC0000; // Sign-extend
    }
    if (CURRENT_STATE.REGS[decoded_instruction.rt] != 0b0000) {
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}