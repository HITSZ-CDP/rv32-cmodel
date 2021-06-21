#include <stdio.h>
#include "../include/cpu.h"
extern riscv32_CPU_state cpu;
extern uint32_t memory[];

IF2ID IF(uint32_t pc) {
    IF2ID ret;
    Assert(pc < MEM_SZ, "PC out of boundary!\n");
    ret.inst = memory[pc >> 2];
    ret.pc = pc;
    return ret;
}
