//Alexandre Moreira de Moraes Junior - 2110520
//Thierno Alexander Dia - 2110844
#include "gera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXVAR 5
#define MAXPARAM 3

typedef struct Jump {
  int posCodigo; // onde será alterado o código de máquina
  int destino;   // linha de destino do jump
  int prox;      // linha da próxima instrução após o jump
} jump;

unsigned char *codigo;
int i;

static void error(const char *msg, int line) {
  fprintf(stderr, "erro %s na linha %d\n", msg, line);
  exit(EXIT_FAILURE);
}

funcp geraSimples(FILE *arq, unsigned char codigo[]) {
  int line = 1;
  int c;
  funcp cod;
  int pos;
  int nJumps = 0;
  long int destino;
  int prox;
  int endereco[30];
  jump jumps[30];
  int i = 0;

  codigo[i++] = 0x55; // push %ebp
  codigo[i++] = 0x48; // mov %rsp, %rbp
  codigo[i++] = 0x89;
  codigo[i++] = 0xe5;
  codigo[i++] = 0x48; // sub $0x20, %rsp
  codigo[i++] = 0x83;
  codigo[i++] = 0xec;
  codigo[i++] = 0x20;

  while ((c = fgetc(arq)) != EOF) {
    switch (c) {
    case 'r': { // retorno
      char var0;
      int idx0;

      if (fscanf(arq, "et %c%d", &var0, &idx0) != 2)
        error("comando invalido", line);

      switch (var0) {
        int masc = 0xFF;

      case '$': {           // constante
        codigo[i++] = 0xb8; // mov $idx0, %eax
        *((int *)&codigo[i]) = idx0;
        i += 4;
        break;
      } // fecha case '$'

      case 'v': {
        codigo[i++] = 0x8b; // mov idx0(%rbp), %eax
        codigo[i++] = 0x45;
        codigo[i++] = 0xff - 4 * idx0 + 1;
        break;
      } // fecha case 'v'

      case 'p': {
        codigo[i++] = 0x89;
        if (idx0 == 1)
          codigo[i++] = 0xf8; // mov %edi, %eax
        if (idx0 == 2)
          codigo[i++] = 0xf0; // mov %esi, %eax
        if (idx0 == 3)
          codigo[i++] = 0xd0; // mov %edx, %eax
        break;
      }                   // fecha case 'p'
      }                   // fecha o  switch (var0)
      codigo[i++] = 0xc9; // leave
      codigo[i++] = 0xc3; // ret
      printf("%d ret %c%d\n", line, var0, idx0);
      break;
    } // fecha case 'r'

    case 'v': {
      char var0 = c, c0, var1;
      int idx0, idx1;

      if (fscanf(arq, "%d %c", &idx0, &c0) != 2)
        error("comando invalido", line);

      switch (c0) {
      case '<': {
        if (fscanf(arq, " %c%d", &var1, &idx1) != 2)
          error("comando invalido", line);

        switch (var1) {
        case '$': {           // atribuição de constante
          codigo[i++] = 0xc7; // mov $idx1, idx0(%rbp)
          codigo[i++] = 0x45;
          codigo[i++] = 0xff - 4 * idx0 + 1;
          *((int *)&codigo[i]) = idx1;
          i += 4;
          break;
        }
        case 'v': {
          codigo[i++] = 0x44; // mov idx1(%rbp), %eax
          codigo[i++] = 0x8b;
          codigo[i++] = 0x55;
          codigo[i++] = 0xff - 4 * idx1 + 1;
          codigo[i++] = 0x44; // mov %eax, idx0(%rbp)
          codigo[i++] = 0x89;
          codigo[i++] = 0x55;
          codigo[i++] = 0xff - 4 * idx0 + 1;
          break;
        }
        case 'p': {
          codigo[i++] = 0x89;
          if (idx1 == 1) {
            codigo[i++] = 0x7d; // mov %edi, 4(%rbp)
            codigo[i++] = 0x04;
          }
          if (idx1 == 2) {
            codigo[i++] = 0x75; // mov %esi, 4(%rbp)
            codigo[i++] = 0x04;
          }
          if (idx1 == 3) {
            codigo[i++] = 0x55; // mov %edx, 4(%rbp)
            codigo[i++] = 0x04;
          }
          break;
        }
        }
        printf("%d %c%d < %c%d\n", line, var0, idx0, var1, idx1);
        break;
      } // fecha case '<'

      case '=': { // atribuicao
        char var2, op;
        int idx2;
        if (fscanf(arq, " %c%d %c %c%d", &var1, &idx1, &op, &var2, &idx2) != 5)
          error("comando invalido", line);

        if (op != '+' && op != '-' && op != '*')
          error("simbolo invalido", line);

        if (var1 == '$' && var2 == '$') {
          codigo[i++] = 0xb8;
          *((int *)&codigo[i]) = idx1;
          i += 4;

          if (op == '+') {
            codigo[i++] = 0x05;
            *((int *)&codigo[i]) = idx2;
            i += 4;
          } // adição $-$

          else if (op == '-') {
            codigo[i++] = 0x83;
            codigo[i++] = 0xe8;
            *((int *)&codigo[i++]) = idx2;
          } // subtração $-$

          else if (op == '*') {
            codigo[i++] = 0x6b;
            codigo[i++] = 0xc0;
            *((int *)&codigo[i]) = idx2;
            i += 1;
          } // multiplicação $-$

          break;
        } // fecha if (var1 == '$' && var2 == '$')
        if (var1 == 'v' && var2 == '$') {
          codigo[i++] = 0x8b;
          codigo[i++] = 0x45;
          //*((int *)&codigo[i]) = idx1;
          // i += 4;
          codigo[i++] = 0xff - 4 * idx1 + 1;

          if (op == '+') {
            codigo[i++] = 0x83;
            codigo[i++] = 0xc0;
          } // adição v-$

          else if (op == '-') {
            codigo[i++] = 0x83;
            codigo[i++] = 0xe8;
          } // substração v-$

          else if (op == '*') {
            codigo[i++] = 0x6b;
            codigo[i++] = 0xc0;
          } // multiplicação v-$

          *((int *)&codigo[i]) = idx2;
          i += 1;
        } // fecha if (var1 == 'v' && var2 == '$')
        else if (var1 == '$' && var2 == 'v') {
          codigo[i++] = 0xb8;
          *((int *)&codigo[i]) = idx1;
          i += 4;

          if (op == '+') {
            codigo[i++] = 0x03;
          } // Adição $-v

          else if (op == '-') {
            codigo[i++] = 0x2b;
          } // Subtração $-v

          else if (op == '*') {
            codigo[i++] = 0x0f;
            codigo[i++] = 0xaf;
          } // Multiplicação $-v

          codigo[i++] = 0x45;
          codigo[i++] = 0xff - 4 * idx2 + 1;

        } // fecha do else if (var1 == '$' && var2 == 'v')
        else if (var1 == 'v' && var2 == 'v') {
          codigo[i++] = 0x8b;
          codigo[i++] = 0x45;

          codigo[i++] = 0xff - 4 * idx1 + 1;

          if (op == '+') {
            codigo[i++] = 0x03;
          } // Adição v-v

          else if (op == '-') {
            codigo[i++] = 0x2b;
          } // Subtração v-v

          else if (op == '*') {
            codigo[i++] = 0x0f;
            codigo[i++] = 0xaf;
          } // multiplicação v-v

          codigo[i++] = 0x45;
          codigo[i++] = 0xff - 4 * idx2 + 1;
          printf("%d %c%d = %c%d %c %c%d\n", line, var0, idx0, var1, idx1, op,
                 var2, idx2);
        } // fecha do else if (var1 == 'v' && var2 == 'v')

        // fim de op aritimetica
        codigo[i++] = 0x89;
        codigo[i++] = 0x45;
        codigo[i++] = 0xff - 4 * idx0 + 1;

      } // fecha case '='

      } // fecha switch(co)
      break;
    } // fecha case 'v'

    case 'i': { // desvio
      char var0;
      int idx0, n;
      if (fscanf(arq, "flez %c%d %d", &var0, &idx0, &n) != 3)
        error("comando invalido", line);

      codigo[i++] = 0x7e;
      jumps[nJumps].posCodigo = i;
      jumps[nJumps].destino = n;
      jumps[nJumps++].prox = line + 1;
      for (int j = 0; j < 4; j++)
        codigo[i++] = 0x00;
      i++;
      printf("%d iflez %c%d %d\n", line, var0, idx0, n);
      break;
    }
    default:
      error("comando desconhecido", line);
    } // fecha switch (c)
    line++;
    fscanf(arq, " ");
  } // fecha WHILE
  return (funcp)codigo;
}
