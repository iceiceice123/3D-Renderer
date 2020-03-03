#pragma once

#include <windows.h>

extern int screen_w, screen_h, screen_exit;
extern int screen_mx, screen_my, screen_mb;
extern int screen_key[512];	// ��ǰ���̰���״̬
extern HWND screen_handle;		// ������ HWND
extern HDC screen_dc;			// ���׵� HDC
extern HBITMAP screen_hb;		// DIB
extern HBITMAP screen_ob;		// �ϵ� BITMAP
extern unsigned char *screen_fb;		// frame buffer
extern long screen_pitch;
// ��Ļ��ʼ��
int screen_init(int w, int h, const TCHAR *title);
// �ر���Ļ
int screen_close();
// ������Ϣ
void screen_dispatch();
// ��ʾ FrameBuffer
void screen_update();

static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);
