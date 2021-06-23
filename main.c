#include <stdio.h>
#include "include/cpu.h"
extern uint32_t memory[];
extern riscv32_CPU_state cpu;
extern void init_memory(char*);
extern int cpu_run_once();
extern void print_reg_state();
int main(void){
    memory[0] = 0x00300093;
    memory[1] = 0x73;
    cpu.npc = 0;
    init_memory("addi.bin");
    Log("CPU Started\n");
    int state = 0;
    cpu.pc = 0;
    while (1) {
        cpu_run_once();
    }
    print_reg_state();
    return 0;
}
