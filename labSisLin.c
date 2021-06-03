#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "utils.h"
#include "SistemasLineares.h"

int main () {

    SistLinear_t *sist = lerSistLinear();
    if (sist == NULL)
        perror("Não foi possível ler o Sistema Linear");

    double tempo;
    tempo = timestamp();
    
    real_t *variaveis;
    for (int i = 0; i < sist->n; ++i) {
      variaveis[i] = 0.0;
    }
    int gaussJac = gaussJacobi(sist, variaveis, &tempo);
    tempo = timestamp() - tempo;
    printf("====> Eliminação Gauss-Jacobi %lf ms --> %d iterações\n", tempo, gaussJac);
    printf("  --> X: ");
    prnVetor(variaveis, sist->n);
    int refinJacob = refinamento(sist, variaveis, &tempo);
    
    for (int i = 0; i < sist->n; ++i) {
      variaveis[i] = 0.0;
    }
    int gaussSei = gaussSeidel(sist, variaveis, &tempo);
    tempo = timestamp() - tempo;
    printf("\n====> Eliminação Gauss-Seidel %lf ms --> %d iterações\n", tempo, gaussSei);
    printf("  --> X: ");
    prnVetor(variaveis, sist->n);
    int refinGaussSei = refinamento(sist, variaveis, &tempo);
    

    int gauss = eliminacaoGauss(sist, sist->b, &tempo);
    if (gauss != 0) {
      perror("Não foi possível fazer Eliminação de Gauss");
    }
    printf("\n====> Eliminação Gauss %lf ms\n", tempo);
    
    variaveis = retroSubs(sist, variaveis);
    int refin = refinamento(sist, variaveis, &tempo);
}

