#include <stdio.h>
#include <stdlib.h>

#include "disassemble.h"
int power(int a, int b) {
  if (b < 0) {
    printf("negative be and I don't know what?! ");
  }
  if (b == 0) {
    return 1;
  }
  return a * power(a, b - 1);
}
signed int signext(unsigned int imm) {
  int k = 0; int j;
  if (imm > power(2, 15)) {
    for (j = 16; j < 32; j += 1) {
      k += power(2, j);
    }
    return imm | k;
  }
  return imm | k;
}
void disassemble(inst_t inst)
{
  /* you could use itype or type instead of rtype */
  /* remember that opcode field is at the same place for all types */
  switch (inst.rtype.opcode)
    {
    case 0x0: // opcode == 0x0 (SPECIAL)

      switch (inst.rtype.funct)
        {
        case 0xc: // funct == 0xc (SYSCALL)
          printf("syscall\n");
          break;

        case 0x25: // funct == 0x25 (OR)
          printf("or\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
          break;

        case 0x0: // funct == 0x25 (OR)
          printf("sll\t$%d,$%d,%u\n", inst.rtype.rd, inst.rtype.rt, inst.rtype.shamt);
          break; 

        case 0x2: // funct == 0x25 (OR)
          printf("srl\t$%d,$%d,%u\n", inst.rtype.rd, inst.rtype.rt, inst.rtype.shamt);
          break;

        case 0x8:// funct == 0x25 (OR)
          printf("jr\t$%d\n", inst.rtype.rs);
          break;

        case 0x9: // funct == 0x25 (OR)
          printf("jalr\t$%d,$%d\n", inst.rtype.rd, inst.rtype.rs);
          break;

        case 0x21: // funct == 0x25 (OR)
          printf("addu\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
          break;

        case 0x23: // funct == 0x25 (OR)
          printf("subu\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
          break;

        case 0x24: // funct == 0x25 (OR)
          printf("and\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
          break;

        case 0x26: // funct == 0x25 (OR)
          printf("xor\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
          break;

        case 0x27: // funct == 0x25 (OR)
          printf("nor\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
          break;

        case 0x2a: // funct == 0x25 (OR)
          printf("slt\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
          break;
        case 0x2b: // funct == 0x25 (OR)
          printf("sltu\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
          break;

        case 0x3: // funct == 0x25 (OR)
          printf("sra\t$%d,$%d,%u\n", inst.rtype.rd, inst.rtype.rt, inst.rtype.shamt);
          break;

        default: // undefined funct
          fprintf(stderr, "%s: What is a %08x\n", __FUNCTION__, inst.bits);
          exit(-1);
          break;
        }
      break;

    case 0xd: // opcode == 0xd (ORI)
      printf("ori\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
      break;

    case 0x4: // opcode == 0xd (ORI)
      printf("beq\t$%d,$%d,%d\n", inst.itype.rs, inst.itype.rt, signext(inst.itype.imm) * 4);
      break;

    case 0x5: // opcode == 0xd (ORI)
      printf("bne\t$%d,$%d,%d\n", inst.itype.rs, inst.itype.rt, signext(inst.itype.imm) * 4);
      break;

    case 0x9: // opcode == 0xd (ORI)
      printf("addiu\t$%d,$%d,%d\n", inst.itype.rt, inst.itype.rs, signext(inst.itype.imm));
      break;

    case 0xa: // opcode == 0xd (ORI)
      printf("slti\t$%d,$%d,%d\n", inst.itype.rt, inst.itype.rs, signext(inst.itype.imm));
      break;

    case 0xb: // opcode == 0xd (ORI)
      printf("sltiu\t$%d,$%d,%d\n", inst.itype.rt, inst.itype.rs, signext(inst.itype.imm));
      break;

    case 0xc: // opcode == 0xd (ORI)
      printf("andi\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
      break;

    case 0xe: // opcode == 0xd (ORI)
      printf("xori\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
      break;

    case 0xf: // opcode == 0xd (ORI)
      printf("lui\t$%d,0x%x\n", inst.itype.rt, inst.itype.imm);
      break;

    case 0x20: // opcode == 0xd (ORI)
      printf("lb\t$%d,%d($%d)\n", inst.itype.rt, signext(inst.itype.imm), inst.itype.rs);
      break;

    case 0x21: // opcode == 0xd (ORI)
      printf("lh\t$%d,%d($%d)\n", inst.itype.rt, signext(inst.itype.imm), inst.itype.rs);
      break;

    case 0x23: // opcode == 0xd (ORI)
      printf("lw\t$%d,%d($%d)\n", inst.itype.rt, signext(inst.itype.imm), inst.itype.rs);
      break;

    case 0x24: // opcode == 0xd (ORI)
      printf("lbu\t$%d,%d($%d)\n", inst.itype.rt, signext(inst.itype.imm), inst.itype.rs);
      break;

    case 0x25: // opcode == 0xd (ORI)
      printf("lhu\t$%d,%d($%d)\n", inst.itype.rt, signext(inst.itype.imm), inst.itype.rs);
      break;

    case 0x28: // opcode == 0xd (ORI)
      printf("sb\t$%d,%d($%d)\n", inst.itype.rt, signext(inst.itype.imm), inst.itype.rs);
      break;

    case 0x29: // opcode == 0xd (ORI)
      printf("sh\t$%d,%d($%d)\n", inst.itype.rt, signext(inst.itype.imm), inst.itype.rs);
      break;

    case 0x2b: // opcode == 0xd (ORI)
      printf("sw\t$%d,%d($%d)\n", inst.itype.rt, signext(inst.itype.imm), inst.itype.rs);
      break;


    case 0x2: // opcode == 0x2 (J)
      printf("j\t0x%x\n", inst.jtype.addr*4);
      break;

    case 0x3: // opcode == 0x2 (J)
      printf("jal\t0x%x\n", inst.jtype.addr*4);
      break;

    default: // undefined opcode
      fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, inst.bits);
      exit(-1);
      break;
    }
}
