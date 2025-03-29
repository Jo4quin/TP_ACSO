typedef struct {
    uint32_t opcode;
    int length;
    int type;  // 0 = register, 1 = immediate, 2 = data transfer, 3 = branch, 4 = conditional branch, 5 = inmediate wide
} tuple_t;

tuple_t opcode_list[27] = {
    {0b10001011001, 11, 0}, // ADD Extended
    {0b10101011001, 11, 0}, // ADDS Extended
    {0b10011011000, 11, 0}, // MUL (Scalar Multiplication)
    {0b11001011001, 11, 0}, // SUBS Extended
    {0b11101011001, 11, 0}, // CMP Extended
    {0b11101010000, 11, 0}, // ANDS Shifted
    {0b11001010000, 11, 0}, // EOR Shifted
    {0b11110001100, 11, 0}, // ORR Shifted
    {0b1001000100, 10, 1},  // ADD Immediate
    {0b1101001101, 10, 1},  // LSR Immediat                                       e
    {0b1101001101, 10, 1},  // LSL Immediate
    {0b1111000100, 10, 1},  // CMP Immediate
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

// Funciones para instrucciones
void arithmetic_instruction(decoded_instruction decoded_opcode);
void arithmetic_immediate_instruction(decoded_instruction decoded_opcode);
void data_transfer_instruction(decoded_instruction decoded_opcode);
void branch_instruction(decoded_instruction decoded_opcode);
void conditional_branch_instruction(decoded_instruction decoded_opcode);
void inmediate_wide_instruction(decoded_instruction decoded_opcode);

// Instrucciones 
void ADDS_Extended(decoded_instruction decoded_opcode);
void SUBS_Extended(decoded_instruction decoded_opcode);
void HLT(decoded_instruction decoded_opcode);
void CMP_Extended(decoded_instruction decoded_opcode);
void ANDS_Shifted(decoded_instruction decoded_opcode);
void ORR_Shifted(decoded_instruction decoded_opcode);
void EOR_Shifted(decoded_instruction decoded_opcode);
void ADD_Extended(decoded_instruction decoded_opcode);
void MOVZ(decoded_instruction decoded_opcode);
void LDURB(decoded_instruction decoded_opcode);
void LDUR(decoded_instruction decoded_opcode);
void LDURH(decoded_instruction decoded_opcode);
void STURH(decoded_instruction decoded_opcode);
void STURB(decoded_instruction decoded_opcode);
void STUR(decoded_instruction decoded_opcode);
void LSR_Immediate(decoded_instruction decoded_opcode);
void BLE(decoded_instruction decoded_opcode);

