#include <stdio.h>
#include "include/cpu.h"
extern uint32_t memory[];
extern riscv32_CPU_state cpu;
extern void init_memory(char*);
extern int cpu_run_once();
extern void print_reg_state();
int main(int argc, char* argv[]){
    cpu.npc = 0;
    if(argc != 2) {
        printf("Usage: ./emu xxx.bin\n");
        return -1;
    }
    init_memory(argv[1]);
    Log("CPU Started\n");
    int state = 0;
    cpu.pc = 0;
    while (1) {
        cpu_run_once();
    }
    print_reg_state();
    return 0;
}
