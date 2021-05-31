#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"
#include "SistemasLineares.h"

real_t* retroSubs (SistLinear_t *SL, real_t *variaveis) {
    for (int i = SL->n - 1; i >= 0; --i) {
        variaveis[i] = SL->b[i];
        for (int j = i + 1; j < SL->n; ++j) 
            variaveis[i] -= SL->A[i][j] * variaveis[j];
        variaveis[i] /= SL->A[i][i];
    }

    return variaveis;
}


int main () {

    SistLinear_t *sist = lerSistLinear();
    if (sist == NULL)
        perror("Não foi possível ler o Sistema Linear");

    prnSistLinear(sist);
    prnVetor(sist->b, sist->n);

    double tempo;
    tempo = timestamp();
    
    int gauss = eliminacaoGauss(sist, sist->b, &tempo);
    
    tempo = timestamp() - tempo;
    
    if (gauss != 0) {
        perror("Não foi possível realizar a Eliminação de Gauss");
    }
    
    printf("Tempo da Eliminação de Gauss %lf", tempo);
    prnSistLinear(sist);
    prnVetor(sist->b, sist->n);

    real_t *variaveis;
    variaveis = retroSubs(sist, variaveis);
    prnVetor(variaveis, sist->n);

}

