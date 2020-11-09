#ifndef WINDOW_H_
#define WINDOW_H_

#include <Windows.h>

#define WINDOW_CLASS_NAME "Window"
#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

class Window {
public:
	/**
	* @brief ウィンドウを作成する関数
	* @param[in] hInstance	インスタンスハンドル
	* @param[in] width		ウィンドウの横幅
	* @param[in] height		ウィンドウの縦幅
	*/
	HWND MakeWindow(HINSTANCE hInstance_, int width_, int height_, const char* title_name_);

};

#endif