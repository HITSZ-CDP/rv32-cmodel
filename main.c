#include <stdio.h>
#include "cpu.h"
riscv32_CPU_state cpu;
uint32_t memory[1024];
extern IF2ID IF(uint32_t);
extern ID2EX ID(IF2ID);
extern EX2WB EXMEM(ID2EX);
extern  void WB(EX2WB);
void cpu_run_once() {
    IF2ID inst = IF(cpu.pc);    
    ID2EX decode_info = ID(inst);
    EX2WB exmem_info = EXMEM(decode_info);
    WB(exmem_info);
}
int main(void){

}
