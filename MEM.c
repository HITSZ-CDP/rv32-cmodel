#include "cpu.h"
#include <stdint.h>
extern struct riscv32_CPU_state cpu;
extern uint32_t memory[];

MEM2WB MEM(EX2MEM ex_info) {
    MEM2WB ret;
    // passthrough
    ret.alu_out = ex_info.alu_out;
    ret.wb_sel = ex_info.wb_sel;
    ret.wb_en = ex_info.wb_en;
    ret.dst = ex_info.dst;
    ret.branch_taken = ex_info.branch_taken;
    ret.target_pc = ex_info.target_pc;
    ret.inst = ex_info.inst;
    ret.pc = ex_info.pc;
    // TODO: ret.load_out
    // TODO: if store?

    return ret;
}
