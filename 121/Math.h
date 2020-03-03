#pragma once
#include<math.h>
typedef unsigned int IUINT32;
typedef struct { float m[4][4]; } matrix;
typedef struct { float x, y, z, w; } vector;
typedef vector point;

int check_range(int x, int min, int max);

// 计算插值
float interpolation(float x1, float x2, float t);
// 计算模长
float vector_length(const vector *v);
// 向量加法
void vector_add(vector *z, const vector *x, const vector *y);
// 向量减法
void vector_sub(vector *z, const vector *x, const vector *y);
// 向量点乘
float vector_dotproduct(const vector *x, const vector *y);
// 向量叉乘
void vector_crossproduct(vector *z, const vector *x, const vector *y);
// 向量插值
void vector_interpolation(vector *z, const vector *x1, const vector *x2, float t);
// 向量单位化
void vector_normalize(vector *v);
// 矩阵加法
void matrix_add(matrix *c, const matrix *a, const matrix *b);
// 矩阵减法
void matrix_sub(matrix *c, const matrix *a, const matrix *b);
// 矩阵乘法
void matrix_mul(matrix *c, const matrix *a, const matrix *b);
// 矩阵乘常数
void matrix_constant(matrix *c, const matrix *a, float f);
// 矩阵乘向量
void matrix_vector(vector *y, const vector *x, const matrix *m);
// 矩阵单位化
void matrix_identity(matrix *m);
// 矩阵归零
void matrix_zero(matrix *m);
// 旋转
void matrix_rotate(matrix *m, float x, float y, float z, float theta);
// 设置摄像机
void set_video(matrix *m, const vector *eye, const vector *at, const vector *up);
