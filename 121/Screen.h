#pragma once

#include <windows.h>

extern int screen_w, screen_h, screen_exit;
extern int screen_mx, screen_my, screen_mb;
extern int screen_key[512];	// 当前键盘按下状态
extern HWND screen_handle;		// 主窗口 HWND
extern HDC screen_dc;			// 配套的 HDC
extern HBITMAP screen_hb;		// DIB
extern HBITMAP screen_ob;		// 老的 BITMAP
extern unsigned char *screen_fb;		// frame buffer
extern long screen_pitch;
// 屏幕初始化
int screen_init(int w, int h, const TCHAR *title);
// 关闭屏幕
int screen_close();
// 处理消息
void screen_dispatch();
// 显示 FrameBuffer
void screen_update();

static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);
