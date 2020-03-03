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
	int width;                  // 窗口宽度
	int height;                 // 窗口高度
	IUINT32 **framebuffer;      // 像素缓存：framebuffer[y] 代表第y行
	float **zbuffer;            // 深度缓存：zbuffer[y] 为第y行指针
	IUINT32 **texture;          // 纹理：同样是每行索引
	int tex_width;              // 纹理宽度
	int tex_height;             // 纹理高度
	float max_u;                // 纹理最大宽度
	float max_v;                // 纹理最大高度
	int render_state;           // 渲染状态
	IUINT32 background;         // 背景颜色
	IUINT32 line_color;         // 线的颜色
}	device_t;

#define WIREFRAME      1		// 渲染线框
#define TEXTURE        2		// 渲染纹理
#define COLOR          3		// 渲染颜色

// 设备初始化
void device_init(device_t *device, int width, int height, void *fb);
// 删除设备
void device_destroy(device_t *device);
// 设置当前纹理
void device_texture(device_t *device, void *bits, long pitch, int w, int h);
// 清空 framebuffer 和 zbuffer
void device_clear(device_t *device);
// 画点
void draw_point(device_t *device, int x, int y, IUINT32 color);
// 绘制线段
void draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c);
// 根读取纹理
IUINT32 device_texture_read(const device_t *device, float u, float v);
// 绘制扫描线
void device_draw_scanline(device_t *device, scanline *scanline);
// 主渲染函数
void device_render_tri(device_t *device, triangle *tri);
// 绘制三角形
void device_draw_triangle(device_t *device, const vertex *v1, const vertex *v2, const vertex *v3);
