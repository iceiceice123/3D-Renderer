#pragma once
#include"math.h"
typedef struct {
	matrix world;         // ��������任����
	matrix view;          // ��Ӱ������任����
	matrix projection;    // ͶӰ�任����
	matrix transform;     // transform =poi world * view * projection
	float w, h;           // ��Ļ���
}	transform;



// ��ʼ����������Ļ����
void init_transform(transform *ts, int width, int height);
// ��ʸ�� x ���� project 
void vector_project(const transform *ts, vector *y, const vector *x);
// �������
void update_transform(transform *ts);
// ����������ͬ cvv �ı߽����ڲü�
int check_cvv(const vector *v);
// �õ���Ļ����
void transform_screen(const transform *ts, vector *y, const vector *x);
// ת������׼������ fovy��׶������¼н� aspect�ݺ�ȣ����ӿռ��ȳ��Ը߶ȡ�
void MatrixPerspectiveFovLH(matrix *m, float fovy, float aspect, float zn, float zf);