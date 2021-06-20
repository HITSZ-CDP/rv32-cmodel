#include "cpu.h"
#define pair(x, y) (((x) << 3) | (y))
#define PAIR_ENTRY(x, y, OP) case pair(x,y): ret.alu_op = OP; break
extern riscv32_CPU_state cpu;
ID2EX ID(IF2ID inst) {
    ID2EX ret;
    ret.inst_raw_split.inst_raw = inst.inst;
    return ret;
}

ID2EX ID_R(IF2ID inst) {
    // 10 instructions
    //ADD/SLT/SLTU/AND/OR/XOR/SLL/SRL/SUB/SRA
    ID2EX ret;
    ret.inst_raw_split.inst_raw = inst.inst;
    switch (pair(ret.inst_raw_split.r.funct7, ret.inst_raw_split.r.funct3)) {
        PAIR_ENTRY(0, 0, OP_ADD);
        PAIR_ENTRY(0, 1, OP_SLL);
        PAIR_ENTRY(0, 2, OP_SLT);
        PAIR_ENTRY(0, 3, OP_SLTU);
        PAIR_ENTRY(0, 4, OP_XOR);
        PAIR_ENTRY(0, 5, OP_SRL);
        PAIR_ENTRY(0, 6, OP_OR);
        PAIR_ENTRY(0, 7, OP_AND);
        PAIR_ENTRY(32, 0, OP_SUB);
        PAIR_ENTRY(32, 5, OP_SRA);
        default: ret.alu_op = OP_INVALID;
    }

    ret.next_pc = inst.pc + 4;

    ret.is_jmp = 0;
    ret.is_branch = 0;
    ret.is_mem = 0;

    ret.src1.type = OP_TYPE_REG;
    ret.src1.value = cpu.gpr[ret.inst_raw_split.r.rs1];

    ret.src2.type = OP_TYPE_REG;
    ret.src2.value = cpu.gpr[ret.inst_raw_split.r.rs2];
    
    ret.dst = ret.inst_raw_split.r.rd;

    ret.wb_en = 1;

    return ret;
}

ID2EX ID_I(IF2ID inst) {
    // 10 instructions
    // ADDI/SLTI/SLTIU/ANDI/ORI/XORI
    // SLLI/SRLI/SRAI
    // LOAD
    ID2EX ret;
    //todo ret.opcode = 

    ret.next_pc = inst.pc + 4;

    ret.is_jmp = 0;
    ret.is_branch = 0;
    ret.is_mem = 0;

    ret.inst_raw_split.inst_raw = inst.inst;

    ret.src1.type = OP_TYPE_REG;
    ret.src1.value = cpu.gpr[ret.inst_raw_split.r.rs1];

    ret.src2.type = OP_TYPE_IMM;
    ret.src2.value = ret.inst_raw_split.i.simm11_0;
    
    ret.dst = ret.inst_raw_split.r.rd;
    ret.wb_en = 1;

    return ret;
}

ID2EX ID_S(IF2ID inst) {
    // 1 instruction
    // STORE
    ID2EX ret;

    ret.next_pc = inst.pc + 4;
    ret.is_jmp = 0;
    ret.is_branch = 0;
    ret.is_mem = 1;
    ret.alu_op = OP_ADD;

    return ret;
}

ID2EX ID_B(IF2ID inst) {
    // 6 instructions
    // BEQ/BNE/BLT/BLTU/BGE/BGEU
    ID2EX ret;
    return ret;
}

ID2EX ID_U(IF2ID inst) {
    // 2 instructions
    // LUI/AUIPC
    ID2EX ret;
    return ret;
}

ID2EX ID_J(IF2ID inst) {
    // 2 instructions
    // JAL/JALR
    ID2EX ret;
    return ret;
}
