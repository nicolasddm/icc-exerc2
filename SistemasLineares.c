#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"
#include "SistemasLineares.h"

/*!
  \brief Essa função calcula a norma L2 do resíduo de um sistema linear 

  \param SL Ponteiro para o sistema linear
  \param x Solução do sistema linear
  \param res Valor do resíduo

  \return Norma L2 do resíduo.
*/
real_t normaL2Residuo(SistLinear_t *SL, real_t *x, real_t *res) {  
  for (int i = 0; i < SL->n; ++i) {
    real_t lineValue = 0.0;
    for (int j = 0; j < SL->n; ++j)
      lineValue += SL->A[i][j] * x[j];
    res[i] = SL->b[i] - lineValue;
  }

  real_t norma = 0.0;
  for (int k = 0; k < SL->n; ++k) {
    norma += pow(res[k], 2);
  }
  norma = sqrt(norma);
  printf("\n  --> Norma L2 do resíduo: %.3g \n", norma);
  return norma;
}


/*!
  \brief Método da Eliminação de Gauss

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução
  \param tTotal tempo gasto pelo método

  \return código de erro. 0 em caso de sucesso.
*/
int eliminacaoGauss (SistLinear_t *SL, real_t *x, double *tTotal) {
  for (int i = 0; i < SL->n; ++i) {
    unsigned int iPivo = encontraMax(SL, i);
    if (i != iPivo) {
      SL = trocaLinha(SL, i, iPivo);
    }
    for (int k = i + 1; k < SL->n; ++k) {
      real_t mult = SL->A[k][i] / SL->A[i][i];
      SL->A[k][i] = 0.0;
      for (int j = i + 1; j < SL->n; ++j) {
        SL->A[k][j] -= SL->A[i][j] * mult;
      }
      x[k] -= x[i] * mult;
    }
  }

  return 0;
}

/*!
  \brief Método de Jacobi

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução. Ao iniciar função contém
            valor inicial
  \param tTotal tempo gasto pelo método

  \return código de erro. Um nr positivo indica sucesso e o nr
          de iterações realizadas. Um nr. negativo indica um erro:
          -1 (não converge) -2 (sem solução)
*/
int gaussJacobi (SistLinear_t *SL, real_t *x, double *tTotal) {
  real_t *xaux = (real_t *) malloc(sizeof(real_t) * SL->n);
  int iterations = 0;
  real_t norma;
  int notStop = 1;
  do {
    for (int i = 0; i < SL->n; ++i) {
      real_t lineValue = SL->b[i];
      for(int j = 0; j < SL->n; ++j) {
        if(i != j){
          lineValue -= SL->A[i][j] * x[j];
        }
      }
      xaux[i] = lineValue / SL->A[i][i];
    }
    ++iterations;
    notStop = shouldStop(SL, x, xaux);
    
    for (int i = 0; i < SL->n; ++i) {
      x[i] = xaux[i];
    }
  } while (notStop);

  return iterations;
}

/*!
  \brief Método de Gauss-Seidel

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução. Ao iniciar função contém
            valor inicial
  \param tTotal tempo gasto pelo método

  \return código de erro. Um nr positivo indica sucesso e o nr
          de iterações realizadas. Um nr. negativo indica um erro:
          -1 (não converge) -2 (sem solução)
  */
int gaussSeidel (SistLinear_t *SL, real_t *x, double *tTotal) {
  real_t *xaux = (real_t *) malloc(sizeof(real_t) * SL->n);
  for (int i = 0; i < SL->n; ++i) {
    xaux[i] = x[i];
  }
  
  int iterations = 0;
  int notStop = 1;
  do {
    for (int i = 0; i < SL->n; ++i) {
      real_t lineValue = SL->b[i];
      for(int j = 0; j < SL->n; ++j) {
        if(i != j){
          lineValue -= SL->A[i][j] * x[j];
        }
      }
      x[i] = lineValue / SL->A[i][i];
    }
    ++iterations;
    
    notStop = shouldStop(SL, xaux, x);
    for (int i = 0; i < SL->n; ++i) {
      xaux[i] = x[i];
    }
    
  } while (notStop);

  return iterations;
}


/*!
  \brief Método de Refinamento

  \param SL Ponteiro para o sistema linear
  \param x ponteiro para o vetor solução. Ao iniciar função contém
            valor inicial para início do refinamento
  \param tTotal tempo gasto pelo método

  \return código de erro. Um nr positivo indica sucesso e o nr
          de iterações realizadas. Um nr. negativo indica um erro:
          -1 (não converge) -2 (sem solução)
  */
int refinamento (SistLinear_t *SL, real_t *x, double *tTotal) {
  real_t norma = normaL2Residuo(SL, x, SL->b);
  int iterations = 0;
  real_t *res = (real_t *) malloc(sizeof(real_t) * SL->n);
  while (norma > SL->erro) {
    real_t *w = (real_t *) malloc(sizeof(real_t) * SL->n);
    int gauss = eliminacaoGauss(SL, SL->b, tTotal);
    *tTotal = timestamp() - *tTotal;
    if (gauss != 0)
      return gauss;
  
    ++iterations;
    printf("\n====> Refinamento %f ms --> %d iteração\n", *tTotal, iterations);
    w = retroSubs(SL, w);
    norma = normaL2Residuo(SL, w, res);
  } 
  return iterations;
}

/*!
  \brief Alocaçao de memória 

  \param n tamanho do SL

  \return ponteiro para SL. NULL se houve erro de alocação
  */
SistLinear_t* alocaSistLinear (unsigned int n) {
  SistLinear_t *sist = (SistLinear_t *) malloc(sizeof(SistLinear_t));

  sist->A = (real_t **) malloc(sizeof(real_t *) * n);
  
  for (int i = 0; i < n; ++i)
    sist->A[i] = (real_t *) malloc(sizeof(real_t) * n);

  sist->b = (real_t *) malloc(sizeof(real_t) * n);
  sist->n = n;

  if (!sist)
    return NULL;

  return sist;
}

/*!
  \brief Liberaçao de memória 

  \param sistema linear SL
  */
void liberaSistLinear (SistLinear_t *SL) {
  free(SL->A);
  free(SL->b);
  free(&(SL->n));
  free(&(SL->erro));
  free(SL);
}

/*!
  \brief Leitura de SL a partir de Entrada padrão (stdin).

  \return sistema linear SL. NULL se houve erro (leitura ou alocação)
  */
SistLinear_t *lerSistLinear () {
  unsigned int n;
  scanf("%d", &n);
  
  SistLinear_t *sist = alocaSistLinear(n);
  if (sist == NULL)
    return NULL;

  scanf("%f", &(sist->erro));

  for (int i = 0; i < sist->n; ++i)
    for (int j = 0; j < sist->n; ++j)
      scanf("%f", &(sist->A[i][j]));

  for (int i = 0; i < sist->n; ++i)
    scanf("%f", &(sist->b[i]));

  if (!sist)
    return NULL;

  return sist;
}

// Exibe SL na saída padrão
void prnSistLinear (SistLinear_t *SL) {
  for (int i = 0; i < SL->n; ++i){
    for (int j = 0; j < SL->n; ++j)
      printf("%f ", SL->A[i][j]);
    printf("\n");
  }
}

// Exibe um vetor na saída padrão
void prnVetor (real_t *v, unsigned int n) {
  for (int i = 0; i < n; ++i)
    printf("%f ", v[i]);
}

