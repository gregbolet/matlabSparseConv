/* 
 * sparseConv.c - an interface MEX file to convert MATLAB
 * sparse matrices to a plaintext CSR format 
 * 
 * LHS and RHS Expected Arguments:
 *    LHS: Output file name (outName)
 *    RHS: SPD Matrix (A), Name of Matrix (name), Description of Matrix (desc)
 *
 * The calling syntax is:
 *
 * outName = sparseConv(A, name, desc)
 *
 */

#include "mex.h"
#include "matrix.h"
#include <stdio.h>
#include <string.h>

void verifyInputArgs(int nlhs,       mxArray *plhs[],
		     int nrhs, const mxArray *prhs[]);


void mexFunction(int nlhs,       mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]){
    
    //Check that all the inputs are correct
    verifyInputArgs(nlhs, plhs, nrhs, prhs);

    char *matName, *matDesc;
       
    //Get the input matrix
    const mxArray* mxMatA = prhs[0];
    
    //MATLAB matrices in sparse format are already in CSR format
    //Assuming we don't need to make a copy of the inputs...    
    mwIndex* row_inds =  mxGetIr(mxMatA);
    mwIndex* col_ptrs =  mxGetJc(mxMatA);
    double* vals = mxGetDoubles(mxMatA);
    
    //Number of columns and non-zeroes in the matrix
    int nc = mxGetN(mxMatA);
    mwIndex nnz = col_ptrs[nc];

    //Get the name and description of the matrix
    matName = mxArrayToString(prhs[1]);
    matDesc = mxArrayToString(prhs[2]);
    
    printf("Output file name is: %s \n", matName);
    
    //Setup the file to write to
    FILE *outfile = fopen(matName, "w");

    //Print the description to the file
    fputs(matDesc, outfile);
    
    //Print the number of rows/cols and the number of nonzeros
    fprintf(outfile, "\n\n%d %d\n\n", nc, (int) nnz);

    int i;
    
    //Print the column pointers
    for(i = 0; i < nc+1; ++i){
	fprintf(outfile, "%d ", (int) col_ptrs[i]+1);
    }

    fprintf(outfile, "\n\n");
    
    //Print the row indices
    for(i = 0; i < nnz; ++i){
	fprintf(outfile, "%d ", (int) row_inds[i]+1);
    }
    
    fprintf(outfile, "\n\n");
    
    //Print the nonzeros
    for(i = 0; i < nnz; ++i){
	fprintf(outfile, "%.13e ", vals[i]);
    }
    
    //Set the return name
    plhs[0] = mxCreateString(matName);
    
    fclose(outfile);  
}


//Used to check the input from MATLAB before
//going on to do any work
void verifyInputArgs(int nlhs,       mxArray *plhs[],
		     int nrhs, const mxArray *prhs[]){

    // Verify that the right-hand side arguments
    //given to us is of the proper size
    if(nrhs != 3) {
	mexErrMsgTxt("Expected 3 input arguments.");
    }
    
    // Verify that the input matrix is of type double.
    if(!mxIsDouble(prhs[0]) || mxIsComplex(prhs[0])) {
	mexErrMsgTxt("Input matrix must be of type double.");
    }

    // Verify that the dimensions are square 
    if(mxGetM(prhs[0]) != mxGetN(prhs[0])) {
	mexErrMsgTxt("Input matrix is not a square matrix.");
    }

    //Verify that the input matrix is sparse
    if(!mxIsSparse(prhs[0])){
	mexErrMsgTxt("Input matrix not in sparse format.");
    }

    //Verify that the name given is not empty
    if(0){
	mexErrMsgTxt("Input matrix name is empty.");
    }

    //Verify that the description given is not empty
    if(0){
	mexErrMsgTxt("Input matrix description is empty.");
    }
}
