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
// 根据三个点生成 0-2个平定或平底三角形,返回生成数量
int make_triangle(triangle *tri, const vertex *p1, const vertex *p2, const vertex *p3);
// 按照 Y 坐标计算出扫描线的两个端点
void get_Y(triangle *tri, float y);
// 根据左右两边的端点，计算出扫描线的起点和步长
void init_scanline(const triangle *tri, scanline *scanline, int y);
