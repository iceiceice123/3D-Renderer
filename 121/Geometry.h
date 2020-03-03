#pragma once

#include"math.h"
typedef struct { float r, g, b; } color;
typedef struct { float u, v; } texcoord;
typedef struct { point pos; texcoord tc; color color; float oneOverW; } vertex;
typedef struct { vertex v, v1, v2; } edge;
typedef struct { float top, bottom; edge left, right; } triangle;
typedef struct { vertex v, step; int x, y, w; } scanline;

void vertex_init(vertex *v);

void vertex_interpolation(vertex *v, const vertex *a, const vertex *b, float t);

void vertex_division(vertex *v, const vertex *a, const vertex *b, float w);

void vertex_add(vertex *v, const vertex *x);
// �������������� 0-2��ƽ����ƽ��������,������������
int make_triangle(triangle *tri, const vertex *p1, const vertex *p2, const vertex *p3);
// ���� Y ��������ɨ���ߵ������˵�
void get_Y(triangle *tri, float y);
// �����������ߵĶ˵㣬�����ɨ���ߵ����Ͳ���
void init_scanline(const triangle *tri, scanline *scanline, int y);
