#include <stdio.h>
#include <stdlib.h>

#include "processor.h"
#include "disassemble.h"
signed int siggnext(unsigned int imm);
int powwer(int a, int b);
int powwer(int a, int b) {
  if (b < 0) {
    printf("%d", b);
    exit(-1);
  }
  if (b == 0) {
    return 1;
  }
  return a * powwer(a, b - 1);
}
/**Takes the sixteen bit imm and turns it into a 32-bit signed.**/
signed int siggnext(unsigned int imm) {
  int k = 0; int j;
  if (imm >= powwer(2, 15)) {
      for (j = 16; j < 32; j += 1) {
      k += powwer(2, j);
    }
    return imm | k;
  }
  return imm | k;
}
int siggnuxt(uint32_t a);
/**Takes the 32bit halfword a and turns it into crap.**/
int siggnuxt(uint32_t a) {
  int p;
  if (a >= powwer(2, 15)) {
    for (p = 16; p < 32; p += 1) {
      a |= powwer(2, p);
    }
  }
  return a;
}
int sijjnuext(uint32_t a);
int sijjnuext(uint32_t a) {
  int p;
  if (a >= powwer(2, 7)) {
    for (p = 8; p < 32; p += 1) {
      a |= powwer(2, p);
    }
  }
  return a;
}

void execute_one_inst(processor_t* p, int prompt, int print_regs) {
  int trash;
  inst_t inst;
  int temp;
  int tempp;
  int wtf;
  signed int wtff;
  /* fetch an instruction */
  inst.bits = load_mem(p->pc, SIZE_WORD);

  /* interactive-mode prompt */
  if(prompt)
    {
      if (prompt == 1) {
        printf("simulator paused, enter to continue...");
        while(getchar() != '\n')
          ;
      }
      printf("%08x: ",p->pc);
      disassemble(inst);
    }

  switch (inst.rtype.opcode) /* could also use e.g. inst.itype.opcode */
    {
    case 0x0: // opcode == 0x0 (SPECIAL)

      switch (inst.rtype.funct)
        {
        case 0xc: // funct == 0xc (SYSCALL)
          handle_syscall(p);
          p->pc += 4;
          break;

        case 0x25: // funct == 0x25 (OR)
          p->R[inst.rtype.rd] = p->R[inst.rtype.rs] | p->R[inst.rtype.rt];
          p->pc += 4;
          break;
        case 0x0: // funct == 0x25 (OR)
          wtf = (p->R[inst.rtype.rt] * powwer(2, inst.rtype.shamt));
          p->R[inst.rtype.rd] = wtf;
          p->pc += 4;
          break; 

        case 0x2: // funct == 0x25 (OR)
          p->R[inst.rtype.rd] = p->R[inst.rtype.rt] >> inst.rtype.shamt;
          p->pc += 4;
          break;

        case 0x8:// funct == 0x25 (OR)
          p->pc = p->R[inst.rtype.rs];
          break;

        case 0x9: // funct == 0x25 (OR)
          temp = p->pc + 4;
          p->pc = p->R[inst.rtype.rs];
          p->R[inst.rtype.rd] = temp;
          break;

        case 0x21: // funct == 0x25 (OR)
          p->R[inst.rtype.rd] = p->R[inst.rtype.rs] + p->R[inst.rtype.rt];
          p->pc += 4;
          break;

        case 0x23: // funct == 0x25 (OR)
          p->R[inst.rtype.rd] = p->R[inst.rtype.rs] - p->R[inst.rtype.rt];
          p->pc += 4;
          break;

        case 0x24: // funct == 0x25 (OR)
          p->R[inst.rtype.rd] = p->R[inst.rtype.rs] & p->R[inst.rtype.rt];
          p->pc += 4;
          break;

        case 0x26: // funct == 0x25 (OR)
          p->R[inst.rtype.rd] = p->R[inst.rtype.rs] ^ p->R[inst.rtype.rt];
          p->pc += 4;
          break;

        case 0x27: // funct == 0x25 (OR)
          p->R[inst.rtype.rd] = ~(p->R[inst.rtype.rs] | p->R[inst.rtype.rt]);
          p->pc += 4;
          break;

        case 0x2a: // funct == 0x25 (OR)
          p->R[inst.rtype.rd] = *(signed int*)&(p->R[inst.rtype.rs]) < *(signed int*)&(p->R[inst.rtype.rt]);
          p->pc += 4;
          break;
        case 0x2b: // funct == 0x25 (OR)
          p->R[inst.rtype.rd] = p->R[inst.rtype.rs] < p->R[inst.rtype.rt];
          p->pc += 4;
          break;

        case 0x3: // funct == 0x25 (OR)
          trash = (p->R[inst.rtype.rt] / powwer(2, inst.rtype.shamt));
          if (p->R[inst.rtype.rt] > powwer(2, 14)) {
            for (tempp = 0; tempp < inst.rtype.shamt; tempp += 1) {
              trash |= powwer(2, 31 - tempp);
            }
          }
          p->R[inst.rtype.rd] = trash;
          p->pc += 4;
          break;

        default: // undefined funct
          fprintf(stderr, "%s: pc=%08x, GARBEGE=%08x\n", __FUNCTION__, p->pc, inst.bits);
          exit(-1);
          break;
        }
      break;

    case 0xd: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = (p->R[inst.itype.rs] | inst.itype.imm);
      p->pc += 4;
      break;

    case 0x4: // opcode == 0xd (ORI)
      if (p->R[inst.itype.rs] == p->R[inst.itype.rt]) {
        p->pc = p->pc + 4 + 4 * siggnext(inst.itype.imm);
      } else {
        p->pc += 4;
      }
      break;

    case 0x5: // opcode == 0xd (ORI)
      if (p->R[inst.itype.rs] != p->R[inst.itype.rt]) {
        p->pc = p->pc + 4 + 4 * siggnext(inst.itype.imm);
      } else {
        p->pc += 4;
      }
      break;

    case 0x9: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = p->R[inst.itype.rs] + siggnext(inst.itype.imm);
      p->pc += 4;
      break;

    case 0xa: // opcode == 0xd (ORI)
      wtff = siggnext(inst.itype.imm);
      p->R[inst.itype.rt] = (*(signed int*)&(p->R[inst.itype.rs]) < *(signed int*)&wtff);
      p->pc += 4;
      break;

    case 0xb: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = *(signed int*)&(p->R[inst.itype.rs]) < inst.itype.imm;
      p->pc += 4;
      break;

    case 0xc: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = p->R[inst.itype.rs] & inst.itype.imm;
      p->pc += 4;
      break;

    case 0xe: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = p->R[inst.itype.rs] ^ inst.itype.imm;
      p->pc += 4;
      break;

    case 0xf: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = inst.itype.imm << 16;
      p->pc += 4;
      break;

    case 0x20: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = sijjnuext(load_mem(siggnext(inst.itype.imm) + p->R[inst.itype.rs], SIZE_BYTE));
      p->pc += 4;
      break;

    case 0x21: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = siggnuxt(load_mem(siggnext(inst.itype.imm) + p->R[inst.itype.rs], SIZE_HALF_WORD));
      p->pc += 4;
      break;

    case 0x23: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = load_mem(siggnext(inst.itype.imm) + p->R[inst.itype.rs], SIZE_WORD);
      p->pc += 4;
      break;

    case 0x24: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = load_mem(siggnext(inst.itype.imm) + p->R[inst.itype.rs], SIZE_BYTE);
      p->pc += 4;
      break;

    case 0x25: // opcode == 0xd (ORI)
      p->R[inst.itype.rt] = load_mem(siggnext(inst.itype.imm) + p->R[inst.itype.rs], SIZE_HALF_WORD);
      p->pc += 4;
      break;

    case 0x28: // opcode == 0xd (ORI)
      store_mem(p->R[inst.itype.rs] + siggnext(inst.itype.imm), SIZE_BYTE, p->R[inst.itype.rt]);
      p->pc += 4;
      break;

    case 0x29: // opcode == 0xd (ORI)
      store_mem(p->R[inst.itype.rs] + siggnext(inst.itype.imm), SIZE_HALF_WORD, p->R[inst.itype.rt]);
      p->pc += 4;
      break;

    case 0x2b: // opcode == 0xd (ORI)
      store_mem(p->R[inst.itype.rs] + siggnext(inst.itype.imm), SIZE_WORD, p->R[inst.itype.rt]);
      p->pc += 4;
      break;


    case 0x2: // opcode == 0x2 (J)
      p->pc = ((p->pc+4) & 0xf0000000) | (inst.jtype.addr << 2);
      break;

    case 0x3: // opcode == 0x2 (J)
      p->R[31] = p->pc + 4;
      p->pc = ((p->pc+4) & 0xf0000000) | (inst.jtype.addr << 2);
      break;

    default: // undefined opcode
      fprintf(stderr, "%s: pc=%08x, illegal instruction: %08x\n", __FUNCTION__, p->pc, inst.bits);
      exit(-1);
      break;
    }

  // enforce $0 being hard-wired to 0
  p->R[0] = 0;

  if(print_regs)
    print_registers(p);
}

void init_processor(processor_t* p)
{
  int i;

  /* initialize pc to 0x1000 */
  p->pc = 0x1000;

  /* zero out all registers */
  for (i=0; i<32; i++)
  {
    p->R[i] = 0;
  }
}

void print_registers(processor_t* p)
{
  int i,j;
  for (i=0; i<8; i++)
  {
    for (j=0; j<4; j++)
      printf("r%2d=%08x ",i*4+j,p->R[i*4+j]);
    puts("");
  }
}

void handle_syscall(processor_t* p)
{
  reg_t i;

  switch (p->R[2]) // sysall number is given by $v0 ($2)
  {
  case 1: // print an integer
    printf("%d", p->R[4]);
    break;

  case 4: // print a string
    for(i = p->R[4]; i < MEM_SIZE && load_mem(i, SIZE_BYTE); i++)
      printf("%c", load_mem(i, SIZE_BYTE));
    break;

  case 10: // exit
    printf("exiting the simulator\n");
    exit(0);
    break;

  case 11: // print a character
    printf("%c", p->R[4]);
    break;

  default: // undefined syscall
    fprintf(stderr, "%s: illegal syscall number %d\n", __FUNCTION__, p->R[2]);
    exit(-1);
    break;
  }
}
