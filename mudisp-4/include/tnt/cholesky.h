// Template Numerical Toolkit (TNT) for Linear Algebra
//
// BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
// Please see http://math.nist.gov/tnt for updates
//
// R. Pozo
// Mathematical and Computational Sciences Division
// National Institute of Standards and Technology



#ifndef CHOLESKY_H
#define CHOLESKY_H

#include <cmath>

// index method

namespace TNT
{

#if 1

// scalar point method
//
//
// Only upper part of A is used.  Cholesky factor is returned in
// lower part of L.
//


/*! @fn template <class SPDMatrix, class SymmMatrix> int Cholesky_upper_factorization(SPDMatrix &A, SymmMatrix &L)
 * @headerfile cholesky.h <tnt/cholesky.h>
 * @brief Cholesky factor, Decomposizione Superiore
 * \tparam A input matrix
 * \tparam L input Symmetrical Matrix
 *  @note Only upper part of A is used.  Cholesky factor is returned in
 * lower part of L. \n In algebra lineare la decomposizione di Cholesky 
 * è la fattorizzazione di una matrice hermitiana e definita positiva in 
 * una matrice triangolare inferiore e nella sua trasposta coniugata. 
 * Essa si può considerare come un caso speciale della più generale decomposizione LU. 
 * Il nome di questa decomposizione ricorda il matematico francese André-Louis Cholesky (1875-1918). 
 * Sia \f$\mathbf{A}\f$ una matrice quadrata, hermitiana e definita positiva su campo \f$\mathbb{K}\f$; tale \f$\mathbf{A}\f$ può essere decomposta come
   \f[
    \mathbf{A} = \mathbf{L} \mathbf{L}^+ \qquad (\mathbf{A} \in \mathbb{K}^{m \times m})
  \f]
	con \f$ L \f$ matrice triangolare inferiore con elementi diagonali positivi e \f$ L^+\f$ la matrice coniugata trasposta di \f$L\f$.\n
	Se la matrice \f$ A \f$ è reale e simmetrica, la coniugata trasposta di \f$L\f$ coincide con la trasposta e la decomposizione si semplifica
\f[
    \mathbf{A} = \mathbf{L} \mathbf{L}^{T} \qquad (\mathbf{A} \in \mathbb{R}^{n \times n})
\f]
\b Algoritmo di Cholesky
L'algoritmo di Cholesky , usato per calcolare la matrice di decomposizione \f$L\f$, è una versione modificata dell'algoritmo di Gauss.

L'algoritmo ricorsivo inizia con il considerare

   \f{eqnarray*}{
	    \mathbf{A}^{(1)} &:=& \mathbf{A} \\
		\mathbf{A}^{(i)} &:=& \begin{pmatrix} a_{i,i} & \mathbf{b}_{i}^{*} \\ \mathbf{b}_{i} & \mathbf{B}^{(i)} \end{pmatrix} \\
		\mathbf{L}_{i} &:=& \begin{pmatrix} \frac{1}{\sqrt{a_{i,i}}} & 0 \\ - \frac{1}{a_{i,i}} \mathbf{b}_{i} & \mathbf{I} \end{pmatrix}\\
    \mathbf{A}^{(i)} &:=& \mathbf{L}_{i}^{-1} \begin{pmatrix} 1 & 0 \\ 0 & \mathbf{B}^{(i)} - \frac{1}{a_{i,i}} \mathbf{b}_{i} \mathbf{b}_{i}^{*} \end{pmatrix} (\mathbf{L}_{i}^{-1})^{*}
	\f}
Si definisce per \f$i\f$ successivi \f$i\f$:

   \f[ \mathbf{A}^{(i + 1)} := \mathbf{B}^{(i)} - \frac{1}{a_{i,i}} \mathbf{b}_{i} \mathbf{b}_{i}^{*}\f]

in modo che

    \f[ \mathbf{A}^{(i)} = \mathbf{L}_{i}^{-1} \begin{pmatrix} 1 & 0 \\ 0 & \mathbf{A}^{(i+1)} \end{pmatrix} (\mathbf{L}_{i}^{-1})^{*}\f]

La ricorsione termina dopo \f$n\f$ passi dove \f$A(n) = 1\f$. Si vede che la matrice triangolare inferiore \f$L\f$ è calcolata come

   \f[ \mathbf{L} := \mathbf{L}_{1} \mathbf{L}_{2} \dots \mathbf{L}_{n}\f]

Algoritmo di Cholesky Banachiewicz 

L'algoritmo di Cholesky Banachiewicz dà una formula per calcolare direttamente le entrate della matrice triangolare inferiore \f$L\f$. Esso inizia formando l'angolo superiore sinistro della matrice \f$L\f$ e procede a calcolare la matrice riga per riga.
 \f{eqnarray*}{
\forall i &=& 1,\dots,m\\
    \forall j &=& 1,\dots,(i - 1)\\
      l_{i,j} &=& \frac{1}{l_{j,j}} \left(a_{i,j} - \sum_{\iota = 1}^{j-1} l_{i,\iota} l_{j,\iota}\right)\\
     l_{i,i} &=& \sqrt{ a_{i,i} - \sum_{k=1}^{i-1} l_{i,k}^2 }. 
\f}
\b Algoritmo di Cholesky-Crout
L' algoritmo di Cholesky-Crout fornisce un procedimento un po' differente per calcolare le entrate della matrice triangolare inferiore \f$L\f$. Inizia formando l'angolo superiore sinistro della matrice \f$L\f$ e procede a calcolare la matrice colonna per colonna.
 \f{eqnarray*}{
 \forall i &=& 1,...,m \\
    l_{i,i} &=& \sqrt{ a_{i,i} - \sum_{k=1}^{i-1} l_{i,k}^2 }. \\
   \forall j &=& (i+1),...,m\\
     l_{j,i} &=& \frac{1}{l_{i,i}} \left(a_{j,i} - \sum_{\iota = 1}^{i-1} l_{j,\iota} l_{i,\iota}\right)
\f}
 * @return 0 if successful, 1 otherwise
 * @ingroup matrix
 */

template <class SPDMatrix, class SymmMatrix>
int Cholesky_upper_factorization(SPDMatrix &A, SymmMatrix &L)
{
    Subscript M = A.dim(1);
    Subscript N = A.dim(2);

    assert(M == N);                 // make sure A is square

    // readjust size of L, if necessary

    if (M != L.dim(1) || N != L.dim(2))
        L = SymmMatrix(N,N);

    Subscript i,j,k;


    typename SPDMatrix::element_type dot=0;


    for (j=1; j<=N; j++)                // form column j of L
    {
        dot= 0;

        for (i=1; i<j; i++)             // for k= 1 TO j-1
            dot = dot +  L(j,i)*L(j,i);

        L(j,j) = A(j,j) - dot;

        for (i=j+1; i<=N; i++)
        {
            dot = 0;
            for (k=1; k<j; k++)
                dot = dot +  L(i,k)*L(j,k);
            L(i,j) = A(j,i) - dot;
        }

        if (L(j,j) <= 0.0) return 1;

        L(j,j) = sqrt( L(j,j) );

        for (i=j+1; i<=N; i++)
            L(i,j) = L(i,j) / L(j,j);

    }

    return 0;
}

#else       /* use vector/matrix index regions */

/*! \fn template <class Matrix, class Vector> void Cholesky_lower_factorization(Matrix &A, Vector &b)
 * @headerfile cholesky.h <tnt/cholesky.h>
 * @brief Cholesky factor, Decomposizione Inferiore
 * \param A input matrix
 * \param b input vector
 *
 * \tparam SPDMatrix matrix
 * \tparam SymmMatrix matrix
 * @note Only lower part of A is used.  Cholesky factor is returned in
  lower part of L. \n In algebra lineare la decomposizione di Cholesky è 
  * la fattorizzazione di una matrice hermitiana e definita positiva in 
  * una matrice triangolare inferiore e nella sua trasposta coniugata. 
  * Essa si può considerare come un caso speciale della più generale 
  * decomposizione LU.
 * @ingroup matrix
 */
template <class Matrix, class Vector>
void Cholesky_lower_factorization(Matrix &A, Vector &b)
{
    Subscript m = A.dim(1);
    Subscript n = A.dim(2);
    Subscript i,j;

    Subscript N = (m < n ? n : m);          // K = min(M,N);
    assert( N <= b.dim() );


    for (j=1; j<=N; j++)
    {
        Index J(1, j-1);

        L(j,j) = A(j,j) - dot_product(L(j,J), L(j,J));
        for (i=j+1; j<=N; j++)
            L(i,j) = A(j,i) - dot_product( L(i,J), L(j,J));
        L(j,j) = sqrt( L(j,j) );
        for (i=j+1; j<=N; j++)
            L(i,j) = L(i,j) / L(j,j);

    }

}


#endif
// TNT_USE_REGIONS

}  // namespace TNT

#endif
// CHOLESKY_H
