#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include "SistemasLineares.h"

/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

double timestamp(void);

int encontraMax(SistLinear_t *ST, unsigned int i);

SistLinear_t* trocaLinha(SistLinear_t *ST, unsigned int i, int iPivo);

#endif // __UTILS_H__