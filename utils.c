#include "utils.h"
#include "SistemasLineares.h"
#include <math.h>

/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

double timestamp(void) {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}

int encontraMax(SistLinear_t *SL, unsigned int i) {
  int maxValue = fabs(SL->A[0][i]);
  int max = 0;

  for (int j = 1; j < SL->n; ++j) {
    if (fabs(SL->A[j][i]) > maxValue) {
      maxValue = fabs(SL->A[j][i]);
      max = j;
    }
  }

  return max;
}

SistLinear_t* trocaLinha(SistLinear_t *SL, unsigned int i, int iPivo) {
  for (int j = 0; j < SL->n; ++j) {
    real_t auxSL = SL->A[j][i];
    SL->A[j][i] = SL->A[j][iPivo];
    SL->A[j][iPivo] = auxSL;
  }

  real_t auxB = SL->b[i];
  SL->b[i] = SL->b[iPivo];
  SL->b[iPivo] = auxB;

  return SL;
}
