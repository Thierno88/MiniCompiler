.text
.globl main

main:
	movl $5, -4(%rbp)
	movl -4(%rbp), %eax
	
/*gcc -c finder.s*/
/*objdump -d finder.o*/
