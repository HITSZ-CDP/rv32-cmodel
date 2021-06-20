#include <stdio.h>
#include "cpu.h"
extern riscv32_CPU_state cpu;
extern uint32_t memory[];

IF2ID IF(uint32_t pc) {
    IF2ID ret;
    ret.inst = memory[pc];
    ret.pc = pc;
    return ret;
}
