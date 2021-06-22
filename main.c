#include <stdio.h>
#include "include/cpu.h"
#include "include/debug.h"
riscv32_CPU_state cpu;
uint32_t memory[1024];
extern IF2ID IF(uint32_t);
extern ID2EX ID(IF2ID);
extern EX2MEM EX(ID2EX);
extern MEM2WB MEM(EX2MEM);
extern int WB(MEM2WB);
int cpu_run_once() {
    IF2ID inst = IF(cpu.pc);    
    ID2EX decode_info = ID(inst);
    EX2MEM ex_info = EX(decode_info);
    MEM2WB mem_info = MEM(ex_info);
    return WB(mem_info);
}
void print_reg_state(){
    color_print("======= REG VALUE =======\n");
    color_print("x[ 0] = 0x00000000\t");
    for(int i = 1; i < 32; i++) {
        if( (i % 4) == 0 ) printf("\n");
        color_print("x[%2d] = 0x%8.8x\t", i, cpu.gpr[i]);
    }
    printf("\n");
}
int main(void){
    memory[0] = 0x00300093;
    memory[1] = 0x73;
    Log("CPU Started\n");
    int state = 0;
    cpu.pc = 0;
    while (1) {
        cpu_run_once();
    }
    print_reg_state();
    return 0;
}
