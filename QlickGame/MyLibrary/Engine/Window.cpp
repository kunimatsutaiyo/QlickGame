#include "Window.h"
#include "../Utility/Size.h"

// �E�B���h�E�v���V�[�W���̍쐬
LRESULT CALLBACK WindowProcedure(
	HWND	window_handle_,
	UINT	message_id_,
	WPARAM	wparam_,
	LPARAM	lparam_)
{
	// ���b�Z�[�W�ɑ΂���Ή�������
	switch (message_id_)
	{
		// �E�B���h�E�́u�~�v�{�^�����N���b�N���ꂽ��
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		// ���b�Z�[�W�̔��������Ȃ�
		return DefWindowProc(window_handle_, message_id_, wparam_, lparam_);
		break;
	}

	// ���b�Z�[�W�̔�����������0��Ԃ�
	return 0;
}

HWND Window::MakeWindow(HINSTANCE hInstance_, int width_, int height_, const char* title_name_)
{
	// �\���̂̓o�^
	HWND hWnd;
	// WNDCLASSEX�\���̂ɕK�v�ȏ���ݒ肷��
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEX),				// �\���̂̃T�C�Y
		CS_HREDRAW | CS_VREDRAW,		// �N���X�̃X�^�C��
		WindowProcedure,				// �E�B���h�E�v���V�[�W��
		0,								// �⏕������
		0,								// �⏕������
		hInstance_,						// ���̃A�v���̃C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	// �A�C�R���摜
		LoadCursor(NULL,IDC_ARROW),		// �J�[�\���摜
		NULL,							// �w�i�u���V(�w�i�F)
		NULL,							// ���j���[��
		TEXT(WINDOW_CLASS_NAME),		// �N���X��
		NULL							// �������A�C�R��
	};

	// WNDCLASSEX�\���̂̓o�^
	if (RegisterClassEx(&window_class) == 0)
	{
		return 0;
	}

	// �E�B���h�E�̍쐬
	hWnd = CreateWindow(
		TEXT(WINDOW_CLASS_NAME),// �g�p����E�B���h�E�N���X��(RegisterClass�œo�^�������O���w��)
		(LPCWSTR)title_name_,	// �����ŃE�B���h�E�̃^�C�g������ύX(����A�W�A�����A��)
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),	// �E�B���h�E�̎�ށAWS_THICKFRAME�̓T�C�Y�ύX�\
		CW_USEDEFAULT,			// �E�B���h�E�̏������W(x���W)�ACW_USEDEFAULT�̓V�X�e���C��
		0,						// �E�B���h�E�̏������W(y���W)�ACW_USEDEFAULT�̓V�X�e���C��
		width_,					// �E�B���h�E�̉���
		height_,				// �E�B���h�E�̏c��
		NULL,					// �e�E�B���h�E�n���h��(�Ȃ����NULL)
		NULL,					// ���j���[�n���h��(�Ȃ����NULL)
		hInstance_,				// ���̃E�B���h�E��ێ�����A�v���̃C���X�^���X�n���h��
		NULL					// NULL�ł���
	);

	if (hWnd == NULL)
	{
		return 0;
	}

	// �E�B���h�E�̃��T�C�Y
	RECT window_rect;
	RECT client_rect;

	// �E�B���h�E�T�C�Y�̎擾
	GetWindowRect(hWnd, &window_rect);
	// �N���C�A���g�̈�̃T�C�Y�擾
	GetClientRect(hWnd, &client_rect);

	// �t���[���T�C�Y�Z�o
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	// ���T�C�Y�p�T�C�Y�̎Z�o
	Size resize;
	resize.Width = frame_size_x + width_;
	resize.Height = frame_size_y + height_;

	// �\���ʒu�̍X�V
	SetWindowPos(
		hWnd,				// �X�V�ΏۂƂȂ�E�B���h�E�n���h��
		NULL,				// ���̃E�B���h�E������O�ɕ\������E�B���h�E�n���h��
		CW_USEDEFAULT,		// �E�B���h�E�̕\���ʒu(x��)
		CW_USEDEFAULT,		// �E�B���h�E�̕\���ʒu(y��)
		resize.Width,		// �E�B���h�E�̉���
		resize.Height,		// �E�B���h�E�̗���
		SWP_NOMOVE			// �E�B���h�E�T�C�Y�ƈʒu�̕ύX�Ɋւ���t���O���w��
	);

	// �E�B���h�E�\��
	ShowWindow(
		hWnd,		// �\���ݒ��ύX����E�B���h�E�n���h��
		SW_SHOW		// �\�����(SW_SHOW�̓E�B���h�E���A�N�e�B�u�ɂ��āA���݂̈ʒu�ƃT�C�Y�ŕ\������)
	);

	// yttm�ł͐G��Ă��Ȃ��������
	UpdateWindow(hWnd);

	return hWnd;
}