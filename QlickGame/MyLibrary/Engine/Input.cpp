#include "Input.h"
#include "Window.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define MOUSE_ON_VALUE (0x80)	// �I����Ԃ̒l

Input* Input::p_InputInstance = nullptr;
LPDIRECTINPUT8 Input::m_input_interface = nullptr;

bool Input::Init(HINSTANCE instance_handle_, HWND window_handle_)
{
	// IDirectInput8�C���^�[�t�F�[�X�쐬
	if (FAILED(DirectInput8Create(
		instance_handle_,			// �A�v���P�[�V�����̃C���X�^���X�n���h��
		DIRECTINPUT_VERSION,		// �g�p����DirectInput�̃o�[�W����(DIRECTINPUT_VERSION�Œ�ł���)
		IID_IDirectInput8,			// �ړI�Ƃ���C���^�[�t�F�[�X�̎��ʎq(IID_IDirectInput8�Œ�ł���)
		(void**)&m_input_interface,	// IDirectInput8�C���^�[�t�F�[�X�|�C���^�󂯎��p
		NULL						// ��{nullptr�ł���
	)))
	{
		// ���s�������Ƃ�ʒm����
		//MessageBoxA(window_handle_, (char*)"error", (char*)"�G���[A", MB_OK);
		return false;
	}

	// �L�[�f�o�C�X�쐬
	if (CreateKeyDevice(window_handle_) == false)
	{
		Release();
		return false;
	}

	// �}�E�X�f�o�C�X�쐬
	if (CreatMouseDevice() == false)
	{
		Release();
		return false;
	}

	// ���͏��̏�����
	ZeroMemory(&m_current_mouses_state, sizeof(DIMOUSESTATE));
	ZeroMemory(&m_prev_mouse_state, sizeof(DIMOUSESTATE));

	return true;
}

void Input::Release()
{
	// �L�[�f�o�C�X���
	if (m_key_device != nullptr)
	{
		m_key_device->Unacquire();
		m_key_device->Release();
		m_key_device = nullptr;
	}

	// �}�E�X�f�o�C�X���
	if (m_mouse_device != nullptr)
	{
		m_mouse_device->Unacquire();
		m_mouse_device->Release();
		m_mouse_device = nullptr;
	}

	// �C���^�[�t�F�[�X���
	if (m_input_interface != nullptr)
	{
		m_input_interface->Release();
		m_input_interface = nullptr;
	}
}

void Input::Update()
{
	UpdateKeyState();
	UpdateMouse();
}

#pragma region �L�[�{�[�h

bool Input::CreateKeyDevice(HWND window_handle_)
{
	// IDirectInputDevice8�̍쐬
	// IDirectInputDevice8�C���^�[�t�F�C�X�̎擾
	if (FAILED(m_input_interface->CreateDevice(
		GUID_SysKeyboard,	// �g�p����f�o�C�X�̎w��
		&m_key_device,		// IDirectInputDevice8�C���^�[�t�F�[�X�󂯎��p
		NULL				// ��{nullptr�ł���
	)))
	{
		// �f�o�C�X�쐬�Ɏ��s
		return false;
	}

	// �f�o�C�X�̃t�H�[�}�b�g�̐ݒ�
	/*
		SetDataFormat:
			�������F	�ݒ肷����̓f�o�C�X�̃t�H�[�}�b�g�f�o�C�X��
						directInput���ŗp�ӂ���Ă���

			�L�[�{�[�h�F		c_dfDIKeyboard
			�}�E�X�F			c_dfDIMouse
			�W���C�X�e�B�b�N�F	c_dfDIJoystick
	*/
	if (FAILED(m_key_device->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

	// �������[�h�̐ݒ�
	/*
		�t�H�A�O���E���h���[�h�F
			�E�B���h�E���o�b�N�O���E���h�Ɉړ�������(��A�N�e�B�u�ɂȂ�)
			�f�o�C�X�̎擾���ł��Ȃ�

		�o�b�N�ド�E���h���[�h�F
			�E�B���h�E����A�N�e�B�u��Ԃł��f�o�C�X���擾�ł���


		�r���I�F
			���̃A�v���P�[�V�����͂��̓��͂��擾�ł��Ȃ�

		��r���I�F
			���̃A�v���P�[�V�����ł����̂܂ܓ��̓f�o�C�X�̎擾���ł���
	*/
	if (FAILED(m_key_device->SetCooperativeLevel(
		window_handle_,							// ���̓f�o�C�X���֘A�t�����Ă���E�B���h�E�n���h��
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE	// �������x���̐ݒ�t���O))
	)))
	{
		return false;
	}

	// �f�o�C�X�̎擾�J�n
	m_key_device->Acquire();

	return true;
}

void Input::UpdateKeyState()
{
	BYTE key[256];

	HRESULT hr = m_key_device->GetDeviceState(256, key);

	if (FAILED(hr))
	{
		return;
	}

	for (int i = 0; i < KEY_TYPE::MAX_KEY_INFO; i++)
	{
		if (key[m_key_info[i]] & 0x80)
		{
			if (m_input_state[i] == INPUT_STATE::NOT_PUSH ||
				m_input_state[i] == INPUT_STATE::RELEASE)
			{
				m_input_state[i] = INPUT_STATE::PUSH_DOWN;
			}
			else {
				m_input_state[i] = INPUT_STATE::PUSH;
			}
		}
		else {
			if (m_input_state[i] == INPUT_STATE::PUSH ||
				m_input_state[i] == INPUT_STATE::PUSH_DOWN)
			{
				m_input_state[i] = INPUT_STATE::RELEASE;
			}
			else {
				m_input_state[i] = INPUT_STATE::NOT_PUSH;
			}
		}
	}
}

bool Input::GetKey(KEY_TYPE key_)
{
	return (m_input_state[key_] == INPUT_STATE::PUSH);
}

bool Input::GetKeyDown(KEY_TYPE key_)
{
	return (m_input_state[key_] == INPUT_STATE::PUSH_DOWN);
}

bool Input::GetKeyUp(KEY_TYPE key_)
{
	return (m_input_state[key_] == INPUT_STATE::RELEASE);
}

#pragma endregion

#pragma region �}�E�X

bool Input::CreatMouseDevice()
{
	// �}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	if (FAILED(m_input_interface->CreateDevice(
		GUID_SysMouse,
		&m_mouse_device,
		NULL
	)))
	{
		return false;
	}

	// ���̓t�H�[�}�b�g�̎w��
	if (FAILED(m_mouse_device->SetDataFormat(&c_dfDIMouse)))
	{
		// �f�[�^�t�H�[�}�b�g�Ɏ��s
		return false;
	}

	// ���[�h��ݒ�(�t�H�A�O���E���h&��r�����[�h)
	if (FAILED(m_mouse_device->SetCooperativeLevel(
		FindWindowA(WINDOW_CLASS_NAME, nullptr),
		//DISCL_BACKGROUND | DISCL_NONEXCLUSIVE
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
	)))
	{
		// ���[�h�̐ݒ�Ɏ��s
		return false;
	}

	//// �f�o�C�X�̐ݒ�
	//DIPROPDWORD diprop;
	//diprop.diph.dwSize = sizeof(diprop);
	//diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	//diprop.diph.dwObj = 0;
	//diprop.diph.dwHow = DIPH_DEVICE;
	//diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�(��Βl��DIPROPAXISMODE_ABS)

	//if (FAILED(m_mouse_device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	//{
	//	// �f�o�C�X�̐ݒ�Ɏ��s
	//	return false;
	//}

	// ���͐���J�n
	m_mouse_device->Acquire();

	// �|�[�����O����
	DIDEVCAPS cap;
	m_mouse_device->GetCapabilities(&cap);
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// �|�[�����O�J�n
		if (FAILED(m_mouse_device->Poll()))
		{
			return false;
		}
	}

	return true;
}

void Input::UpdateMouse()
{
	// �X�V�O�ɍŐV�̃}�E�X�̏���ۑ�����
	m_prev_mouse_state = m_current_mouses_state;

	// �}�E�X�̏�Ԃ��擾���܂�
	if (FAILED(m_mouse_device->GetDeviceState(
		sizeof(DIMOUSESTATE),
		&m_current_mouses_state
	)))
	{
		m_mouse_device->Acquire();
		m_mouse_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_current_mouses_state);
	}

	D3DXVECTOR2 prev = m_mouse_pos;
	POINT p;
	// �}�E�X���W(�X�N���[�����W)���擾����
	GetCursorPos(&p);

	// �X�N���[�����W�ɃN���C�A���g���W�ɕϊ�����
	ScreenToClient(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	// �ϊ��������W��ۑ�
	m_mouse_pos.x = (float)p.x;
	m_mouse_pos.y = (float)p.y;
}

bool Input::OnMousePush(MouseButton mouse_button_)
{
	if (m_prev_mouse_state.rgbButtons[mouse_button_] & MOUSE_ON_VALUE &&
		m_current_mouses_state.rgbButtons[mouse_button_] & MOUSE_ON_VALUE)
	{
		return true;
	}
	return false;
}

bool Input::OnMouseDown(MouseButton mouse_button_)
{
	if (!(m_prev_mouse_state.rgbButtons[mouse_button_] & MOUSE_ON_VALUE) &&
		m_current_mouses_state.rgbButtons[mouse_button_] & MOUSE_ON_VALUE)
	{
		return true;
	}
	return false;
}

bool Input::OnMouseUp(MouseButton mouse_button_)
{
	if (m_prev_mouse_state.rgbButtons[mouse_button_] & MOUSE_ON_VALUE &&
		!(m_current_mouses_state.rgbButtons[mouse_button_] & MOUSE_ON_VALUE))
	{
		return true;
	}
	return false;
}

#pragma endregion