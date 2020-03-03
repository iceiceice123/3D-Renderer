#pragma once
#include"math.h"
typedef struct {
	matrix world;         // 世界坐标变换矩阵
	matrix view;          // 摄影机坐标变换矩阵
	matrix projection;    // 投影变换矩阵
	matrix transform;     // transform =poi world * view * projection
	float w, h;           // 屏幕宽高
}	transform;



// 初始化，设置屏幕长宽
void init_transform(transform *ts, int width, int height);
// 将矢量 x 进行 project 
void vector_project(const transform *ts, vector *y, const vector *x);
// 矩阵更新
void update_transform(transform *ts);
// 检查齐次坐标同 cvv 的边界用于裁剪
int check_cvv(const vector *v);
// 得到屏幕坐标
void transform_screen(const transform *ts, vector *y, const vector *x);
// 转换到标准视体内 fovy视锥体的上下夹角 aspect纵横比，在视空间宽度除以高度。
void MatrixPerspectiveFovLH(matrix *m, float fovy, float aspect, float zn, float zf);