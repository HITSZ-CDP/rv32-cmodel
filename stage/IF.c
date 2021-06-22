#include <stdio.h>
#include "../include/cpu.h"
extern riscv32_CPU_state cpu;
extern uint32_t memory[];

IF2ID IF(uint32_t pc) {
    IF2ID ret;
    Assert(pc < MEM_SZ, "PC out of boundary!\n");
    ret.inst = memory[pc >> 2];
    ret.pc = pc;
    Log("Fetched instruction 0x%8.8x at PC=0x%8.8x", ret.inst, ret.pc);
    return ret;
}
