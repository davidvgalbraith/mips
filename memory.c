#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
int power(int a, int b);
signed int signext(unsigned int imm);
/* Pointer to simulator memory */
uint8_t *mem;

/* Called by program loader to initialize memory. */
uint8_t *init_mem() {
  assert (mem == NULL);
  mem = calloc(MEM_SIZE, sizeof(uint8_t)); // allocate zeroed memory
  return mem;
}

/* Returns 1 if memory access is ok, otherwise 0 */
int access_ok(uint32_t mipsaddr, mem_unit_t size) {

  if (mipsaddr >= MEM_SIZE || mipsaddr % size != 0 || mipsaddr < 1) {
    return 0;
    }
  return 1;
}

/* Writes size bytes of value into mips memory at mipsaddr */
void store_mem(uint32_t mipsaddr, mem_unit_t size, uint32_t value) {
  if (!access_ok(mipsaddr, size)) {
    fprintf(stderr, "%s: bad write=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  }
  int z;
  for (z = 0; z < size; z += 1) {
    mem[mipsaddr + z] = 0;
  }
  int a;
  int b;
  int c = 0;
  for (a = 0; a < size; a += 1) {
    for (b = 0; b < 8; b += 1) {
      mem[mipsaddr + a] |= ((value & power(2, b + c)) >> c);
    }
    c += 8;
  }

}

/* Returns zero-extended value from mips memory */
uint32_t load_mem(uint32_t mipsaddr, mem_unit_t size) {
  if (!access_ok(mipsaddr, size)) {
    fprintf(stderr, "%s: bad read=%08x\n", __FUNCTION__, mipsaddr);
    exit(-1);
  }
  if (size == SIZE_BYTE) {
    uint32_t x = 0;
    uint8_t y = *(uint8_t*) (mem + mipsaddr);
    return (uint32_t) (y | x);
  }
  if (size == SIZE_HALF_WORD) {
    uint32_t xx = 0;
    uint16_t yy = *(uint16_t*) (mem + mipsaddr);
    return (uint32_t) yy | xx;
  }
  // incomplete stub to let mipscode/simple execute
  // (only handles size == SIZE_WORD correctly)
  // feel free to delete and implement your own way
  return *(uint32_t*)(mem + mipsaddr);
}
