#ifndef _VIENNACL_MATRIX_PROD_ROW_COL_COL_SOURCE_HPP_
#define _VIENNACL_MATRIX_PROD_ROW_COL_COL_SOURCE_HPP_
//Automatically generated file from aux-directory, do not edit manually!
namespace viennacl
{
 namespace linalg
 {
  namespace kernels
  {
const char * const matrix_prod_row_col_col_align1_prod_TA = 
"// file automatically generated - do not edit!\n"
"// matrix-matrix multiplication C = A^T * B\n"
"// matrix layouts: C...col_major, A...row_major, B...col_major\n"
"__kernel void prod_TA(\n"
"          __global const float * A,\n"
"          unsigned int A_rows,\n"
"          unsigned int A_cols,\n"
"          unsigned int A_internal_rows,\n"
"          unsigned int A_internal_cols,\n"
"          __global const float * B,  \n"
"          unsigned int B_rows,\n"
"          unsigned int B_cols,\n"
"          unsigned int B_internal_rows,\n"
"          unsigned int B_internal_cols,\n"
"          __global float * C,\n"
"          unsigned int C_rows,\n"
"          unsigned int C_cols,\n"
"          unsigned int C_internal_rows,\n"
"          unsigned int C_internal_cols,\n"
"          __local float * bufA,\n"
"          __local float * bufB) \n"
"{ \n"
"  int block_size = get_local_size(0);\n"
"  int row_block_id = get_group_id(0);\n"
"  int col_block_id = get_group_id(1);\n"
"  int row_thread_id = get_local_id(0);\n"
"  int col_thread_id = get_local_id(1);\n"
"  int aBegin = row_block_id * block_size;\n"
"  int aStep = block_size * A_internal_cols;\n"
"  int bBegin = col_block_id * block_size * B_internal_rows;\n"
"  int bStep = block_size;\n"
"  int block_num = A_rows / block_size;\n"
"  if (block_num * block_size != A_rows)\n"
"    ++block_num;\n"
"  float Csub = 0;\n"
"  int aOffset = row_thread_id + col_thread_id * A_internal_cols;\n"
"  int bOffset = row_thread_id + col_thread_id * B_internal_rows;\n"
"  for (int block = 0;\n"
"           block < block_num;\n"
"           ++block)\n"
"  {\n"
"    if (block * block_size + col_thread_id < A_rows && get_global_id(0) < A_cols)\n"
"      bufA[row_thread_id * block_size + col_thread_id] = A[aBegin + aOffset];\n"
"    else\n"
"      bufA[row_thread_id * block_size + col_thread_id] = 0;\n"
"    if ( (block * block_size + row_thread_id < B_rows) && get_global_id(1) < B_cols )\n"
"      bufB[row_thread_id * block_size + col_thread_id] = B[bBegin + bOffset]; \n"
"    else\n"
"      bufB[row_thread_id * block_size + col_thread_id] = 0;\n"
"    barrier(CLK_LOCAL_MEM_FENCE);\n"
"    for (int k = 0; k < block_size; ++k)\n"
"      Csub += bufA[row_thread_id * block_size + k] * bufB[k * block_size + col_thread_id];\n"
"    barrier(CLK_LOCAL_MEM_FENCE);\n"
"    aBegin += aStep;\n"
"    bBegin += bStep;\n"
"  }\n"
"  if (get_global_id(0) < A_cols && get_global_id(1) < B_cols)\n"
"    C[get_global_id(0) + get_global_id(1) * C_internal_rows] = Csub;\n"
"}\n"
; //matrix_prod_row_col_col_align1_prod_TA

const char * const matrix_prod_row_col_col_align1_prod_TT = 
"// file automatically generated - do not edit!\n"
"// matrix-matrix multiplication C = A^T * B^T\n"
"// matrix layouts: C...col_major, A...row_major, B...col_major\n"
"__kernel void prod_TT(\n"
"          __global const float * A,\n"
"          unsigned int A_rows,\n"
"          unsigned int A_cols,\n"
"          unsigned int A_internal_rows,\n"
"          unsigned int A_internal_cols,\n"
"          __global const float * B,  \n"
"          unsigned int B_rows,\n"
"          unsigned int B_cols,\n"
"          unsigned int B_internal_rows,\n"
"          unsigned int B_internal_cols,\n"
"          __global float * C,\n"
"          unsigned int C_rows,\n"
"          unsigned int C_cols,\n"
"          unsigned int C_internal_rows,\n"
"          unsigned int C_internal_cols,\n"
"          __local float * bufA,\n"
"          __local float * bufB) \n"
"{ \n"
"  int block_size = get_local_size(0);\n"
"  int row_block_id = get_group_id(0);\n"
"  int col_block_id = get_group_id(1);\n"
"  int row_thread_id = get_local_id(0);\n"
"  int col_thread_id = get_local_id(1);\n"
"  int aBegin = row_block_id * block_size;\n"
"  int aStep = block_size * A_internal_cols;\n"
"  int bBegin = col_block_id * block_size;\n"
"  int bStep = block_size * B_internal_rows;\n"
"  int block_num = A_rows / block_size;\n"
"  if (block_num * block_size != A_rows)\n"
"    ++block_num;\n"
"  float Csub = 0;\n"
"  int aOffset = row_thread_id + col_thread_id * A_internal_cols;\n"
"  int bOffset = row_thread_id * B_internal_rows + col_thread_id;\n"
"  for (int block = 0;\n"
"           block < block_num;\n"
"           ++block)\n"
"  {\n"
"    if (block * block_size + col_thread_id < A_rows && get_global_id(0) < A_cols)\n"
"      bufA[row_thread_id * block_size + col_thread_id] = A[aBegin + aOffset];\n"
"    else\n"
"      bufA[row_thread_id * block_size + col_thread_id] = 0;\n"
"    if ( (block * block_size + row_thread_id < B_cols) && get_global_id(1) < B_rows )\n"
"      bufB[row_thread_id * block_size + col_thread_id] = B[bBegin + bOffset]; \n"
"    else\n"
"      bufB[row_thread_id * block_size + col_thread_id] = 0;\n"
"    barrier(CLK_LOCAL_MEM_FENCE);\n"
"    for (int k = 0; k < block_size; ++k)\n"
"      Csub += bufA[row_thread_id * block_size + k] * bufB[k * block_size + col_thread_id];\n"
"    barrier(CLK_LOCAL_MEM_FENCE);\n"
"    aBegin += aStep;\n"
"    bBegin += bStep;\n"
"  }\n"
"  if (get_global_id(0) < A_cols && get_global_id(1) < B_rows)\n"
"    C[get_global_id(0) + get_global_id(1) * C_internal_rows] = Csub;\n"
"}\n"
; //matrix_prod_row_col_col_align1_prod_TT

const char * const matrix_prod_row_col_col_align1_prod_AT = 
"// file automatically generated - do not edit!\n"
"// matrix-matrix multiplication C = A * B^T\n"
"// matrix layouts: C...col_major, A...row_major, B...col_major\n"
"__kernel void prod_AT(\n"
"          __global const float * A,\n"
"          unsigned int A_rows,\n"
"          unsigned int A_cols,\n"
"          unsigned int A_internal_rows,\n"
"          unsigned int A_internal_cols,\n"
"          __global const float * B,  \n"
"          unsigned int B_rows,\n"
"          unsigned int B_cols,\n"
"          unsigned int B_internal_rows,\n"
"          unsigned int B_internal_cols,\n"
"          __global float * C,\n"
"          unsigned int C_rows,\n"
"          unsigned int C_cols,\n"
"          unsigned int C_internal_rows,\n"
"          unsigned int C_internal_cols,\n"
"          __local float * bufA,\n"
"          __local float * bufB) \n"
"{ \n"
"  int block_size = get_local_size(0);\n"
"  int row_block_id = get_group_id(0);\n"
"  int col_block_id = get_group_id(1);\n"
"  int row_thread_id = get_local_id(0);\n"
"  int col_thread_id = get_local_id(1);\n"
"  int aBegin = row_block_id * block_size * A_internal_cols;\n"
"  int aStep = block_size;\n"
"  int bBegin = col_block_id * block_size;\n"
"  int bStep = block_size * B_internal_rows;\n"
"  int block_num = A_cols / block_size;\n"
"  if (block_num * block_size != A_cols)\n"
"    ++block_num;\n"
"  float Csub = 0;\n"
"  int aOffset = row_thread_id * A_internal_cols + col_thread_id;\n"
"  int bOffset = row_thread_id * B_internal_rows + col_thread_id;\n"
"  for (int block = 0;\n"
"           block < block_num;\n"
"           ++block)\n"
"  {\n"
"    if (block * block_size + col_thread_id < A_cols && get_global_id(0) < A_rows)\n"
"      bufA[row_thread_id * block_size + col_thread_id] = A[aBegin + aOffset];\n"
"    else\n"
"      bufA[row_thread_id * block_size + col_thread_id] = 0;\n"
"    if ( (block * block_size + row_thread_id < B_cols) && get_global_id(1) < B_rows )\n"
"      bufB[row_thread_id * block_size + col_thread_id] = B[bBegin + bOffset]; \n"
"    else\n"
"      bufB[row_thread_id * block_size + col_thread_id] = 0;\n"
"    barrier(CLK_LOCAL_MEM_FENCE);\n"
"    for (int k = 0; k < block_size; ++k)\n"
"      Csub += bufA[row_thread_id * block_size + k] * bufB[k * block_size + col_thread_id];\n"
"    barrier(CLK_LOCAL_MEM_FENCE);\n"
"    aBegin += aStep;\n"
"    bBegin += bStep;\n"
"  }\n"
"  if (get_global_id(0) < A_rows && get_global_id(1) < B_rows)\n"
"    C[get_global_id(0) + get_global_id(1) * C_internal_rows] = Csub;\n"
"}\n"
; //matrix_prod_row_col_col_align1_prod_AT

const char * const matrix_prod_row_col_col_align1_prod_AA = 
"// file automatically generated - do not edit!\n"
"// matrix-matrix multiplication C = A * B\n"
"// matrix layouts: C...col_major, A...row_major, B...col_major\n"
"__kernel void prod_AA(\n"
"          __global const float * A,\n"
"          unsigned int A_rows,\n"
"          unsigned int A_cols,\n"
"          unsigned int A_internal_rows,\n"
"          unsigned int A_internal_cols,\n"
"          __global const float * B,  \n"
"          unsigned int B_rows,\n"
"          unsigned int B_cols,\n"
"          unsigned int B_internal_rows,\n"
"          unsigned int B_internal_cols,\n"
"          __global float * C,\n"
"          unsigned int C_rows,\n"
"          unsigned int C_cols,\n"
"          unsigned int C_internal_rows,\n"
"          unsigned int C_internal_cols,\n"
"          __local float * bufA,\n"
"          __local float * bufB) \n"
"{ \n"
"  int block_size = get_local_size(0);\n"
"  int row_block_id = get_group_id(0);\n"
"  int col_block_id = get_group_id(1);\n"
"  int row_thread_id = get_local_id(0);\n"
"  int col_thread_id = get_local_id(1);\n"
"  int aBegin = row_block_id * block_size * A_internal_cols;\n"
"  int aStep = block_size;\n"
"  int bBegin = col_block_id * block_size * B_internal_rows;\n"
"  int bStep = block_size;\n"
"  int block_num = A_cols / block_size;\n"
"  if (block_num * block_size != A_cols)\n"
"    ++block_num;\n"
"  float Csub = 0;\n"
"  int aOffset = row_thread_id * A_internal_cols + col_thread_id;\n"
"  int bOffset = row_thread_id + col_thread_id * B_internal_rows;\n"
"  for (int block = 0;\n"
"           block < block_num;\n"
"           ++block)\n"
"  {\n"
"    if (block * block_size + col_thread_id < A_cols && get_global_id(0) < A_rows)\n"
"      bufA[row_thread_id * block_size + col_thread_id] = A[aBegin + aOffset];\n"
"    else\n"
"      bufA[row_thread_id * block_size + col_thread_id] = 0;\n"
"    if ( (block * block_size + row_thread_id < B_rows) && get_global_id(1) < B_cols )\n"
"      bufB[row_thread_id * block_size + col_thread_id] = B[bBegin + bOffset]; \n"
"    else\n"
"      bufB[row_thread_id * block_size + col_thread_id] = 0;\n"
"    barrier(CLK_LOCAL_MEM_FENCE);\n"
"    for (int k = 0; k < block_size; ++k)\n"
"      Csub += bufA[row_thread_id * block_size + k] * bufB[k * block_size + col_thread_id];\n"
"    barrier(CLK_LOCAL_MEM_FENCE);\n"
"    aBegin += aStep;\n"
"    bBegin += bStep;\n"
"  }\n"
"  if (get_global_id(0) < A_rows && get_global_id(1) < B_cols)\n"
"    C[get_global_id(0) + get_global_id(1) * C_internal_rows] = Csub;\n"
"}\n"
; //matrix_prod_row_col_col_align1_prod_AA

  }  //namespace kernels
 }  //namespace linalg
}  //namespace viennacl
#endif
