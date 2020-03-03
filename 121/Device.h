#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <tchar.h>
#include "Transform.h"
#include "Geometry.h"
typedef unsigned int IUINT32;
typedef struct {
	transform transform;
	int width;                  // ���ڿ��
	int height;                 // ���ڸ߶�
	IUINT32 **framebuffer;      // ���ػ��棺framebuffer[y] �����y��
	float **zbuffer;            // ��Ȼ��棺zbuffer[y] Ϊ��y��ָ��
	IUINT32 **texture;          // ����ͬ����ÿ������
	int tex_width;              // ������
	int tex_height;             // ����߶�
	float max_u;                // ���������
	float max_v;                // �������߶�
	int render_state;           // ��Ⱦ״̬
	IUINT32 background;         // ������ɫ
	IUINT32 line_color;         // �ߵ���ɫ
}	device_t;

#define WIREFRAME      1		// ��Ⱦ�߿�
#define TEXTURE        2		// ��Ⱦ����
#define COLOR          3		// ��Ⱦ��ɫ

// �豸��ʼ��
void device_init(device_t *device, int width, int height, void *fb);
// ɾ���豸
void device_destroy(device_t *device);
// ���õ�ǰ����
void device_texture(device_t *device, void *bits, long pitch, int w, int h);
// ��� framebuffer �� zbuffer
void device_clear(device_t *device);
// ����
void draw_point(device_t *device, int x, int y, IUINT32 color);
// �����߶�
void draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c);
// ����ȡ����
IUINT32 device_texture_read(const device_t *device, float u, float v);
// ����ɨ����
void device_draw_scanline(device_t *device, scanline *scanline);
// ����Ⱦ����
void device_render_tri(device_t *device, triangle *tri);
// ����������
void device_draw_triangle(device_t *device, const vertex *v1, const vertex *v2, const vertex *v3);
