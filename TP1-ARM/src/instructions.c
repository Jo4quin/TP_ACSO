#include "instructions.h"


void ADDS_Extended(decoded_instruction decoded_opcode) {
    // Realiza la suma entre el registro y el valor inmediato
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] + (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rm];

    // Almacena el resultado en el registro de destino
    CURRENT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;

    // Actualiza los flags del procesador
    NEXT_STATE.FLAG_Z = (CURRENT_STATE.REGS[decoded_opcode.rd] == 0); // Flag de cero
    NEXT_STATE.FLAG_N = (CURRENT_STATE.REGS[decoded_opcode.rd] >> 31) & 1; // Flag de negativo (bit más significativo)

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void ADDS_Immediate(decoded_instruction decoded_opcode) {
    // Realiza la suma entre el registro y el valor inmediato
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] + (uint64_t)decoded_opcode.ALU_immediate;

    // Almacena el resultado en el registro de destino
    CURRENT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;

    // Actualiza los flags del procesador
    NEXT_STATE.FLAG_Z = (CURRENT_STATE.REGS[decoded_opcode.rd] == 0); // Flag de cero
    NEXT_STATE.FLAG_N = (CURRENT_STATE.REGS[decoded_opcode.rd] >> 31) & 1; // Flag de negativo (bit más significativo)

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void SUBS_Immediate(decoded_instruction decoded_opcode) {
    // Realiza la resta entre el registro y el valor inmediato
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] - (uint64_t)decoded_opcode.ALU_immediate;

    // Almacena el resultado en el registro de destino
    CURRENT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;

    // Actualiza los flags del procesador
    NEXT_STATE.FLAG_Z = (CURRENT_STATE.REGS[decoded_opcode.rd] == 0); // Flag de cero
    NEXT_STATE.FLAG_N = (CURRENT_STATE.REGS[decoded_opcode.rd] >> 31) & 1; // Flag de negativo (bit más significativo)

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void SUBS_Extended(decoded_instruction decoded_opcode) {
    // Realiza la resta entre los registros rn y rm
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] - (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rm];

    // Almacena el resultado en el registro de destino rd
    CURRENT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;

    // Actualiza los flags del procesador
    NEXT_STATE.FLAG_Z = (CURRENT_STATE.REGS[decoded_opcode.rd] == 0); // Flag de cero
    NEXT_STATE.FLAG_N = (CURRENT_STATE.REGS[decoded_opcode.rd] >> 31) & 1; // Flag de negativo (bit más significativo)

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void HLT(decoded_instruction decoded_opcode) {
    // Detener la simulación
    RUN_BIT = 0;

    // Mensaje opcional para depuración
    printf("Simulación detenida. Volviendo al shell...\n");
}

void CMP_Immediate(decoded_instruction decoded_opcode) {
    // Realiza la resta entre el registro y el valor inmediato
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] - (uint64_t)decoded_opcode.ALU_immediate;

    // No se almacena el resultado, solo se actualizan los flags del procesador
    NEXT_STATE.FLAG_Z = ((uint32_t)result == 0); // Flag de cero
    NEXT_STATE.FLAG_N = ((uint32_t)result >> 31) & 1; // Flag de negativo (bit más significativo)

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void CMP_Extended(decoded_instruction decoded_opcode){
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] - (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rm];

    // No se almacena el resultado, solo se actualizan los flags del procesador
    NEXT_STATE.FLAG_Z = ((uint32_t)result == 0); // Flag de cero
    NEXT_STATE.FLAG_N = ((uint32_t)result >> 31) & 1; // Flag de negativo (bit más significativo)

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void ANDS_Shifted(decoded_instruction decoded_opcode) {
    uint32_t shifted_value;

    // Aplica el desplazamiento (shift) al registro rm
    switch (decoded_opcode.shamt) {
        case 0b00: // LSL (Logical Shift Left)
            shifted_value = CURRENT_STATE.REGS[decoded_opcode.rm] << decoded_opcode.ALU_immediate;
            break;
        case 0b01: // LSR (Logical Shift Right)
            shifted_value = CURRENT_STATE.REGS[decoded_opcode.rm] >> decoded_opcode.ALU_immediate;
            break;
        default:
            printf("Error: Tipo de desplazamiento no reconocido.\n");
            return;
    }

    // Realiza la operación AND entre rn y el valor desplazado
    uint32_t result = CURRENT_STATE.REGS[decoded_opcode.rn] & shifted_value;

    // Almacena el resultado en el registro de destino
    CURRENT_STATE.REGS[decoded_opcode.rd] = result;

    // Actualiza los flags del procesador
    NEXT_STATE.FLAG_Z = (result == 0); // Flag de cero
    NEXT_STATE.FLAG_N = (result >> 31) & 1; // Flag de negativo (bit más significativo)

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void EOR_Shifted(decoded_instruction decoded_opcode) {
    // Realiza la operación XOR entre los registros rn y rm
    uint32_t result = CURRENT_STATE.REGS[decoded_opcode.rn] ^ CURRENT_STATE.REGS[decoded_opcode.rm];

    // Almacena el resultado en el registro de destino rd
    CURRENT_STATE.REGS[decoded_opcode.rd] = result;

    // No se actualizan los flags, ya que no se especifica en la descripción.

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void ORR_Shifted(decoded_instruction decoded_opcode) {
    // Realiza la operación OR entre los registros rn y rm
    uint32_t result = CURRENT_STATE.REGS[decoded_opcode.rn] | CURRENT_STATE.REGS[decoded_opcode.rm];

    // Almacena el resultado en el registro de destino rd
    NEXT_STATE.REGS[decoded_opcode.rd] = result;

    // No se actualizan los flags, ya que no se especifica en la descripción.

    // Incrementa el contador de programa
    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
}

void B(decoded_instruction decoded_opcode) {
    // Realiza el salto a la dirección calculada
    int32_t offset = (int32_t)(decoded_opcode.BR_address << 6) >> 6; // Sign-extend el desplazamiento
    NEXT_STATE.PC = CURRENT_STATE.PC + (offset << 2); // El desplazamiento es en palabras, se multiplica por 4

    // Mensaje opcional para depuración
    printf("B: Branching to address 0x%016" PRIx64 "\n", NEXT_STATE.PC);
}

void BR(decoded_instruction decoded_opcode) {
    // Realiza el salto a la dirección calculada
    NEXT_STATE.PC = CURRENT_STATE.REGS[decoded_opcode.rn];
    // Mensaje opcional para depuración
    printf("BR: Branching to address 0x%016" PRIx64 "\n", NEXT_STATE.PC);
}

void B_cond(decoded_instruction decoded_opcode) {
    // Verifica los flags para determinar si se cumple la condición
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
    if (CURRENT_STATE.FLAG_Z == 1) {
        // Realiza el  salto a la direccion calculada
        int32_t offset = decoded_opcode.cond_branch_address << 2; // se multiplica por 4

        // Extensión de signo si el bit 18 (más significativo de imm19) es 1
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }

        NEXT_STATE.PC = CURRENT_STATE.PC + offset;

    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_not_equal(decoded_instruction decoded_opcode) {
    if (CURRENT_STATE.FLAG_Z == 0) {
        // Realiza el salto a la dirección calculada
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4

        // Extensión de signo si el bit 18 (más significativo de imm19) es 1
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }

        NEXT_STATE.PC = CURRENT_STATE.PC + offset;

    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_greater(decoded_instruction decoded_opcode) {
    if (CURRENT_STATE.FLAG_N == 0 && CURRENT_STATE.FLAG_Z == 0) {
        // Realiza el salto a la dirección calculada
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4

        // Extensión de signo si el bit 18 (más significativo de imm19) es 1
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }

        NEXT_STATE.PC = CURRENT_STATE.PC + offset;

    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_less(decoded_instruction decoded_opcode) {
    if (CURRENT_STATE.FLAG_N == 1) {
        // Realiza el salto a la dirección calculada
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4

        // Extensión de signo si el bit 18 (más significativo de imm19) es 1
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }

        NEXT_STATE.PC = CURRENT_STATE.PC + offset;

    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_greater_equal(decoded_instruction decoded_opcode) {
    if (CURRENT_STATE.FLAG_N == 0) {
        // Realiza el salto a la dirección calculada
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4

        // Extensión de signo si el bit 18 (más significativo de imm19) es 1
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }

        NEXT_STATE.PC = CURRENT_STATE.PC + offset;

    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void B_less_equal(decoded_instruction decoded_opcode) {
    if (CURRENT_STATE.FLAG_N == 1 || CURRENT_STATE.FLAG_Z == 1) {
        // Realiza el salto a la dirección calculada
        int32_t offset = decoded_opcode.cond_branch_address << 2; // Se multiplica por 4

        // Extensión de signo si el bit 18 (más significativo de imm19) es 1
        if (offset & 0x40000) {
            offset |= 0xFFFC0000; // Sign-extend
        }

        NEXT_STATE.PC = CURRENT_STATE.PC + offset;

    } else {
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void ADD_Extended(decoded_instruction decoded_opcode) {
    // Realiza la suma entre los registros rn y rm
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] + (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rm];

    // Almacena el resultado en el registro de destino rd
    CURRENT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void ADD_Immediate(decoded_instruction decoded_opcode) {
    // Realiza la suma entre el registro y el valor inmediato
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] + (uint64_t)decoded_opcode.ALU_immediate;

    // Almacena el resultado en el registro de destino
    CURRENT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;
}

void MOVZ(decoded_instruction decoded_opcode) {
    // Caso hw = 0 (sin desplazamiento)
    CURRENT_STATE.REGS[decoded_opcode.rd] = decoded_opcode.MOV_inmediate;

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;

    // Mensaje opcional para depuración
    printf("MOVZ: X%d = %d\n", decoded_opcode.rd, decoded_opcode.MOV_inmediate);
}

void LDURB(decoded_instruction decoded_opcode) {
    // Calcula la dirección de memoria: base (X2) + desplazamiento inmediato (DT_address)
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;

    // Lee un byte desde la memoria
    uint8_t byte_value = mem_read_32(address) & 0xFF; // Solo los primeros 8 bits

    // Almacena el valor en el registro destino (W1), con los 56 bits superiores en cero
    CURRENT_STATE.REGS[decoded_opcode.rt] = (uint64_t)byte_value;

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;

    // Mensaje opcional para depuración
    printf("LDURB: Loaded byte 0x%02x from address 0x%016" PRIx64 " into W%d\n", byte_value, address, decoded_opcode.rt);
}

void LDUR(decoded_instruction decoded_opcode) {
    // Calcula la dirección de memoria: base (X2) + desplazamiento inmediato (DT_address)
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;

    // Lee un valor de 64 bits desde la memoria
    uint64_t value = mem_read_32(address); // Lee 4 bytes (32 bits)
    value |= ((uint64_t)mem_read_32(address + 4)) << 32; // Lee los siguientes 4 bytes y combina

    // Almacena el valor en el registro destino (X1)
    CURRENT_STATE.REGS[decoded_opcode.rt] = value;

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;

    // Mensaje opcional para depuración
    printf("LDUR: Loaded 0x%016" PRIx64 " from address 0x%016" PRIx64 " into X%d\n", value, address, decoded_opcode.rt);
}

void LDURH(decoded_instruction decoded_opcode) {
    // Calcula la dirección de memoria: base (X2) + desplazamiento inmediato (DT_address)
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;

    // Lee 2 bytes (16 bits) desde la memoria
    uint16_t halfword_value = mem_read_32(address) & 0xFFFF; // Solo los primeros 16 bits

    // Almacena el valor en el registro destino (W1), con los 48 bits superiores en cero
    CURRENT_STATE.REGS[decoded_opcode.rt] = (uint64_t)halfword_value;

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;

    // Mensaje opcional para depuración
    printf("LDURH: Loaded halfword 0x%04x from address 0x%016" PRIx64 " into W%d\n", halfword_value, address, decoded_opcode.rt);
}

void STURH(decoded_instruction decoded_opcode) {
    // Calcula la dirección de memoria: base (X2) + desplazamiento inmediato (DT_address)
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;

    // Obtiene los primeros 16 bits del registro fuente (W1)
    uint16_t halfword_value = CURRENT_STATE.REGS[decoded_opcode.rt] & 0xFFFF;

    // Divide el valor en dos bytes para almacenarlos en formato little-endian
    uint8_t lower_byte = halfword_value & 0xFF;         // Byte menos significativo
    uint8_t upper_byte = (halfword_value >> 8) & 0xFF; // Byte más significativo

    // Escribe los bytes en la memoria
    mem_write_32(address, (mem_read_32(address) & 0xFFFFFF00) | lower_byte);       // Escribe el byte menos significativo
    mem_write_32(address + 1, (mem_read_32(address + 1) & 0xFFFFFF00) | upper_byte); // Escribe el byte más significativo

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;

    // Mensaje opcional para depuración
    printf("STURH: Stored halfword 0x%04x from W%d into address 0x%016" PRIx64 "\n", halfword_value, decoded_opcode.rt, address);
}

void STURB(decoded_instruction decoded_opcode) {
    // Calcula la dirección de memoria: base (X2) + desplazamiento inmediato (DT_address)
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;

    // Obtiene los primeros 8 bits del registro fuente (X1)
    uint8_t byte_value = CURRENT_STATE.REGS[decoded_opcode.rt] & 0xFF;

    // Escribe el byte en la memoria (little-endian)
    mem_write_32(address, (mem_read_32(address) & 0xFFFFFF00) | byte_value);

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;

    // Mensaje opcional para depuración
    printf("STURB: Stored byte 0x%02x from X%d into address 0x%016" PRIx64 "\n", byte_value, decoded_opcode.rt, address);
}

void STUR(decoded_instruction decoded_opcode) {
    // Calcula la dirección de memoria: base (X2) + desplazamiento inmediato (DT_address)
    uint64_t address = CURRENT_STATE.REGS[decoded_opcode.rn] + decoded_opcode.DT_address;

    // Obtiene el valor de 64 bits del registro fuente (X1)
    uint64_t value = CURRENT_STATE.REGS[decoded_opcode.rt];

    // Divide el valor en dos partes de 32 bits para almacenarlo en memoria
    uint32_t lower_word = value & 0xFFFFFFFF;         // Parte menos significativa (32 bits)
    uint32_t upper_word = (value >> 32) & 0xFFFFFFFF; // Parte más significativa (32 bits)

    // Escribe los 64 bits en la memoria (little-endian)
    mem_write_32(address, lower_word);       // Escribe los primeros 32 bits
    mem_write_32(address + 4, upper_word);   // Escribe los siguientes 32 bits

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;

    // Mensaje opcional para depuración
    printf("STUR: Stored 0x%016" PRIx64 " from X%d into address 0x%016" PRIx64 "\n", value, decoded_opcode.rt, address);
}

void shift(decoded_instruction decoded_opcode) {
    // Realiza el desplazamiento lógico a la derecha
    uint32_t imms = (decoded_opcode.ALU_immediate & 0b111111); //bit 0 a 5 del ALU_immediate
    uint32_t immr = (decoded_opcode.ALU_immediate >> 6); //bit 6 a 11 del ALU_immediate

    if(imms == 63){
        NEXT_STATE.REGS[decoded_opcode.rd] = CURRENT_STATE.REGS[decoded_opcode.rn] >> immr; 
        NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[decoded_opcode.rd] < 0) ? 1 : 0;  
        NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[decoded_opcode.rd] == 0) ? 1 : 0;
    }
    else{
    uint32_t neg_imms = ~(imms);
    NEXT_STATE.REGS[decoded_opcode.rd] = CURRENT_STATE.REGS[decoded_opcode.rn] << neg_imms;
    NEXT_STATE.FLAG_N = (NEXT_STATE.REGS[decoded_opcode.rd] < 0) ? 1 : 0;  
    NEXT_STATE.FLAG_Z = (NEXT_STATE.REGS[decoded_opcode.rd] == 0) ? 1 : 0;
    printf("Valor de neg_imms: %d\n", neg_imms);
    printf("Valor de imms: %d\n", imms);
    }
}

void BLE(decoded_instruction decoded_opcode) {
    // Verifica los flags para determinar si X1 <= X2

    if (NEXT_STATE.FLAG_Z == 1 || NEXT_STATE.FLAG_N == 1) {
        // Realiza el salto a la dirección calculada
        int32_t offset = (int32_t)(decoded_opcode.cond_branch_address << 13) >> 13; // Sign-extend el desplazamiento
        CURRENT_STATE.PC += (offset << 2); // El desplazamiento es en palabras, se multiplica por 4

        // Mensaje opcional para depuración
        printf("BLE: Branching to address 0x%016" PRIx64 "\n", CURRENT_STATE.PC);
    } else {
        // Incrementa el contador de programa si no se cumple la condición
        CURRENT_STATE.PC += 4;

        // Mensaje opcional para depuración
        printf("BLE: Condition not met, continuing to next instruction.\n");
    }
}

void MUL(decoded_instruction decoded_opcode) {
    // Realiza la multiplicación entre los registros rn y rm
    uint64_t result = (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rn] * (uint64_t)CURRENT_STATE.REGS[decoded_opcode.rm];

    // Almacena el resultado en el registro de destino rd
    NEXT_STATE.REGS[decoded_opcode.rd] = (uint32_t)result;

    // Incrementa el contador de programa
    CURRENT_STATE.PC += 4;

    // Mensaje opcional para depuración
    printf("MUL: X%d = X%d * X%d = 0x%016" PRIx64 "\n", decoded_opcode.rd, decoded_opcode.rn, decoded_opcode.rm, result);
}

void CBZ(decoded_instruction decoded_instruction) {
    int32_t offset = decoded_instruction.cond_branch_address << 2; // Se multiplica por 4

    // Extensión de signo si el bit 18 (más significativo de imm19) es 1
    if (offset & 0x40000) {
        offset |= 0xFFFC0000; // Sign-extend
    }

    if (CURRENT_STATE.REGS[decoded_instruction.rt] == 0b0000) {
        // Realiza el salto a la dirección calculada
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        // Incrementa el contador de programa si no se cumple la condición
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}

void CBNZ(decoded_instruction decoded_instruction) {
    int32_t offset = decoded_instruction.cond_branch_address << 2; // Se multiplica por 4

    // Extensión de signo si el bit 18 (más significativo de imm19) es 1
    if (offset & 0x40000) {
        offset |= 0xFFFC0000; // Sign-extend
    }

    if (CURRENT_STATE.REGS[decoded_instruction.rt] != 0b0000) {
        // Realiza el salto a la dirección calculada
        NEXT_STATE.PC = CURRENT_STATE.PC + offset;
    } else {
        // Incrementa el contador de programa si no se cumple la condición
        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
    }
}