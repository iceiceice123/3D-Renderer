#include <stdio.h>
#include <stdlib.h>
#include "Screen.h"
#include "Math.h"
#include "Transform.h"
#include "Geometry.h"
#include "Device.h"

typedef unsigned int IUINT32;
vertex example[8] = {
	{ { 1, -1,  1, 1 },{ 0, 0 },{ 1.0f, 0.2f, 0.2f }, 1 },
	{ { -1, -1,  1, 1 },{ 0, 1 },{ 0.2f, 1.0f, 0.2f }, 1 },
	{ { -1,  1,  1, 1 },{ 1, 1 },{ 0.2f, 0.2f, 1.0f }, 1 },
	{ { 1,  1,  1, 1 },{ 1, 0 },{ 1.0f, 0.2f, 1.0f }, 1 },
	{ { 1, -1, -1, 1 },{ 0, 0 },{ 1.0f, 1.0f, 0.2f }, 1 },
	{ { -1, -1, -1, 1 },{ 0, 1 },{ 0.2f, 1.0f, 1.0f }, 1 },
	{ { -1,  1, -1, 1 },{ 1, 1 },{ 1.0f, 0.3f, 0.3f }, 1 },
	{ { 1,  1, -1, 1 },{ 1, 0 },{ 0.2f, 1.0f, 0.3f }, 1 },
};

void draw_plane(device_t *device, int a, int b, int c, int d) {
	vertex p1 = example[a], p2 = example[b], p3 = example[c], p4 = example[d];
	p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
	p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
	device_draw_triangle(device, &p1, &p2, &p3);
	device_draw_triangle(device, &p3, &p4, &p1);
}

void draw_box(device_t *device, float theta) {
	matrix m;
	matrix_rotate(&m, -1, -0.5, 1, theta);
	device->transform.world = m;
	update_transform(&device->transform);
	draw_plane(device, 0, 1, 2, 3);
	draw_plane(device, 4, 5, 6, 7);
	draw_plane(device, 2, 6, 7, 3);
	draw_plane(device, 1, 5, 6, 2);
	draw_plane(device, 3, 7, 4, 0);
	draw_plane(device, 0, 4, 5, 1);
}

void camera_at_zero(device_t *device, float x, float y, float z) {
	point eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
	set_video(&device->transform.view, &eye, &at, &up);
	update_transform(&device->transform);
}

void init_texture(device_t *device) {
	static IUINT32 texture[1024][1024];
	int i, j;
	for (j = 0; j < 1024; j++) {
		for (i = 0; i < 1024; i++) {
			int x = i / 128, y = j / 128;
			texture[j][i] = ((x + y) % 2) ? 0x000000 : 0xff0000;
		}
	}
	device_texture(device, texture, 1024 * 4, 1024, 1024);
}

int main(void)
{
	device_t device;
	int states[] = { 0,TEXTURE, COLOR, WIREFRAME };
	int state = 0;
	int kbhit = 0;
	float alpha = 1;
	float pos = 3.5;

	TCHAR *title = _T("software render demo");


	if (screen_init(1000, 800, title))
		return -1;

	device_init(&device, 1000, 800, screen_fb);
	camera_at_zero(&device, 10, 0, 0);

	init_texture(&device);
	device.render_state = TEXTURE;

	while (screen_exit == 0 && screen_key[VK_ESCAPE] == 0) {
		screen_dispatch();
		device_clear(&device);
		camera_at_zero(&device, pos, 0, 0);

		if (screen_key[VK_UP]) pos -= 0.01f;
		if (screen_key[VK_DOWN]) pos += 0.01f;
		if (screen_key[VK_LEFT]) alpha += 0.01f;
		if (screen_key[VK_RIGHT]) alpha -= 0.01f;

		if (screen_key[VK_SPACE]) {
			if (kbhit == 0) {
				kbhit = 1;
				if (state > 3) state = 1;
				else state++;
				device.render_state = states[state];
			}
		}
		else {
			kbhit = 0;
		}

		draw_box(&device, alpha);
		screen_update();
		
	}
	return 0;
}

