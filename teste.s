.section .data
  machine_code:
  .byte 0x55,0x48,0x89,0xE5,0x48,0x83,0xEC,0x20,0x89,0x7D,0x04,0x8B,0x45,0xFC,0x83,0xC0,0x01,0x89,0x45,0xFC,0x7E,0x00,0x00,0x00,0x00,0x01,0xB8,0x00,0x00,0x00,0x00,0xC9,0xC3,0xB8,0x01,0x00,0x00,0x00,0xC9,0xC3
  
.globl _start
_start:
jmp machine_code

/*as -o teste.o teste.s*/
/*objdump -D teste.o*/
