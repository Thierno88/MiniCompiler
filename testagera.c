//Alexandre Moreira de Moraes Junior - 2110520
//Thierno Alexander Dia - 2110844
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gera.h"

int tests_run = 0;

typedef int (*funcp)();

/* Função gera */
//funcp geraSimples(FILE arq, unsigned char codigo[]);

/* Testando ret $100*/
static char * test_ret_100() {
    unsigned char codigo[100];
    FILE *arq = fopen("teste1.txt","r");
    if (arq == NULL) {
    printf("Erro ao abrir do arquivo\n");
    exit(1);
    }
    
    funcp funcao = geraSimples(arq, codigo);
    fclose(arq);
    //for (int i = 0; i < 30; i++) {
    //printf("0x%02X,", codigo[i]);
  //}
    unsigned char expected[] = {
        0x55,
        0x48,0x89,0xe5,
        0x48,0x83,0xec,0x20,
       	0xb8,0x64,0x00,0x00,0x00,
        0xc9,
        0xc3
    };

    mu_assert("Erro no teste v1 < $10", memcmp(codigo, expected, sizeof(expected)) == 0);
    // Limpar o array codigo
    //memset(codigo, 0, sizeof(codigo));
    return 0;
}


//Teste caso v1 < $5 ret v1
static char * test_v1_5_retv1() {
    unsigned char codigo1[100];
    FILE *arq = fopen("teste2.txt","r");
    if (arq == NULL) {
    printf("Erro ao abrir do arquivo\n");
    exit(1);
    }
    
    funcp funcao = geraSimples(arq, codigo1);
    fclose(arq);

    unsigned char expected[] = {
        0x55, 
        0x48, 0x89, 0xe5, 
        0x48, 0x83, 0xec, 0x20,
        0xc7, 0x45, 0xfc, 0x05, 0x00, 0x00, 0x00,0x8b,0x45,0xfc, 
        0xc9, 
        0xc3
    };
    mu_assert("Erro no teste ret $5", memcmp(codigo1, expected, sizeof(expected)) == 0);
    // Limpar o array codigo
    //memset(codigo1, 0, sizeof(codigo1));
    return 0;
}


//Teste caso v1 = v2 + v3 
//Usando %eax como variavel intermediaria
static char * test_v1_v2_soma_v3() {
    unsigned char codigo[100];
    FILE *arq= fopen("teste3.txt","r");
    if (arq == NULL) {
    printf("Erro ao abrir do arquivo\n");
    exit(1);
    }
    
    funcp funcao = geraSimples(arq, codigo);
    fclose(arq);

    unsigned char expected[] = {
        0x55,0x48,0x89,0xE5,0x48,0x83,0xEC,0x20,0x8B,0x45,0xF8,0x03,0x45,0xF4,0x89,0x45
    };

    mu_assert("Erro no teste v1 = v2 + v3", memcmp(codigo, expected, sizeof(expected)) == 0);
    // Limpar o array codigo
    memset(codigo, 0, sizeof(codigo));
    return 0;
}

/* Teste caso ret v2 = $2
		  v3 = $5
		  v1 = v3 - v2
		  ret v1 
*/
static char * test_v2_menos_v3_v1() {
    unsigned char codigo[100];
    FILE *arq= fopen("teste4.txt","r");
    if (arq == NULL) {
    printf("Erro ao abrir do arquivo\n");
    exit(1);
    }
    
    funcp funcao = geraSimples(arq, codigo);
    fclose(arq);

    unsigned char expected[] = {
        0x55,0x48,0x89,0xE5,0x48,0x83,0xEC,0x20,0xC7,0x45,0xF8,0x02,0x00,0x00,0x00,0xC7,0x45,0xF4,0x05,0x00,0x00,0x00,0x8B,0x45,0xF4,0x2B,0x45,0xF8,0x89,0x45,0xFC,0x8B,0x45,0xFC,0xC9,0xC3
    };

    mu_assert("Erro no teste v2 < $2, v3 < %5, v1 = v3 - v2, ret v1", memcmp(codigo, expected, sizeof(expected)) == 0);
    // Limpar o array codigo
    memset(codigo, 0, sizeof(codigo));
    return 0;
}

//Testando v1 < p2, ret v1
static char * test_v1_p2() {
    unsigned char codigo[100];
    FILE *arq= fopen("teste5.txt","r");
    if (arq == NULL) {
    printf("Erro ao abrir do arquivo\n");
    exit(1);
    }
    
    funcp funcao = geraSimples(arq, codigo);
    fclose(arq);

    unsigned char expected[] = {
        0x55,0x48,0x89,0xE5,0x48,0x83,0xEC,0x20,0x89,0x75,0x04,0x8B,0x45,0xFC,0xC9,0xC3
    };

    mu_assert("Erro no teste v1 < p2", memcmp(codigo, expected, sizeof(expected)) == 0);
    // Limpar o array codigo
    memset(codigo, 0, sizeof(codigo));
    return 0;
}

//Testando operacoes entre $ e v1
static char * test_v1_soma_$_v2() {
    unsigned char codigo[100];
    FILE *arq= fopen("teste6.txt","r");
    if (arq == NULL) {
    printf("Erro ao abrir do arquivo\n");
    exit(1);
    }
    
    funcp funcao = geraSimples(arq, codigo);
    fclose(arq);

    unsigned char expected[] = {
        0x55,0x48,0x89,0xE5,0x48,0x83,0xEC,0x20,0xC7,0x45,0xF8,0x0A,0x00,0x00,0x00,0xB8,0x05,0x00,0x00,0x00,0x03,0x45,0xF8,0x89,0x45,0xFC,0x8B,0x45,0xFC,0xC9,0xC3
    };

    mu_assert("Erro no teste v1 < p2", memcmp(codigo, expected, sizeof(expected)) == 0);
    // Limpar o array codigo
    memset(codigo, 0, sizeof(codigo));
    return 0;
}

//Testando iflez
static char * iflez() {
    unsigned char codigo[100];
    FILE *arq= fopen("teste7.txt","r");
    if (arq == NULL) {
    printf("Erro ao abrir do arquivo\n");
    exit(1);
    }
    
    funcp funcao = geraSimples(arq, codigo);
    fclose(arq);

    unsigned char expected[] = {
        0x55,0x48,0x89,0xE5,0x48,0x83,0xEC,0x20,0x89,0x7D,0x04,0x8B,0x45,0xFC,0x83,0xC0,0x01,0x89,0x45,0xFC,0x7E,0x00,0x00,0x00,0x00,0x00,0xB8,0x00,0x00,0x00,0x00,0xC9,0xC3
    };

    mu_assert("Erro no teste iflez", memcmp(codigo, expected, sizeof(expected)) == 0);
    // Limpar o array codigo
    memset(codigo, 0, sizeof(codigo));
    return 0;
}

static char * all_tests() {
    mu_run_test(test_ret_100);
    mu_run_test(test_v1_5_retv1);
    mu_run_test(test_v1_v2_soma_v3);
    mu_run_test(test_v2_menos_v3_v1);
    mu_run_test(test_v1_p2);
    mu_run_test(test_v1_soma_$_v2);
    mu_run_test(iflez);
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("TODOS TESTES APROVADOS\n");
    }
    printf("QTD TESTES REALIZADOS: %d\n", tests_run);

    return result != 0;
}
