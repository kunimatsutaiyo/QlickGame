#include <Windows.h>

#include "MyLibrary/Engine/Window.h"
#include "MyLibrary/Engine/Engine.h"
#include "MyLibrary/Engine/Input.h"
#include "MyLibrary/Engine/Graphics.h"
#include "MyLibrary/Engine/Texture.h"
#include "MyLibrary/Engine/Sound.h"

int APIENTRY WinMain(
	HINSTANCE	hInstance_,		// �C���X�^���X�n���h��
	HINSTANCE	hPrevInstance_,	// ���d�l(NULL����ɓ����Ă���̂Ŗ���)
	LPSTR		lpCmpLine_,		// �R�}���h���C��
	INT			nCmdShow_		// �\�����
)
{
	Window window;
	Engine engine;
	HWND window_handle = window.MakeWindow(hInstance_, 1920, 1080, "�|�`�|�`�Q�[��");
	engine.InitEngine(hInstance_, window_handle);

	// ���C�����[�v
	while (true)
	{
		// ���b�Z�[�W�\����
		MSG msg;

		// ���b�Z�[�W��M
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// WM_QUIT�ŏI��
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else {
				// ���b�Z�[�W�|��
				TranslateMessage(&msg);
				// �v���V�[�W���Ƀ��b�Z�[�W���M
				DispatchMessage(&msg);
			}
		}
		else {
			




		}
	}

	// �G���W���I��
	engine.EndEngine();

	//return msg.wParam;
	return 0;
}