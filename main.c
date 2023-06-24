//Alexandre Moreira de Moraes Junior - 2110520
//Thierno Alexander Dia - 2110844
#include "gera.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  funcp gera;
  FILE *arq = fopen("simples.txt", "r");
  int result;
  unsigned char codigo[100];

  if (arq == NULL) {
    perror("Erro ao abrir arquivo");
    exit(1);
  }

  gera = geraSimples(arq, codigo);

  for (int i = 0; i < 100; i++) {
    printf("0x%02X,", codigo[i]);
  }

  fclose(arq);

  return 0;
}
