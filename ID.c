#include "cpu.h"
#include "bin.h"
#define pair(x, y) (((x) << 3) | (y))
#define PAIR_ENTRY(x, y, OP) case pair(x,y): ret.alu_op = OP; break
extern riscv32_CPU_state cpu;
ID2EX ID_R(IF2ID inst) {
    // 10 instructions
    //ADD/SLT/SLTU/AND/OR/XOR/SLL/SRL/SUB/SRA
    ID2EX ret;
    ret.inst_raw_split.inst_raw = inst.inst;
    ret.next_pc = inst.pc + 4;
    ret.pc = inst.pc;
    ret.inst = inst.inst;

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

ID2EX ID_I_LOAD(IF2ID inst) {
    // LOAD
    ID2EX ret;
    ret.inst_raw_split.inst_raw = inst.inst;
    switch (ret.inst_raw_split.i.funct3) {
        case 0: ret.mem_op = MEM_LB; break;
        case 1: ret.mem_op = MEM_LH; break;
        case 2: ret.mem_op = MEM_LW; break;
        case 4: ret.mem_op = MEM_LBU; break;
        case 5: ret.mem_op = MEM_LHU; break;
        default: ret.mem_op = MEM_LW; break;
    }

    ret.next_pc = inst.pc + 4;

    ret.is_jmp = 0;
    ret.is_branch = 0;
    ret.is_mem = 1;

    ret.src1.type = OP_TYPE_REG;
    ret.src1.value = cpu.gpr[ret.inst_raw_split.r.rs1];

    ret.src2.type = OP_TYPE_IMM;
    ret.src2.value = ret.inst_raw_split.i.simm11_0;
    
    ret.dst = ret.inst_raw_split.r.rd;
    ret.wb_en = 1;

    return ret;
}

ID2EX ID_I(IF2ID inst) {
    // 9 instructions
    // ADDI/SLTI/SLTIU/ANDI/ORI/XORI
    // SLLI/SRLI/SRAI
    ID2EX ret;
    ret.inst_raw_split.inst_raw = inst.inst;
    ret.next_pc = inst.pc + 4;

    switch (ret.inst_raw_split.i.funct3) {
        case 0: ret.alu_op = OP_ADD; break;
        case 1: ret.alu_op = OP_SLL; break;
        case 2: ret.alu_op = OP_SLT; break;
        case 3: ret.alu_op = OP_SLTU; break;
        case 4: ret.alu_op = OP_XOR; break;
        case 5: if(ret.inst_raw_split.s.simm11_5 == 0) ret.alu_op = OP_SRL;
                else ret.alu_op = OP_SRA;
                break;
        case 6: ret.alu_op = OP_AND; break;
    }

    ret.is_jmp = 0;
    ret.is_branch = 0;
    ret.is_mem = 0;

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
    ret.inst_raw_split.inst_raw = inst.inst;
    ret.next_pc = inst.pc + 4;

    ret.is_jmp = 0;
    ret.is_branch = 0;
    ret.is_mem = 1;

    ret.alu_op = OP_ADD;

    switch (ret.inst_raw_split.s.funct3) {
        case 0: ret.mem_op = MEM_SB; break;
        case 1: ret.mem_op = MEM_SH; break;
        case 2: ret.mem_op = MEM_SW; break;
        default: ret.mem_op = MEM_SW; break;
    }

    ret.src1.type = OP_TYPE_REG;
    ret.src1.value = cpu.gpr[ret.inst_raw_split.r.rs1];

    ret.src2.type = OP_TYPE_IMM;
    ret.src2.value = ret.inst_raw_split.i.simm11_0;  // TODO: concat the imm here

    ret.store_val = cpu.gpr[ret.inst_raw_split.r.rs2];
    
    ret.wb_en = 0;

    return ret;
}

ID2EX ID_B(IF2ID inst) {
    // 6 instructions
    // BEQ/BNE/BLT/BLTU/BGE/BGEU
    ID2EX ret;
    ret.inst_raw_split.inst_raw = inst.inst;
    ret.next_pc = inst.pc + 4; // TODO: calculate next_pc here

    ret.is_jmp = 0;
    ret.is_branch = 1;
    ret.is_mem = 0;

    switch(ret.inst_raw_split.b.funct3) {
        case 0: ret.br_op = BR_EQ; break;
        case 1: ret.br_op = BR_NEQ ; break;
        case 4: ret.br_op = BR_LT; break;
        case 5: ret.br_op = BR_GE; break;
        case 6: ret.br_op = BR_LTU; break;
        case 7: ret.br_op = BR_GEU; break;
    }

    ret.src1.type = OP_TYPE_REG;
    ret.src1.value = cpu.gpr[ret.inst_raw_split.r.rs1];

    ret.src2.type = OP_TYPE_REG;
    ret.src2.value = cpu.gpr[ret.inst_raw_split.r.rs2];

    ret.wb_en = 0;

    return ret;
}

ID2EX ID_U(IF2ID inst) {
    // 2 instructions
    // LUI/AUIPC
    ID2EX ret;
    ret.inst_raw_split.inst_raw = inst.inst;
    ret.next_pc = inst.pc + 4;

    ret.is_jmp = 0;
    ret.is_branch = 0;
    ret.is_mem = 0;

    if(ret.inst_raw_split.u.opcode6_2 == 6) {
        ret.src1.value = 0;
    } else {
        ret.src1.value = inst.pc;
    }

    ret.src2.value = ret.inst_raw_split.u.imm31_12; // TODO: Shift the imm
    ret.alu_op = OP_ADD;
    
    ret.dst = ret.inst_raw_split.u.rd;
    ret.wb_en = 1;

    return ret;
}

ID2EX ID_J(IF2ID inst) {
    // JAL/JALR
    ID2EX ret;
    ret.inst_raw_split.inst_raw = inst.inst;

    ret.is_jmp = 1;
    ret.is_branch = 0;
    ret.is_mem = 0;

    if(ret.inst_raw_split.j.opcode6_2 == 13) {  // JAL 
        ret.next_pc = inst.pc + 0; // TODO: Make the imm
    } else {
        ret.next_pc = cpu.gpr[ret.inst_raw_split.r.rs1];
    }

    ret.dst = ret.inst_raw_split.j.rd;
    ret.wb_en = 1;

    return ret;
}

ID2EX ID(IF2ID inst) {
    ID2EX ret;
    ret.inst_raw_split.inst_raw = inst.inst;
    switch( ((ret.inst_raw_split.r.opcode6_2) << 3) | (ret.inst_raw_split.r.opcode1_0) ) { // funct7
        case B8(00110111):
        case B8(00010111):
            ret = ID_U(inst);
            break;
        case B8(01101111):
        case B8(01100111):
            ret = ID_J(inst);
            break;
        case B8(01100011):
            ret = ID_B(inst);
            break;
        case B8(00000011):
            ret = ID_I_LOAD(inst);
            break;
        case B8(00100011):
            ret = ID_S(inst);
            break;
        case B8(00010011):
            ret = ID_I(inst);
            break;
        case B8(00110011):
            ret = ID_R(inst);
            break;
        default:
            ret.alu_op = OP_INVALID;
            break;
    }
    return ret;
}

