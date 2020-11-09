#ifndef WINDOW_H_
#define WINDOW_H_

#include <Windows.h>

#define WINDOW_CLASS_NAME "Window"
#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920

class Window {
public:
	/**
	* @brief �E�B���h�E���쐬����֐�
	* @param[in] hInstance	�C���X�^���X�n���h��
	* @param[in] width		�E�B���h�E�̉���
	* @param[in] height		�E�B���h�E�̏c��
	*/
	HWND MakeWindow(HINSTANCE hInstance_, int width_, int height_, const char* title_name_);

};

#endif