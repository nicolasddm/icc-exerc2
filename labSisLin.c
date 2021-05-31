#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "utils.h"
#include "SistemasLineares.h"

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

}

