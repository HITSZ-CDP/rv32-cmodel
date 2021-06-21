#include <stdio.h>
#include <stdint.h>
#include "../include/cpu.h"
extern struct riscv32_CPU_state cpu;

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
    ret.mem_op = decode_info.mem_op;

    uint32_t alu_result;
    uint32_t src1 = decode_info.src1.value;
    uint32_t src2 = decode_info.src2.value;
    switch(decode_info.alu_op) {
        case OP_ADD: alu_result = src1 + src2;
        // TODO
        default: alu_result = 0;
    }
    ret.alu_out = alu_result;

    uint32_t br_cond = 0;
    if(decode_info.is_branch) {
        switch(decode_info.br_op) {
            case BR_EQ: br_cond = (src1 == src2);
                        // TODO
            default: br_cond = 0;
        }
    } else if (decode_info.is_jmp) {
        br_cond = 1;
    }
    ret.branch_taken = br_cond;

    return ret;
}
