#ifndef __CPU__
#define __CPU__
#include <stdint.h>
typedef struct {
  uint32_t gpr[32]; 
  uint32_t pc;
} riscv32_CPU_state;

typedef struct {
    uint32_t inst;
    uint32_t pc;
} IF2ID;

typedef enum { OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM } op_type;
typedef enum { R, I, S, B, U, J } inst_format;
typedef enum { IMM_I, IMM_S, IMM_B, IMM_U, IMM_J } imm_format;
typedef enum { MEM_BYTE, MEM_HALF, MEM_WORD } mem_width;

typedef struct {
    op_type type;
    uint32_t value;
} Operand;

typedef enum { OP_ADD, OP_SLT, OP_SLTU, OP_AND, OP_OR, OP_XOR, OP_SLL, OP_SRL, OP_SUB, OP_SRA, OP_INVALID } alu_op;
typedef enum { MEM_LB, MEM_LBU, MEM_LH, MEM_LHU, MEM_LW, MEM_SB, MEM_SH, MEM_SW } mem_op;
typedef enum { BR_EQ, BR_NEQ, BR_GE, BR_GEU, BR_LT, BR_LTU, JUMP } br_op;

#include "riscv32_instdef.h"
typedef struct {
    Decodeinfo_raw inst_raw_split;
    alu_op alu_op;
    mem_op mem_op;
    br_op  br_op;
    uint32_t next_pc;
    uint32_t is_jmp;
    uint32_t is_branch;
    uint32_t is_mem;
    Operand src1, src2;
    uint32_t dst;
    uint32_t wb_en;
} ID2EX;

#endif
