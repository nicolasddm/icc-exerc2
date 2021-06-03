#include "utils.h"
#include <stdio.h>
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
  real_t maxValue = fabs(SL->A[i][i]);
  int max = i;

  for (int j = i; j < SL->n; ++j) {
    if (fabs(SL->A[j][i]) > maxValue) {
      printf("Trocou Max %f pela %f \n", maxValue, fabs(SL->A[j][i]));
      maxValue = fabs(SL->A[j][i]);
      max = j;
    }
  }

  return max;
}

SistLinear_t* trocaLinha(SistLinear_t *SL, unsigned int i, int iPivo) {
  for (int j = 0; j < SL->n; ++j) {
    real_t auxSL = SL->A[i][j];
    SL->A[i][j] = SL->A[iPivo][j];
    SL->A[iPivo][j] = auxSL;
  }

  real_t auxB = SL->b[i];
  SL->b[i] = SL->b[iPivo];
  SL->b[iPivo] = auxB;

  return SL;
}

real_t* retroSubs (SistLinear_t *SL, real_t *variaveis) {
  printf("  --> X: ");
  for (int i = SL->n - 1; i >= 0; --i) {
      variaveis[i] = SL->b[i];
      for (int j = i + 1; j < SL->n; ++j) 
          variaveis[i] -= SL->A[i][j] * variaveis[j];
      variaveis[i] /= SL->A[i][i];
      printf("%f ", variaveis[i]);
  }
  return variaveis;
}

int shouldStop(SistLinear_t *SL, real_t *x1, real_t *x2) {
  real_t max = fabs(x2[0] - x1[0]);
  for (int i = 1; i < SL->n; ++i)
    if (max < fabs(x2[i] - x1[i]))
      max = fabs(x2[i] - x1[i]);
  
  if(max <= SL->erro)
    return 0;
  return 1;
}