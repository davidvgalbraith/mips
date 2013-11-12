#include "io.h"

int garbage(int a, int b) {
  int l = 1;
  while (a > b) {
    b *= l;
    l += 1;
  }
  return l;
}

int main() {
  int a = 2323523; 
  int b = 423;
  mips_print_string("A(");
  mips_print_int(a);
  mips_print_string(", ");
  mips_print_int(b);
  mips_print_string(") = ");
  mips_print_int(garbage(a,b));
  mips_print_string("\n");

  return 0;
}
