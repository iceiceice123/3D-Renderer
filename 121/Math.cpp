#include<math.h>
#include"Math.h"
typedef unsigned int IUINT32;

int check_range(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }

float interpolation(float x1, float x2, float t) { return x1 + (x2 - x1) * t; }

float vector_length(const vector *v) {
	float sq = v->x * v->x + v->y * v->y + v->z * v->z;
	return (float)sqrt(sq);
}

void vector_add(vector *z, const vector *x, const vector *y) {
	z->x = x->x + y->x;
	z->y = x->y + y->y;
	z->z = x->z + y->z;
	z->w = 1.0f;
}

void vector_sub(vector *z, const vector *x, const vector *y) {
	z->x = x->x - y->x;
	z->y = x->y - y->y;
	z->z = x->z - y->z;
	z->w = 1.0f;
}

float vector_dotproduct(const vector *x, const vector *y) {
	return x->x * y->x + x->y * y->y + x->z * y->z;
}

void vector_crossproduct(vector *z, const vector *x, const vector *y) {
	float m1, m2, m3;
	m1 = x->y * y->z - x->z * y->y;
	m2 = x->z * y->x - x->x * y->z;
	m3 = x->x * y->y - x->y * y->x;
	z->x = m1;
	z->y = m2;
	z->z = m3;
	z->w = 1.0f;
}

void vector_interpolation(vector *z, const vector *x1, const vector *x2, float t) {
	z->x = interpolation(x1->x, x2->x, t);
	z->y = interpolation(x1->y, x2->y, t);
	z->z = interpolation(x1->z, x2->z, t);
	z->w = 1.0f;
}

void vector_normalize(vector *v) {
	if (vector_length(v)!= 0.0f) {
		float oneOverL = 1.0f / vector_length(v);
		v->x *= oneOverL;
		v->y *= oneOverL;
		v->z *= oneOverL;
	}
}

void matrix_add(matrix *c, const matrix *a, const matrix *b) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] + b->m[i][j];
	}
}

void matrix_sub(matrix *c, const matrix *a, const matrix *b) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] - b->m[i][j];
	}
}

void matrix_mul(matrix *c, const matrix *a, const matrix *b) {
	matrix z;
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			z.m[j][i] = (a->m[j][0] * b->m[0][i]) +
				(a->m[j][1] * b->m[1][i]) +(a->m[j][2] * b->m[2][i]) +(a->m[j][3] * b->m[3][i]);
		}
	}
	c[0] = z;
}

void matrix_constant(matrix *c, const matrix *a, float f) {
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)
			c->m[i][j] = a->m[i][j] * f;
	}
}

void matrix_vector(vector *y, const vector *x, const matrix *m) {
	float X = x->x, Y = x->y, Z = x->z, W = x->w;
	y->x = X * m->m[0][0] + Y * m->m[1][0] + Z * m->m[2][0] + W * m->m[3][0];
	y->y = X * m->m[0][1] + Y * m->m[1][1] + Z * m->m[2][1] + W * m->m[3][1];
	y->z = X * m->m[0][2] + Y * m->m[1][2] + Z * m->m[2][2] + W * m->m[3][2];
	y->w = X * m->m[0][3] + Y * m->m[1][3] + Z * m->m[2][3] + W * m->m[3][3];
}

void matrix_identity(matrix *m) {
	m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f;
	m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
}

void matrix_zero(matrix *m) {
	m->m[0][0] = m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][1] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][2] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = m->m[3][3] = 0.0f;
}

void matrix_rotate(matrix *m, float x, float y, float z, float theta) {
	float qsin = (float)sin(theta * 0.5f);
	float qcos = (float)cos(theta * 0.5f);
	vector vec = { x, y, z, 1.0f };
	vector_normalize(&vec);
	x = vec.x * qsin;
	y = vec.y * qsin;
	float w = qcos;
	z = vec.z * qsin;
	m->m[0][0] = 1 - 2 * y * y - 2 * z * z;
	m->m[1][0] = 2 * x * y - 2 * w * z;
	m->m[2][0] = 2 * x * z + 2 * w * y;
	m->m[0][1] = 2 * x * y + 2 * w * z;
	m->m[1][1] = 1 - 2 * x * x - 2 * z * z;
	m->m[2][1] = 2 * y * z - 2 * w * x;
	m->m[0][2] = 2 * x * z - 2 * w * y;
	m->m[1][2] = 2 * y * z + 2 * w * x;
	m->m[2][2] = 1 - 2 * x * x - 2 * y * y;
	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
	m->m[3][3] = 1.0f;
}

void set_video(matrix *m, const vector *eye, const vector *at, const vector *up) {
	vector new_x, new_y, new_z;

	vector_sub(&new_z, at, eye);
	vector_normalize(&new_z);
	vector_crossproduct(&new_x, up, &new_z);
	vector_normalize(&new_x);
	vector_crossproduct(&new_y, &new_z, &new_x);

	m->m[0][0] = new_x.x;
	m->m[1][0] = new_x.y;
	m->m[2][0] = new_x.z;
	m->m[3][0] = -vector_dotproduct(&new_x, eye);

	m->m[0][1] = new_y.x;
	m->m[1][1] = new_y.y;
	m->m[2][1] = new_y.z;
	m->m[3][1] = -vector_dotproduct(&new_y, eye);

	m->m[0][2] = new_z.x;
	m->m[1][2] = new_z.y;
	m->m[2][2] = new_z.z;
	m->m[3][2] = -vector_dotproduct(&new_z, eye);

	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
	m->m[3][3] = 1.0f;
}

