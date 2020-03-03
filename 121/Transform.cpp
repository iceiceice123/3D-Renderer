#include"math.h"
#include"Transform.h"

void init_transform(transform *ts, int width, int height) {
	matrix_identity(&ts->world);
	matrix_identity(&ts->view);
	ts->w = (float)width;
	ts->h = (float)height;
	MatrixPerspectiveFovLH(&ts->projection, 3.1415926f*0.7f, (float)width / ((float)height), 1.0f, 500.0f);
	update_transform(ts);
}

void vector_project(const transform *ts, vector *y, const vector *x) {
	matrix_vector(y, x, &ts->transform);
}

void update_transform(transform *ts) {
	matrix m;
	matrix_mul(&m, &ts->world, &ts->view);
	matrix_mul(&ts->transform, &m, &ts->projection);
}

int check_cvv(const vector *v) {
	float w = v->w;
	int res = 0;
	if (v->x < -w) res|= 1;
	if (v->x > w) res |= 2;
	if (v->y < -w) res |= 4;
	if (v->y > w) res |= 8;
	if (v->z < 0.0f) res |= 16;
	if (v->z > w) res |= 32;
	return res;
}

void transform_screen(const transform *ts, vector *y, const vector *x) {
	float oneOverW = 1.0f / x->w;
	y->x = (x->x * oneOverW + 1.0f) * ts->w * 0.5f;
	y->y = (1.0f - x->y * oneOverW) * ts->h * 0.5f;
	y->z = x->z * oneOverW;
	y->w = 1.0f;
}

void MatrixPerspectiveFovLH(matrix *m, float fovy, float aspect, float zn, float zf) {
	float fax = 1.0f / (float)tan(fovy * 0.5f);
	matrix_zero(m);
	m->m[0][0] = (float)(fax / aspect);
	m->m[1][1] = (float)(fax);
	m->m[2][2] = (float)zf / (zf - zn);
	m->m[3][2] = (float)-zn * zf / (zf - zn);
	m->m[2][3] = 1;
}



