//Alexandre Moreira de Moraes Junior - 2110520
//Thierno Alexander Dia - 2110844
#include <stdio.h>
typedef int (*funcp)();
funcp geraSimples(FILE*arq,unsigned char codigo[]);
/* file: minunit.h */
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
 extern int tests_run;
