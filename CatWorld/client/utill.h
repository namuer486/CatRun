#pragma once
#include<graphics.h>
#include"camera.h"
#include"atlas.h"

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"MSIMG32.lib")

struct Rect
{
	int x, y;
	int w, h;
};

inline void flip_image(IMAGE* scr, IMAGE* dst)
{
	//图片左右镜像
	int w = scr->getwidth();
	int h = scr->getheight();
	Resize(dst, w, h);
	DWORD* scr_buffer = GetImageBuffer(scr);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (int y = 0; y < h; y++) {

		for (int x = 0; x < w; x++) {

			int idx_scr = w * y + x;
			int idx_dst = w * y + (w - x - 1);
			dst_buffer[idx_dst] = scr_buffer[idx_scr];
		}
	}
}

inline void putimage_ex(const Camera& camera,IMAGE* img, Rect* rect_dst, const Rect* rect_src = nullptr)
{
	//长条图片裁剪绘制
	const Vector2& camera_pos = camera.get_position();
	static BLENDFUNCTION blend_func = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	AlphaBlend(GetImageHDC(GetWorkingImage()), rect_dst->x - camera_pos.x, rect_dst->y - camera_pos.y, rect_dst->w, rect_dst->h,
		GetImageHDC(img), rect_src ? rect_src->x : 0, rect_src ? rect_src->y  : 0,
		rect_src ? rect_src->w : img->getwidth(), rect_src ? rect_src->h : img->getheight(), blend_func);
}

inline void putimage_alpha(const Camera& camera, int x, int y, IMAGE* img)
{
	//消除黑色底色的新绘制

	int w = img->getwidth();
	int h = img->getheight();
	const Vector2& pos_camera = camera.get_position();
	AlphaBlend(GetImageHDC(NULL), (int)(x - pos_camera.x), (int)(y - pos_camera.y), w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });

}

inline void putimage_alpha(int des_x, int des_y, int width, int height, IMAGE* img, int det_x, int det_y)
{
	//增加裁剪的新绘制

	int w = width > 0 ? width : img->getwidth();
	int h = height > 0 ? height : img->getheight();
	AlphaBlend(GetImageHDC(GetWorkingImage()), des_x, des_y, w, h,
		GetImageHDC(img), det_x, det_y, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });

}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2)
{
	//窗口坐标横线绘制
	const Vector2& pos_camera = camera.get_position();
	line((int)(x1 - pos_camera.x), (int)(y1 - pos_camera.y), (int)(x2 - pos_camera.x), (int)(y2 - pos_camera.y));
}

inline void sketch_image(IMAGE* src, IMAGE* dst)
{
	//图片虚化显示功能
	int x = src->getwidth();
	int h = src->getheight();
	Resize(dst, x, h);
	DWORD* src_buffer = GetImageBuffer(src);
	DWORD* dst_buffer = GetImageBuffer(dst);
	for (size_t i = 0; i < h; i++)
	{
		for (size_t j = 0; j < x; j++)
		{
			int idx = j + i * x;
			dst_buffer[idx] = BGR(RGB(255, 255, 255)) | (src_buffer[idx] & 0XFF000000);
		}
	}
}

static void flip_atlas(Atlas& scr, Atlas& dst) {
	//翻转图集图片
	dst.clear();
	for (size_t i = 0; i < scr.get_size(); i++) {
		IMAGE dst_image;
		flip_image(scr.get_image((int)i), &dst_image);
		dst.add_image(dst_image);
	}
}

inline void load_audio(LPCTSTR path, LPCTSTR id)
{
	static TCHAR str_char[512];
	_stprintf_s(str_char, _T("open %s alias %s"), path, id);
	mciSendString(str_char, NULL, 0, NULL);
}

inline void play_audio(LPCTSTR id, bool is_loop)
{
	static TCHAR str_char[512];
	_stprintf_s(str_char, _T("play %s %s from 0"), id, is_loop ? _T("repeat") : _T(""));
	mciSendString(str_char, NULL, 0, NULL);
}


inline void stop_audio(LPCTSTR id)
{
	static TCHAR str_char[512];
	_stprintf_s(str_char, _T("pause %s"), id);
	mciSendString(str_char, NULL, 0, NULL);
}

inline int range_random(int max, int min)
{
	return min + rand() % (max - min + 1);
}

