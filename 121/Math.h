#pragma once
#include<math.h>
typedef unsigned int IUINT32;
typedef struct { float m[4][4]; } matrix;
typedef struct { float x, y, z, w; } vector;
typedef vector point;

int check_range(int x, int min, int max);

// �����ֵ
float interpolation(float x1, float x2, float t);
// ����ģ��
float vector_length(const vector *v);
// �����ӷ�
void vector_add(vector *z, const vector *x, const vector *y);
// ��������
void vector_sub(vector *z, const vector *x, const vector *y);
// �������
float vector_dotproduct(const vector *x, const vector *y);
// �������
void vector_crossproduct(vector *z, const vector *x, const vector *y);
// ������ֵ
void vector_interpolation(vector *z, const vector *x1, const vector *x2, float t);
// ������λ��
void vector_normalize(vector *v);
// ����ӷ�
void matrix_add(matrix *c, const matrix *a, const matrix *b);
// �������
void matrix_sub(matrix *c, const matrix *a, const matrix *b);
// ����˷�
void matrix_mul(matrix *c, const matrix *a, const matrix *b);
// ����˳���
void matrix_constant(matrix *c, const matrix *a, float f);
// ���������
void matrix_vector(vector *y, const vector *x, const matrix *m);
// ����λ��
void matrix_identity(matrix *m);
// �������
void matrix_zero(matrix *m);
// ��ת
void matrix_rotate(matrix *m, float x, float y, float z, float theta);
// ���������
void set_video(matrix *m, const vector *eye, const vector *at, const vector *up);
