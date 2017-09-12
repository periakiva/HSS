/* Code using NVIDIA cuBLAS for BLAS and Intel MKL for LAPACK with OpenMP on host
   driver 1: test low rank SVD routines
*/

#define min(x,y) (((x) < (y)) ? (x) : (y))
#define max(x,y) (((x) > (y)) ? (x) : (y))

#include "rank_revealing_algorithms_mkl_and_cublas.h"

int main()
{
    int i, j, m, n, k, p, q, s, vnum, frank;
    double normM,normU,normS,normV,normP,percent_error;
    mat *M, *U, *S, *V, *P;
    time_t start_time, end_time;
    char *M_file = "../../matrix_data/A_mat_6kx12k.bin";
    //char *M_file = "../data/A_mat_1kx2k.bin";

    printf("Initializing cuBLAS\n");
	cublasCreate(&handle);

 
    printf("loading matrix from %s\n", M_file);
    M = matrix_load_from_binary_file(M_file);
    m = M->nrows;
    n = M->ncols;
    printf("sizes of M are %d by %d\n", m, n);

    // now test low rank SVD of M..
    k = 2000;
	p = 40;
	q = 2;
	s = 1;
	vnum = 1;
    
    printf("calling random SVD..\n");
    time(&start_time);
        low_rank_svd_rand_decomp_fixed_rank(M, k, p, vnum, q, s, &frank, &U, &S, &V);
    time(&end_time);
    printf("elapsed time: about %d seconds\n", (int)difftime(end_time,start_time));

    // form product matrix
    P = matrix_new(m,n);
    form_svd_product_matrix(U,S,V,P);

    // get norms of each
    normM = get_matrix_frobenius_norm(M);
    normU = get_matrix_frobenius_norm(U);
    normS = get_matrix_frobenius_norm(S);
    normV = get_matrix_frobenius_norm(V);
    normP = get_matrix_frobenius_norm(P);
    printf("normM = %f ; normU = %f ; normS = %f ; normV = %f ; normP = %f\n", normM, normU, normS, normV, normP);

    // calculate percent error
    percent_error = get_percent_error_between_two_mats(M,P);
    printf("percent_error between M and U S V^T = %f\n", percent_error);


    // delete and exit
    printf("delete and exit..\n");
    matrix_delete(M);
    matrix_delete(U);
    matrix_delete(S);
    matrix_delete(V);
    matrix_delete(P);

    printf("Shutting down cuBLAS\n");
	cublasDestroy(handle);
	return 0;
}

