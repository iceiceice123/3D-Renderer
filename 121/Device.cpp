#include <stdio.h>
#include <stdlib.h>
#include"math.h"
#include"Geometry.h"
#include"Transform.h"
#include"Device.h"

void device_init(device_t *device, int width, int height, void *fb) {
	int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
	char *ptr = (char*)malloc(need + 64);
	char *framebuf, *zbuf;
	int k;
	device->framebuffer = (IUINT32**)ptr;
	device->zbuffer = (float**)(ptr + sizeof(void*) * height);//framebuffer所占空间为（指针大小*行数）因为共有行数个指针来保存每一列的数值
	ptr += sizeof(void*) * height * 2;//同上
	device->texture = (IUINT32**)ptr;//最大1024*1024纹理，共1024行
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width * height * 4;//外部framebuffer，每行4字节对齐
	ptr += width * height * 8;//外部zbuffer，同上
	if (fb != NULL) framebuf = (char*)fb;
	for (k = 0; k < height; k++) {
		device->framebuffer[k] = (IUINT32*)(framebuf + width * 4 * k);
		device->zbuffer[k] = (float*)(zbuf + width * 4 * k);
	}
	device->texture[0] = (IUINT32*)ptr;
	device->texture[1] = (IUINT32*)(ptr + 16);
	memset(device->texture[0], 0, 64);
	device->tex_width = 2;
	device->tex_height = 2;
	device->max_u = 1.0f;
	device->max_v = 1.0f;
	device->width = width;
	device->height = height;
	device->background = 0;
	device->line_color = 0;
	init_transform(&device->transform, width, height);
	device->render_state = WIREFRAME;
}

void device_destroy(device_t *device) {
	if (device->framebuffer)
		free(device->framebuffer);
	device->zbuffer = NULL;
	device->framebuffer = NULL;
	device->texture = NULL;
}

void device_texture(device_t *device, void *bits, long pitch, int w, int h) {
	char *ptr = (char*)bits;
	int j;
	for (j = 0; j < h; ptr += pitch, j++) 	// 重新计算每行纹理的指针
		device->texture[j] = (IUINT32*)ptr;
	device->tex_width = w;
	device->tex_height = h;
	device->max_u = (float)(w - 1);
	device->max_v = (float)(h - 1);
}

void device_clear(device_t *device) {
	int y, x, height = device->height;
	for (y = 0; y < device->height; y++) {
		IUINT32 *dst = device->framebuffer[y];
		for (x = device->width; x > 0; dst++, x--) 
			dst[0] = 0xffffff;
	}
	for (y = 0; y < device->height; y++) {
		float *dst = device->zbuffer[y];
		for (x = device->width; x > 0; dst++, x--) 
			dst[0] = 0.0f;
	}
}

void draw_point(device_t *device, int x, int y, IUINT32 color) {
	if (((IUINT32)x) < (IUINT32)device->width && ((IUINT32)y) < (IUINT32)device->height) {
		device->framebuffer[y][x] = color;
	}
}

void draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c) {
	int x, y, k = 0;
	if (x1 == x2 && y1 == y2) {
		draw_point(device, x1, y1, c);
	}
	else if (x1 == x2) {
		int inc = (y1 <= y2) ? 1 : -1;
		for (y = y1; y != y2; y += inc)
			draw_point(device, x1, y, c);
		draw_point(device, x2, y2, c);
	}
	else if (y1 == y2) {
		int inc = (x1 <= x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc)
			draw_point(device, x, y1, c);
		draw_point(device, x2, y2, c);
	}
	else {
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		if (dx >= dy) {
			if (x2 < x1)
				x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++) {
				draw_point(device, x, y, c);
				k += dy;
				if (k >= dx) {
					k -= dx;
					y += (y2 >= y1) ? 1 : -1;
					draw_point(device, x, y, c);
				}
			}
			draw_point(device, x2, y2, c);
		}
		else {
			if (y2 < y1)
				x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				draw_point(device, x, y, c);
				k += dx;
				if (k >= dy) {
					k -= dy;
					x += (x2 >= x1) ? 1 : -1;
					draw_point(device, x, y, c);
				}
			}
			draw_point(device, x2, y2, c);
		}
	}
}

IUINT32 device_texture_read(const device_t *device, float u, float v) {
	int x, y;
	u = u * device->max_u;
	v = v * device->max_v;
	x = check_range((int)(u + 0.5f), 0, device->tex_width - 1);
	y = check_range((int)(v + 0.5f), 0, device->tex_height - 1);
	return device->texture[y][x];
}

void device_draw_scanline(device_t *device, scanline *scanline) {
	IUINT32 *framebuffer = device->framebuffer[scanline->y];
	float *zbuffer = device->zbuffer[scanline->y];
	int x = scanline->x;
	int width = device->width;
	int render_state = device->render_state;
	for (int w = scanline->w; w > 0; x++, w--) {
		if (x >= 0 && x < width) {
			float oneOverW = scanline->v.oneOverW;
			if (oneOverW >= zbuffer[x]) {                             //可见性检测
				float w = 1.0f / oneOverW;
				zbuffer[x] = oneOverW;
				if (render_state == TEXTURE) {
					float u = scanline->v.tc.u * w;
					float v = scanline->v.tc.v * w;
					IUINT32 cc = device_texture_read(device, u, v);
					framebuffer[x] = cc;
				}
				if (render_state == COLOR) {
					float r = scanline->v.color.r * w;
					float g = scanline->v.color.g * w;
					float b = scanline->v.color.b * w;
					int R = (int)(r * 255.0f);
					int G = (int)(g * 255.0f);
					int B = (int)(b * 255.0f);
					R = check_range(R, 0, 255);
					G = check_range(G, 0, 255);
					B = check_range(B, 0, 255);
					framebuffer[x] = (R << 16) | (G << 8) | (B);
				}
			}
		}
		vertex_add(&scanline->v, &scanline->step);
		if (x >= width) break;
	}
}

void device_render_tri(device_t *device, triangle *tri) {
	scanline scanline;
	int j, top, bottom;
	top = (int)(tri->top + 0.5f);
	bottom = (int)(tri->bottom + 0.5f);
	for (j = top; j < bottom; j++) {
		if (j >= 0 && j < device->height) {
			get_Y(tri, (float)j + 0.5f);
			init_scanline(tri, &scanline, j);
			device_draw_scanline(device, &scanline);
		}
		if (j >= device->height) break;
	}
}

void device_draw_triangle(device_t *device, const vertex *v1,
	const vertex *v2, const vertex *v3) {
	point p1, p2, p3, c1, c2, c3;
	int render_state = device->render_state;

	// 按照 Transform 变化
	vector_project(&device->transform, &c1, &v1->pos);
	vector_project(&device->transform, &c2, &v2->pos);
	vector_project(&device->transform, &c3, &v3->pos);

	// 裁剪
	if (check_cvv(&c1) != 0) return;
	if (check_cvv(&c2) != 0) return;
	if (check_cvv(&c3) != 0) return;

	// 转换到屏幕坐标
	transform_screen(&device->transform, &p1, &c1);
	transform_screen(&device->transform, &p2, &c2);
	transform_screen(&device->transform, &p3, &c3);

	// 纹理或者色彩填充
	if (render_state == TEXTURE ||render_state== COLOR) {
		vertex t1 = *v1, t2 = *v2, t3 = *v3;
		triangle tris[2];
		int n;

		t1.pos = p1;
		t2.pos = p2;
		t3.pos = p3;

		t1.pos.w = c1.w;
		t2.pos.w = c2.w;
		t3.pos.w = c3.w;

		vertex_init(&t1);	
		vertex_init(&t2);	
		vertex_init(&t3);	

		n = make_triangle(tris, &t1, &t2, &t3);

		if (n >= 1) device_render_tri(device, &tris[0]);
		if (n >= 2) device_render_tri(device, &tris[1]);
	}

	if (render_state == WIREFRAME) {		// 线框绘制
		draw_line(device, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, device->line_color);
		draw_line(device, (int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, device->line_color);
		draw_line(device, (int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, device->line_color);
	}
}