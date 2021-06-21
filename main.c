#include <stdio.h>
#include "include/cpu.h"
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
int main(void){
    Log("CPU Started\n");
    int state = 0;
    while((state = cpu_run_once()) == 0) {

    }
}
