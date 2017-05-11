/*!
 * Very simple wrapper over BLAS calls to make them generic to the data type.
 */

#ifndef blas_hpp
#define blas_hpp

#include <cblas.h>
#include <complex>

namespace blas
{
	template <typename float_type> using complex = std::complex<float_type>;

	using order = CBLAS_ORDER;
	using transpose = CBLAS_TRANSPOSE;
	using triangular = CBLAS_UPLO;
	using diagonal = CBLAS_DIAG;
	using side = CBLAS_SIDE;

	// ========================================================================
	// Overrides for level 1 BLAS functions
	// ========================================================================

	inline float dot(const int N, const float *X, const int incX, const float *Y, const int incY)
	{
		return cblas_sdot(N, X, incX, Y, incY);
	}

	inline double dot(const int N, const double *X, const int incX, const double *Y, const int incY)
	{
		return cblas_ddot(N, X, incX, Y, incY);
	}

	// Functions having prefixes Z and C only

	inline complex<float> dotu(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
	{
		complex<float> u;
		cblas_cdotu_sub(N, X, incX, Y, incY, &u);
		return u;
	}

	inline complex<float> dotc(const int N, const complex<float> *X, const int incX, const complex<float> *Y, const int incY)
	{
		complex<float> c;
		cblas_cdotc_sub(N, X, incX, Y, incY, &c);
		return c;
	}

	// Functions having prefixes S D SC DZ

	inline complex<double> dotu(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
	{
		complex<double> u;
		cblas_zdotu_sub(N, X, incX, Y, incY, &u);
		return u;
	}

	inline complex<double> dotc(const int N, const complex<double> *X, const int incX, const complex<double> *Y, const int incY)
	{
		complex<double> c;
		cblas_zdotc_sub(N, X, incX, Y, incY, &c);
		return c;
	}

	inline float nrm2(const int N, const float *X, const int incX)
	{
		return cblas_snrm2(N, X, incX);
	}

	inline float asum(const int N, const float *X, const int incX)
	{
		return cblas_sasum(N, X, incX);
	}

	inline double nrm2(const int N, const double *X, const int incX)
	{
		return cblas_dnrm2(N, X, incX);
	}

	inline double asum(const int N, const double *X, const int incX)
	{
		return cblas_dasum(N, X, incX);
	}

	inline float nrm2(const int N, const complex<float> *X, const int incX)
	{
		return cblas_scnrm2(N, X, incX);
	}

	inline float asum(const int N, const complex<float> *X, const int incX)
	{
		return cblas_scasum(N, X, incX);
	}

	inline double nrm2(const int N, const complex<double> *X, const int incX)
	{
		return cblas_dznrm2(N, X, incX);
	}

	inline double asum(const int N, const complex<double> *X, const int incX)
	{
		return cblas_dzasum(N, X, incX);
	}

	// Functions having standard 4 prefixes (S D C Z)

	inline std::size_t amax(const int N, const float *X, const int incX)
	{
		return cblas_isamax(N, X, incX);
	}

	inline std::size_t amax(const int N, const double *X, const int incX)
	{
		return cblas_idamax(N, X, incX);
	}

	inline std::size_t amax(const int N, const complex<float> *X, const int incX)
	{
		return cblas_icamax(N, X, incX);
	}

	inline std::size_t amax(const int N, const complex<double> *X, const int incX)
	{
		return cblas_izamax(N, X, incX);
	}

	// ========================================================================
	// Overrides for level 1 BLAS routines
	// ========================================================================

	// Routines with standard 4 prefixes (s, d, c, z)

	inline void swap(const int N, float *X, const int incX, float *Y, const int incY)
	{
		cblas_sswap(N, X, incX, Y, incY);
	}

	inline void copy(const int N, const float *X, const int incX, float *Y, const int incY)
	{
		cblas_scopy(N, X, incX, Y, incY);
	}

	inline void axpy(const int N, const float alpha, const float *X, const int incX, float *Y, const int incY)
	{
		cblas_saxpy(N, alpha, X, incX, Y, incY);
	}

	inline void swap(const int N, double *X, const int incX, double *Y, const int incY)
	{
		cblas_dswap(N, X, incX, Y, incY);
	}

	inline void copy(const int N, const double *X, const int incX, double *Y, const int incY)
	{
		cblas_dcopy(N, X, incX, Y, incY);
	}

	inline void axpy(const int N, const double alpha, const double *X, const int incX, double *Y, const int incY)
	{
		cblas_daxpy(N, alpha, X, incX, Y, incY);
	}

	inline void swap(const int N, complex<float> *X, const int incX, complex<float> *Y, const int incY)
	{
		cblas_cswap(N, X, incX, Y, incY);
	}

	inline void copy(const int N, const complex<float> *X, const int incX, complex<float> *Y, const int incY)
	{
		cblas_ccopy(N, X, incX, Y, incY);
	}

	inline void axpy(const int N, const complex<float> &alpha, const complex<float> *X, const int incX, complex<float> *Y, const int incY)
	{
		cblas_caxpy(N, &alpha, X, incX, Y, incY);
	}

	inline void swap(const int N, complex<double> *X, const int incX, complex<double> *Y, const int incY)
	{
		cblas_zswap(N, X, incX, Y, incY);
	}

	inline void copy(const int N, const complex<double> *X, const int incX, complex<double> *Y, const int incY)
	{
		cblas_zcopy(N, X, incX, Y, incY);
	}

	inline void axpy(const int N, const complex<double> &alpha, const complex<double> *X, const int incX, complex<double> *Y, const int incY)
	{
		cblas_zaxpy(N, &alpha, X, incX, Y, incY);
	}

	// Routines with S and D prefix only

	inline void rotg(float *a, float *b, float *c, float *s)
	{
		cblas_srotg(a, b, c, s);
	}

	inline void rotmg(float *d1, float *d2, float *b1, const float b2, float *P)
	{
		cblas_srotmg(d1, d2, b1, b2, P);
	}

	inline void rot(const int N, float *X, const int incX, float *Y, const int incY, const float c, const float s)
	{
		cblas_srot(N, X, incX, Y, incY, c, s);
	}

	inline void rotm(const int N, float *X, const int incX, float *Y, const int incY, const float *P)
	{
		cblas_srotm(N, X, incX, Y, incY, P);
	}

	inline void rotg(double *a, double *b, double *c, double *s)
	{
		cblas_drotg(a, b, c, s);
	}

	inline void rotmg(double *d1, double *d2, double *b1, const double b2, double *P)
	{
		cblas_drotmg(d1, d2, b1, b2, P);
	}

	inline void rot(const int N, double *X, const int incX, double *Y, const int incY, const double c, const double s)
	{
		cblas_drot(N, X, incX, Y, incY, c, s);
	}

	inline void rotm(const int N, double *X, const int incX, double *Y, const int incY, const double *P)
	{
		cblas_drotm(N, X, incX, Y, incY, P);
	}

	// Routines with S D C Z CS and ZD prefixes

	inline void scal(const int N, const float alpha, float *X, const int incX)
	{
		cblas_sscal(N, alpha, X, incX);
	}

	inline void scal(const int N, const double alpha, double *X, const int incX)
	{
		cblas_dscal(N, alpha, X, incX);
	}

	inline void scal(const int N, const complex<float> &alpha, complex<float> *X, const int incX)
	{
		cblas_cscal(N, &alpha, X, incX);
	}

	inline void scal(const int N, const complex<double> &alpha, complex<double> *X, const int incX)
	{
		cblas_zscal(N, &alpha, X, incX);
	}

	inline void scal(const int N, const float alpha, complex<float> *X, const int incX)
	{
		cblas_csscal(N, alpha, X, incX);
	}

	inline void scal(const int N, const double alpha, complex<double> *X, const int incX)
	{
		cblas_zdscal(N, alpha, X, incX);
	}

	// ========================================================================
	// Overrides for level 2 BLAS
	// ========================================================================

	// Routines with standard 4 prefixes (S, D, C, Z)

	inline void gemv(const enum order order, const enum transpose transA, const int M, const int N, const float alpha, const float *A, const int lda, const float *X, const int incX, const float beta, float *Y, const int incY)
	{
		cblas_sgemv(order, transA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
	}

	inline void gbmv(const enum order order, const enum transpose transA, const int M, const int N, const int KL, const int KU, const float alpha, const float *A, const int lda, const float *X, const int incX, const float beta, float *Y, const int incY)
	{
		cblas_sgbmv(order, transA, M, N, KL, KU, alpha, A, lda, X, incX, beta, Y, incY);
	}

	inline void trmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const float *A, const int lda, float *X, const int incX)
	{
		cblas_strmv(order, uplo, transA, diag, N, A, lda, X, incX);
	}

	inline void tbmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const int K, const float *A, const int lda, float *X, const int incX)
	{
		cblas_stbmv(order, uplo, transA, diag, N, K, A, lda, X, incX);
	}

	inline void tpmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const float *Ap, float *X, const int incX)
	{
		cblas_stpmv(order, uplo, transA, diag, N, Ap, X, incX);
	}

	inline void trsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const float *A, const int lda, float *X, const int incX)
	{
		cblas_strsv(order, uplo, transA, diag, N, A, lda, X, incX);
	}

	inline void tbsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const int K, const float *A, const int lda, float *X, const int incX)
	{
		cblas_stbsv(order, uplo, transA, diag, N, K, A, lda, X, incX);
	}

	inline void tpsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const float *Ap, float *X, const int incX)
	{
		cblas_stpsv(order, uplo, transA, diag, N, Ap, X, incX);
	}

	inline void gemv(const enum order order, const enum transpose transA, const int M, const int N, const double alpha, const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)
	{
		cblas_dgemv(order, transA, M, N, alpha, A, lda, X, incX, beta, Y, incY);
	}

	inline void gbmv(const enum order order, const enum transpose transA, const int M, const int N, const int KL, const int KU, const double alpha, const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)
	{
		cblas_dgbmv(order, transA, M, N, KL, KU, alpha, A, lda, X, incX, beta, Y, incY);
	}

	inline void trmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const double *A, const int lda, double *X, const int incX)
	{
		cblas_dtrmv(order, uplo, transA, diag, N, A, lda, X, incX);
	}

	inline void tbmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const int K, const double *A, const int lda, double *X, const int incX)
	{
		cblas_dtbmv(order, uplo, transA, diag, N, K, A, lda, X, incX);
	}

	inline void tpmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const double *Ap, double *X, const int incX)
	{
		cblas_dtpmv(order, uplo, transA, diag, N, Ap, X, incX);
	}

	inline void trsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const double *A, const int lda, double *X, const int incX)
	{
		cblas_dtrsv(order, uplo, transA, diag, N, A, lda, X, incX);
	}

	inline void tbsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const int K, const double *A, const int lda, double *X, const int incX)
	{
		cblas_dtbsv(order, uplo, transA, diag, N, K, A, lda, X, incX);
	}

	inline void tpsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const double *Ap, double *X, const int incX)
	{
		cblas_dtpsv(order, uplo, transA, diag, N, Ap, X, incX);
	}

	inline void gemv(const enum order order, const enum transpose transA, const int M, const int N, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> &beta, complex<float> *Y, const int incY)
	{
		cblas_cgemv(order, transA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
	}

	inline void gbmv(const enum order order, const enum transpose transA, const int M, const int N, const int KL, const int KU, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> &beta, complex<float> *Y, const int incY)
	{
		cblas_cgbmv(order, transA, M, N, KL, KU, &alpha, A, lda, X, incX, &beta, Y, incY);
	}

	inline void trmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const complex<float> *A, const int lda, complex<float> *X, const int incX)
	{
		cblas_ctrmv(order, uplo, transA, diag, N, A, lda, X, incX);
	}

	inline void tbmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const int K, const complex<float> *A, const int lda, complex<float> *X, const int incX)
	{
		cblas_ctbmv(order, uplo, transA, diag, N, K, A, lda, X, incX);
	}

	inline void tpmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const complex<float> *Ap, complex<float> *X, const int incX)
	{
		cblas_ctpmv(order, uplo, transA, diag, N, Ap, X, incX);
	}

	inline void trsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const complex<float> *A, const int lda, complex<float> *X, const int incX)
	{
		cblas_ctrsv(order, uplo, transA, diag, N, A, lda, X, incX);
	}

	inline void tbsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const int K, const complex<float> *A, const int lda, complex<float> *X, const int incX)
	{
		cblas_ctbsv(order, uplo, transA, diag, N, K, A, lda, X, incX);
	}

	inline void tpsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const complex<float> *Ap, complex<float> *X, const int incX)
	{
		cblas_ctpsv(order, uplo, transA, diag, N, Ap, X, incX);
	}

	inline void gemv(const enum order order, const enum transpose transA, const int M, const int N, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> &beta, complex<double> *Y, const int incY)
	{
		cblas_zgemv(order, transA, M, N, &alpha, A, lda, X, incX, &beta, Y, incY);
	}

	inline void gbmv(const enum order order, const enum transpose transA, const int M, const int N, const int KL, const int KU, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> &beta, complex<double> *Y, const int incY)
	{
		cblas_zgbmv(order, transA, M, N, KL, KU, &alpha, A, lda, X, incX, &beta, Y, incY);
	}

	inline void trmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const complex<double> *A, const int lda, complex<double> *X, const int incX)
	{
		cblas_ztrmv(order, uplo, transA, diag, N, A, lda, X, incX);
	}

	inline void tbmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const int K, const complex<double> *A, const int lda, complex<double> *X, const int incX)
	{
		cblas_ztbmv(order, uplo, transA, diag, N, K, A, lda, X, incX);
	}

	inline void tpmv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const complex<double> *Ap, complex<double> *X, const int incX)
	{
		cblas_ztpmv(order, uplo, transA, diag, N, Ap, X, incX);
	}

	inline void trsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const complex<double> *A, const int lda, complex<double> *X, const int incX)
	{
		cblas_ztrsv(order, uplo, transA, diag, N, A, lda, X, incX);
	}

	inline void tbsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const int K, const complex<double> *A, const int lda, complex<double> *X, const int incX)
	{
		cblas_ztbsv(order, uplo, transA, diag, N, K, A, lda, X, incX);
	}

	inline void tpsv(const enum order order, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int N, const complex<double> *Ap, complex<double> *X, const int incX)
	{
		cblas_ztpsv(order, uplo, transA, diag, N, Ap, X, incX);
	}

	// Routines with S and D prefixes only

	inline void symv(const enum order order, const enum triangular uplo, const int N, const float alpha, const float *A, const int lda, const float *X, const int incX, const float beta, float *Y, const int incY)
	{
		cblas_ssymv(order, uplo, N, alpha, A, lda, X, incX, beta, Y, incY);
	}

	inline void sbmv(const enum order order, const enum triangular uplo, const int N, const int K, const float alpha, const float *A, const int lda, const float *X, const int incX, const float beta, float *Y, const int incY)
	{
		cblas_ssbmv(order, uplo, N, K, alpha, A, lda, X, incX, beta, Y, incY);
	}

	inline void spmv(const enum order order, const enum triangular uplo, const int N, const int K, const float alpha, const float *A, const int lda, const float *Ap, const float *X, const int incX, const float beta, float *Y, const int incY)
	{
		cblas_sspmv(order, uplo, N, alpha, Ap, X, incX, beta, Y, incY);
	}

	inline void ger(const enum order order, const int M, const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY, float *A, const int lda)
	{
		cblas_sger(order, M, N, alpha, X, incX, Y, incY, A, lda);
	}

	inline void syr(const enum order order, const triangular uplo, const int N, const float alpha, const float *X, const int incX, float *A, const int lda)
	{
		cblas_ssyr(order, uplo, N, alpha, X, incX, A, lda);
	}

	inline void spr(const enum order order, const triangular uplo, const int N, const float alpha, const float *X, const int incX, float *Ap)
	{
		cblas_sspr(order, uplo, N, alpha, X, incX, Ap);
	}

	inline void syr2(const enum order order, const triangular uplo, const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY, float *A, const int lda)
	{
		cblas_ssyr2(order, uplo, N, alpha, X, incX, Y, incY, A, lda);
	}

	inline void spr2(const enum order order, const triangular uplo, const int N, const float alpha, const float *X, const int incX, const float *Y, const int incY, float *A)
	{
		cblas_sspr2(order, uplo, N, alpha, X, incX, Y, incY, A);
	}

	inline void symv(const enum order order, const enum triangular uplo, const int N, const double alpha, const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)
	{
		cblas_dsymv(order, uplo, N, alpha, A, lda, X, incX, beta, Y, incY);
	}

	inline void sbmv(const enum order order, const enum triangular uplo, const int N, const int K, const double alpha, const double *A, const int lda, const double *X, const int incX, const double beta, double *Y, const int incY)
	{
		cblas_dsbmv(order, uplo, N, K, alpha, A, lda, X, incX, beta, Y, incY);
	}

	inline void spmv(const enum order order, const enum triangular uplo, const int N, const int K, const double alpha, const double *A, const int lda, const double *Ap, const double *X, const int incX, const double beta, double *Y, const int incY)
	{
		cblas_dspmv(order, uplo, N, alpha, Ap, X, incX, beta, Y, incY);
	}

	inline void ger(const enum order order, const int M, const int N, const double alpha, const double *X, const int incX, const double *Y, const int incY, double *A, const int lda)
	{
		cblas_dger(order, M, N, alpha, X, incX, Y, incY, A, lda);
	}

	inline void syr(const enum order order, const triangular uplo, const int N, const double alpha, const double *X, const int incX, double *A, const int lda)
	{
		cblas_dsyr(order, uplo, N, alpha, X, incX, A, lda);
	}

	inline void spr(const enum order order, const triangular uplo, const int N, const double alpha, const double *X, const int incX, double *Ap)
	{
		cblas_dspr(order, uplo, N, alpha, X, incX, Ap);
	}

	inline void syr2(const enum order order, const triangular uplo, const int N, const double alpha, const double *X, const int incX, const double *Y, const int incY, double *A, const int lda)
	{
		cblas_dsyr2(order, uplo, N, alpha, X, incX, Y, incY, A, lda);
	}

	inline void spr2(const enum order order, const triangular uplo, const int N, const double alpha, const double *X, const int incX, const double *Y, const int incY, double *A)
	{
		cblas_dspr2(order, uplo, N, alpha, X, incX, Y, incY, A);
	}

	// Routines with C and Z prefixes only

	inline void hemv(const enum order order, const enum triangular uplo, const int N, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> &beta, complex<float> *Y, const int incY)
	{
		cblas_chemv(order, uplo, N, &alpha, A, lda, X, incX, &beta, Y, incY);
	}

	inline void hbmv(const enum order order, const enum triangular uplo, const int N, const int K, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> *X, const int incX, const complex<float> &beta, complex<float> *Y, const int incY)
	{
		cblas_chbmv(order, uplo, N, K, &alpha, A, lda, X, incX, &beta, Y, incY);
	}

	inline void hpmv(const enum order order, const enum triangular uplo, const int N, const complex<float> &alpha, const complex<float> *Ap, const complex<float> *X, const int incX, const complex<float> &beta, complex<float> *Y, const int incY)
	{
		cblas_chpmv(order, uplo, N, &alpha, Ap, X, incX, &beta, Y, incY);
	}

	inline void geru(const enum order order, const int M, const int N, const complex<float> &alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *A, const int lda)
	{
		cblas_cgeru(order, M, N, &alpha, X, incX, Y, incY, A, lda);
	}

	inline void gerc(const enum order order, const int M, const int N, const complex<float> &alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *A, const int lda)
	{
		cblas_cgerc(order, M, N, &alpha, X, incX, Y, incY, A, lda);
	}

	inline void her(const enum order order, const enum triangular uplo, const int N, const float alpha, const complex<float> *X, const int incX, complex<float> *A, const int lda)
	{
		cblas_cher(order, uplo, N, alpha, X, incX, A, lda);
	}

	inline void hpr(const enum order order, const enum triangular uplo, const int N, const float alpha, const complex<float> *X, const int incX, complex<float> *A)
	{
		cblas_chpr(order, uplo, N, alpha, X, incX, A);
	}

	inline void her2(const enum order order, const enum triangular uplo, const int N, const complex<float> &alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *A, const int lda)
	{
		cblas_cher2(order, uplo, N, &alpha, X, incX, Y, incY, A, lda);
	}

	inline void hpr2(const enum order order, const enum triangular uplo, const int N, const complex<float> &alpha, const complex<float> *X, const int incX, const complex<float> *Y, const int incY, complex<float> *Ap)
	{
		cblas_chpr2(order, uplo, N, &alpha, X, incX, Y, incY, Ap);
	}

	inline void hemv(const enum order order, const enum triangular uplo, const int N, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> &beta, complex<double> *Y, const int incY)
	{
		cblas_zhemv(order, uplo, N, &alpha, A, lda, X, incX, &beta, Y, incY);
	}

	inline void hbmv(const enum order order, const enum triangular uplo, const int N, const int K, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> *X, const int incX, const complex<double> &beta, complex<double> *Y, const int incY)
	{
		cblas_zhbmv(order, uplo, N, K, &alpha, A, lda, X, incX, &beta, Y, incY);
	}

	inline void hpmv(const enum order order, const enum triangular uplo, const int N, const complex<double> &alpha, const complex<double> *Ap, const complex<double> *X, const int incX, const complex<double> &beta, complex<double> *Y, const int incY)
	{
		cblas_zhpmv(order, uplo, N, &alpha, Ap, X, incX, &beta, Y, incY);
	}

	inline void geru(const enum order order, const int M, const int N, const complex<double> &alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *A, const int lda)
	{
		cblas_zgeru(order, M, N, &alpha, X, incX, Y, incY, A, lda);
	}

	inline void gerc(const enum order order, const int M, const int N, const complex<double> &alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *A, const int lda)
	{
		cblas_zgerc(order, M, N, &alpha, X, incX, Y, incY, A, lda);
	}

	inline void her(const enum order order, const enum triangular uplo, const int N, const double alpha, const complex<double> *X, const int incX, complex<double> *A, const int lda)
	{
		cblas_zher(order, uplo, N, alpha, X, incX, A, lda);
	}

	inline void hpr(const enum order order, const enum triangular uplo, const int N, const double alpha, const complex<double> *X, const int incX, complex<double> *A)
	{
		cblas_zhpr(order, uplo, N, alpha, X, incX, A);
	}

	inline void her2(const enum order order, const enum triangular uplo, const int N, const complex<double> &alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *A, const int lda)
	{
		cblas_zher2(order, uplo, N, &alpha, X, incX, Y, incY, A, lda);
	}

	inline void hpr2(const enum order order, const enum triangular uplo, const int N, const complex<double> &alpha, const complex<double> *X, const int incX, const complex<double> *Y, const int incY, complex<double> *Ap)
	{
		cblas_zhpr2(order, uplo, N, &alpha, X, incX, Y, incY, Ap);
	}

	// ========================================================================
	// Overrides for level 3 BLAS
	// ========================================================================

	// Routines with standard 4 prefixes (S, D, C, Z)

	void gemm(const enum order order, const enum transpose transA, const enum transpose transB, const int M, const int N, const int K, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc)
	{
		cblas_sgemm(order, transA, transB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
	}

	void symm(const enum order order, const enum side side, const triangular uplo, const int M, const int N, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc)
	{
		cblas_ssymm(order, side, uplo, M, N, alpha, A, lda, B, ldb, beta, C, ldc);
	}

	void syrk(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const float alpha, const float *A, const int lda, const float beta, float *C, const int ldc)
	{
		cblas_ssyrk(order, uplo, trans, N, K, alpha, A, lda, beta, C, ldc);
	}

	void syr2k(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const float alpha, const float *A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc)
	{
		cblas_ssyr2k(order, uplo, trans, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
	}

	void trmm(const enum order order, const enum side side, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int M, const int N, const float alpha, const float *A, const int lda, float *B, const int ldb)
	{
		cblas_strmm(order, side, uplo, transA, diag, M, N, alpha, A, lda, B, ldb);
	}

	void trsm(const enum order order, const enum side side, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int M, const int N, const float alpha, const float *A, const int lda, float *B, const int ldb)
	{
		cblas_strsm(order, side, uplo, transA, diag, M, N, alpha, A, lda, B, ldb);
	}

	void gemm(const enum order order, const enum transpose transA, const enum transpose transB, const int M, const int N, const int K, const double alpha, const double *A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc)
	{
		cblas_dgemm(order, transA, transB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
	}

	void symm(const enum order order, const enum side side, const triangular uplo, const int M, const int N, const double alpha, const double *A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc)
	{
		cblas_dsymm(order, side, uplo, M, N, alpha, A, lda, B, ldb, beta, C, ldc);
	}

	void syrk(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const double alpha, const double *A, const int lda, const double beta, double *C, const int ldc)
	{
		cblas_dsyrk(order, uplo, trans, N, K, alpha, A, lda, beta, C, ldc);
	}

	void syr2k(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const double alpha, const double *A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc)
	{
		cblas_dsyr2k(order, uplo, trans, N, K, alpha, A, lda, B, ldb, beta, C, ldc);
	}

	void trmm(const enum order order, const enum side side, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int M, const int N, const double alpha, const double *A, const int lda, double *B, const int ldb)
	{
		cblas_dtrmm(order, side, uplo, transA, diag, M, N, alpha, A, lda, B, ldb);
	}

	void trsm(const enum order order, const enum side side, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int M, const int N, const double alpha, const double *A, const int lda, double *B, const int ldb)
	{
		cblas_dtrsm(order, side, uplo, transA, diag, M, N, alpha, A, lda, B, ldb);
	}

	void gemm(const enum order order, const enum transpose transA, const enum transpose transB, const int M, const int N, const int K, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> &beta, complex<float> *C, const int ldc)
	{
		cblas_cgemm(order, transA, transB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
	}

	void symm(const enum order order, const enum side side, const triangular uplo, const int M, const int N, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> &beta, complex<float> *C, const int ldc)
	{
		cblas_csymm(order, side, uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
	}

	void syrk(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> &beta, complex<float> *C, const int ldc)
	{
		cblas_csyrk(order, uplo, trans, N, K, &alpha, A, lda, &beta, C, ldc);
	}

	void syr2k(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> &beta, complex<float> *C, const int ldc)
	{
		cblas_csyr2k(order, uplo, trans, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
	}

	void trmm(const enum order order, const enum side side, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int M, const int N, const complex<float> &alpha, const complex<float> *A, const int lda, complex<float> *B, const int ldb)
	{
		cblas_ctrmm(order, side, uplo, transA, diag, M, N, &alpha, A, lda, B, ldb);
	}

	void trsm(const enum order order, const enum side side, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int M, const int N, const complex<float> &alpha, const complex<float> *A, const int lda, complex<float> *B, const int ldb)
	{
		cblas_ctrsm(order, side, uplo, transA, diag, M, N, &alpha, A, lda, B, ldb);
	}

	void gemm(const enum order order, const enum transpose transA, const enum transpose transB, const int M, const int N, const int K, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> &beta, complex<double> *C, const int ldc)
	{
		cblas_zgemm(order, transA, transB, M, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
	}

	void symm(const enum order order, const enum side side, const triangular uplo, const int M, const int N, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> &beta, complex<double> *C, const int ldc)
	{
		cblas_zsymm(order, side, uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
	}

	void syrk(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> &beta, complex<double> *C, const int ldc)
	{
		cblas_zsyrk(order, uplo, trans, N, K, &alpha, A, lda, &beta, C, ldc);
	}

	void syr2k(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> &beta, complex<double> *C, const int ldc)
	{
		cblas_zsyr2k(order, uplo, trans, N, K, &alpha, A, lda, B, ldb, &beta, C, ldc);
	}

	void trmm(const enum order order, const enum side side, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int M, const int N, const complex<double> &alpha, const complex<double> *A, const int lda, complex<double> *B, const int ldb)
	{
		cblas_ztrmm(order, side, uplo, transA, diag, M, N, &alpha, A, lda, B, ldb);
	}

	void trsm(const enum order order, const enum side side, const enum triangular uplo, const enum transpose transA, const enum diagonal diag, const int M, const int N, const complex<double> &alpha, const complex<double> *A, const int lda, complex<double> *B, const int ldb)
	{
		cblas_ztrsm(order, side, uplo, transA, diag, M, N, &alpha, A, lda, B, ldb);
	}

	// Routines with prefixes C and Z only

	void hemm(const enum order order, const enum side side, const triangular uplo, const int M, const int N, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const complex<float> &beta, complex<float> *C, const int ldc)
	{
		cblas_chemm(order, side, uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
	}

	void herk(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const float alpha, const complex<float> *A, const int lda, const float beta, float *C, const int ldc)
	{
		cblas_cherk(order, uplo, trans, N, K, alpha, A, lda, beta, C, ldc);
	}

	void her2k(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const complex<float> &alpha, const complex<float> *A, const int lda, const complex<float> *B, const int ldb, const float beta, complex<float> *C, const int ldc)
	{
		cblas_cher2k(order, uplo, trans, N, K, &alpha, A, lda, B, ldb, beta, C, ldc);
	}

	void hemm(const enum order order, const enum side side, const triangular uplo, const int M, const int N, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const complex<double> &beta, complex<double> *C, const int ldc)
	{
		cblas_zhemm(order, side, uplo, M, N, &alpha, A, lda, B, ldb, &beta, C, ldc);
	}

	void herk(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const double alpha, const complex<double> *A, const int lda, const double beta, double *C, const int ldc)
	{
		cblas_zherk(order, uplo, trans, N, K, alpha, A, lda, beta, C, ldc);
	}

	void her2k(const enum order order, const enum triangular uplo, const enum transpose trans, const int N, const int K, const complex<double> &alpha, const complex<double> *A, const int lda, const complex<double> *B, const int ldb, const double beta, complex<double> *C, const int ldc)
	{
		cblas_zher2k(order, uplo, trans, N, K, &alpha, A, lda, B, ldb, beta, C, ldc);
	}
}

// Overrides for aggregated data

namespace blas
{
	// ========================================================================
	// Overrides for level 1 BLAS functions
	// ========================================================================

	template <typename typeX, typename typeY> auto dot(const typeX &X, const typeY &Y)
	{
		return dot(X.dim(), X.data(), X.stride(), Y.data(), Y.stride());
	}

	template <typename typeX, typename typeY> auto dotu(const typeX &X, const typeY &Y)
	{
		return dotu(X.dim(), X.data(), X.stride(), Y.data(), Y.stride());
	}

	template <typename typeX, typename typeY> auto dotc(const typeX &X, const typeY &Y)
	{
		return dotc(X.dim(), X.data(), X.stride(), Y.data(), Y.stride());
	}

	template <typename typeX> auto nrm2(const typeX &X)
	{
		return nrm2(X.dim(), X.data(), X.stride());
	}

	template <typename typeX> auto asum(const typeX &X)
	{
		return asum(X.dim(), X.data(), X.stride());
	}

	template <typename typeX> auto iamax(const typeX &X)
	{
		return amax(X.dim(), X.data(), X.stride());
	}

	template <typename typeX, typename typeY> void swap(typeX &X, typeY &Y)
	{
		swap(X.dim(), X.data(), X.stride(), Y.data(), Y.stride());
	}

	template <typename typeX, typename typeY> void copy(const typeX &X, typeY &Y)
	{
		copy(X.dim(), X.data(), X.stride(), Y.data(), Y.stride());
	}

	template <typename scalar, typename typeX, typename typeY> void axpy(const scalar alpha, const typeX &X, typeY &Y)
	{
		axpy(X.dim(), alpha, X.data(), X.stride(), Y.data(), Y.stride());
	}

	template <typename scalar, typename typeX> void scal(const scalar alpha, typeX &X)
	{
		scal(X.dim(), alpha, X.data(), X.stride());
	}

	// ========================================================================
	// Overrides for level 2 BLAS
	// ========================================================================

	template <typename scalar, typename typeA, typename typeX, typename typeY>
	void gemv(const enum order order, const scalar alpha, const typeA &A, const typeX &X, const scalar beta, typeY &Y)
	{
		assert(A.row_size() == X.dim());
		assert(A.column_size() == Y.dim());
		gemv(order, A.transposition(), alpha, A.data(), A.column_size(), A.row_size(), A.stride(), X.data(), X.stride(), beta, Y.data(), Y.stride());
	}
}

#endif // file
