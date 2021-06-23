#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../include/cpu.h"
extern struct riscv32_CPU_state cpu;
extern void print_reg_state();
#define ALU_CASE_ENTRY(op, calc) case op: alu_result = (calc); break
#define BR_CASE_ENTRY(op, calc) case op: br_result = (calc); break

EX2MEM EX(ID2EX decode_info) {
    EX2MEM ret;
    // Passthrough
    ret.inst = decode_info.inst;
    ret.pc = decode_info.pc;
    ret.wb_sel = decode_info.wb_sel;
    ret.wb_en = decode_info.wb_en;
    ret.dst = decode_info.dst;
    ret.target_pc = decode_info.next_pc; // evaluated in ID
    ret.store_val = decode_info.store_val;
    ret.is_mem = decode_info.is_mem;
    ret.mem_op = decode_info.mem_op;

    uint32_t alu_result;
    uint32_t src1 = decode_info.src1.value;
    uint32_t src2 = decode_info.src2.value;
    switch(decode_info.alu_op) {
        case OP_INVALID: 
            panic("Invalid Insturction %8.8x at PC=%8.8x\n", decode_info.inst, decode_info.pc); break;
        case OP_ECALL:
            color_print("ECALL at PC = 0x%8.8x, Stop now.\n", decode_info.pc);
            print_reg_state();
            exit(0);
            ALU_CASE_ENTRY(OP_ADD, src1 + src2);
            ALU_CASE_ENTRY(OP_SLT, (int32_t)src1 < (int32_t)src2);
            ALU_CASE_ENTRY(OP_SLTU, src1 < src2);
            ALU_CASE_ENTRY(OP_AND, src1 & src2);
            ALU_CASE_ENTRY(OP_OR, src1 | src2);
            ALU_CASE_ENTRY(OP_XOR, src1 ^ src2);
            ALU_CASE_ENTRY(OP_SLL, src1 << src2);
            ALU_CASE_ENTRY(OP_SRL, src1 >> src2);
            ALU_CASE_ENTRY(OP_SUB, src1 - src2);
            ALU_CASE_ENTRY(OP_SRA, (int32_t)src1 >> src2);
        default: alu_result = 0;
    }
    ret.alu_out = alu_result;
    Log("ALU Result = %8.8x\n", alu_result);

    uint32_t br_result = 0;
    if(decode_info.is_branch) {
        switch(decode_info.br_op) {
            BR_CASE_ENTRY(BR_EQ, src1 == src2);
            BR_CASE_ENTRY(BR_NEQ, src1 != src2);
            BR_CASE_ENTRY(BR_GE, (int32_t)src1 >= (int32_t)src2);
            BR_CASE_ENTRY(BR_GEU, src1 >= src2);
            BR_CASE_ENTRY(BR_LT, (int32_t)src1 < (int32_t)src2);
            BR_CASE_ENTRY(BR_LTU, src1 < src2);
            default: br_result = 0; break;
        }
    } else if (decode_info.is_jmp) {
        br_result = 1;
    }
    ret.branch_taken = br_result;

    return ret;
}
