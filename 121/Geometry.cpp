#include"math.h"
#include"Geometry.h"

//这里u，v除以w的倒数的原因是u,v和屏幕坐标不成线性关系，而u/w, v/w承线性关系
void vertex_init(vertex *v) {
	float oneOverW = 1.0f / v->pos.w;
	v->oneOverW = oneOverW;
	v->color.r *= oneOverW;
	v->color.g *= oneOverW;
	v->color.b *= oneOverW;
	v->tc.u *= oneOverW;
	v->tc.v *= oneOverW;

}

void vertex_interpolation(vertex *v, const vertex *a, const vertex *b, float t) {
	vector_interpolation(&v->pos, &a->pos, &b->pos, t);
	v->tc.u = interpolation(a->tc.u, b->tc.u, t);
	v->tc.v = interpolation(a->tc.v, b->tc.v, t);
	v->color.r = interpolation(a->color.r, b->color.r, t);
	v->color.g = interpolation(a->color.g, b->color.g, t);
	v->color.b = interpolation(a->color.b, b->color.b, t);
	v->oneOverW = interpolation(a->oneOverW, b->oneOverW, t);
}

void vertex_division(vertex *v, const vertex *a, const vertex *b, float w) {
	float oneOverW = 1.0f / w;
	v->pos.x = (b->pos.x - a->pos.x) * oneOverW;
	v->pos.y = (b->pos.y - a->pos.y) * oneOverW;
	v->pos.z = (b->pos.z - a->pos.z) * oneOverW;
	v->pos.w = (b->pos.w - a->pos.w) * oneOverW;
	v->tc.u = (b->tc.u - a->tc.u) * oneOverW;
	v->tc.v = (b->tc.v - a->tc.v) * oneOverW;
	v->color.r = (b->color.r - a->color.r) * oneOverW;
	v->color.g = (b->color.g - a->color.g) * oneOverW;
	v->color.b = (b->color.b - a->color.b) * oneOverW;
	v->oneOverW = (b->oneOverW - a->oneOverW) * oneOverW;
}

void vertex_add(vertex *v, const vertex *x) {
	v->pos.x += x->pos.x;
	v->pos.y += x->pos.y;
	v->pos.z += x->pos.z;
	v->pos.w += x->pos.w;
	v->oneOverW += x->oneOverW;
	v->tc.u += x->tc.u;
	v->tc.v += x->tc.v;
	v->color.r += x->color.r;
	v->color.g += x->color.g;
	v->color.b += x->color.b;
}

void init_scanline(const triangle *tri, scanline *scanline, int y) {
	float width = tri->right.v.pos.x - tri->left.v.pos.x;
	scanline->x = (int)(tri->left.v.pos.x + 0.5f);
	scanline->y = y;
	scanline->w = (int)(tri->right.v.pos.x + 0.5f) - scanline->x;
	scanline->v = tri->left.v;
	if (tri->left.v.pos.x >= tri->right.v.pos.x) scanline->w = 0;
	vertex_division(&scanline->step, &tri->left.v, &tri->right.v, width);
}

int make_triangle(triangle *tri, const vertex *p1,
	 const vertex *p2,  const vertex *p3) {
	 const vertex *p;
	float a, b;

	if (p1->pos.y > p2->pos.y) p = p1, p1 = p2, p2 = p;
	if (p1->pos.y > p3->pos.y) p = p1, p1 = p3, p3 = p;
	if (p2->pos.y > p3->pos.y) p = p2, p2 = p3, p3 = p;
	//三点共线
	if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;
	if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;
	//平底三角形
	if (p1->pos.y == p2->pos.y) {
		if (p1->pos.x > p2->pos.x) 
		p = p1, p1 = p2, p2 = p;
		tri[0].top = p1->pos.y;
		tri[0].bottom = p3->pos.y;
		tri[0].left.v1 = *p1;
		tri[0].left.v2 = *p3;
		tri[0].right.v1 = *p2;
		tri[0].right.v2 = *p3;
		return (tri[0].top < tri[0].bottom) ? 1 : 0;
	}
	//平顶三角形
	if (p2->pos.y == p3->pos.y) {
		if (p2->pos.x > p3->pos.x) 
		p = p2, p2 = p3, p3 = p;
		tri[0].top = p1->pos.y;
		tri[0].bottom = p3->pos.y;
		tri[0].left.v1 = *p1;
		tri[0].left.v2 = *p2;
		tri[0].right.v1 = *p1;
		tri[0].right.v2 = *p3;
		return (tri[0].top < tri[0].bottom) ? 1 : 0;
	}
	//普通三角形
	tri[0].top = p1->pos.y;
	tri[0].bottom = p2->pos.y;
	tri[1].top = p2->pos.y;
	tri[1].bottom = p3->pos.y;
	//判断2号点的位置
	a = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
	b = p1->pos.x + (p2->pos.x - p1->pos.x) * a;
	//2号点在右面
	if (b >= p3->pos.x) {
		tri[0].left.v1 = *p1;
		tri[0].left.v2 = *p3;
		tri[0].right.v1 = *p1;
		tri[0].right.v2 = *p2;
		tri[1].left.v1 = *p1;
		tri[1].left.v2 = *p3;
		tri[1].right.v1 = *p2;
		tri[1].right.v2 = *p3;
	}
	else {//2在左面
		tri[0].left.v1 = *p1;
		tri[0].left.v2 = *p2;
		tri[0].right.v1 = *p1;
		tri[0].right.v2 = *p3;
		tri[1].left.v1 = *p2;
		tri[1].left.v2 = *p3;
		tri[1].right.v1 = *p1;
		tri[1].right.v2 = *p3;
	}

	return 2;
}

void get_Y(triangle *tri, float y) {
	float s1 = tri->left.v2.pos.y - tri->left.v1.pos.y;
	float s2 = tri->right.v2.pos.y - tri->right.v1.pos.y;

	float t1 = (y - tri->left.v1.pos.y) / s1;
	float t2 = (y - tri->right.v1.pos.y) / s2;

	vertex_interpolation(&tri->left.v, &tri->left.v1, &tri->left.v2, t1);
	vertex_interpolation(&tri->right.v, &tri->right.v1, &tri->right.v2, t2);
}


