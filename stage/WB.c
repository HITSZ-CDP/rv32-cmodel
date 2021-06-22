#include "../include/cpu.h"
extern riscv32_CPU_state cpu;
int WB(MEM2WB mem_info) {
    if(mem_info.branch_taken) {
        cpu.pc = mem_info.target_pc;
    }
    uint32_t wb_val;
    if(mem_info.wb_en) {
        switch(mem_info.wb_sel) {
            case WB_ALU: wb_val = mem_info.alu_out; break;
            case WB_PC: wb_val = mem_info.pc + 4; break;
            case WB_LOAD: wb_val = mem_info.load_out; break;
            default: wb_val = 0; break;
        }
        cpu.gpr[mem_info.dst] = wb_val;
    }
    cpu.gpr[0] = 0;
    // TODO: if ecall, return 1;
    Log("========WB Stage=======\n");
    Log("PC = %8.8x\n", mem_info.pc);
    if(mem_info.wb_en) {
        Log("WB value = %8.8x, WReg = %d\n", wb_val, mem_info.dst);
    }
    if(mem_info.branch_taken) {
        Log("Branch Taken, target is %8.8x\n", mem_info.target_pc);
    }

    return 0;
}
