#include "Input.h"
#include "Window.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

#define MOUSE_ON_VALUE (0x80)	// オン状態の値

Input* Input::p_InputInstance = nullptr;
LPDIRECTINPUT8 Input::m_input_interface = nullptr;

bool Input::Init(HINSTANCE instance_handle_, HWND window_handle_)
{
	// IDirectInput8インターフェース作成
	if (FAILED(DirectInput8Create(
		instance_handle_,			// アプリケーションのインスタンスハンドル
		DIRECTINPUT_VERSION,		// 使用するDirectInputのバージョン(DIRECTINPUT_VERSION固定でいい)
		IID_IDirectInput8,			// 目的とするインターフェースの識別子(IID_IDirectInput8固定でいい)
		(void**)&m_input_interface,	// IDirectInput8インターフェースポインタ受け取り用
		NULL						// 基本nullptrでいい
	)))
	{
		// 失敗したことを通知する
		//MessageBoxA(window_handle_, (char*)"error", (char*)"エラーA", MB_OK);
		return false;
	}

	// キーデバイス作成
	if (CreateKeyDevice(window_handle_) == false)
	{
		Release();
		return false;
	}

	// マウスデバイス作成
	if (CreatMouseDevice() == false)
	{
		Release();
		return false;
	}

	// 入力情報の初期化
	ZeroMemory(&m_current_mouses_state, sizeof(DIMOUSESTATE));
	ZeroMemory(&m_prev_mouse_state, sizeof(DIMOUSESTATE));

	return true;
}

void Input::Release()
{
	// キーデバイス解放
	if (m_key_device != nullptr)
	{
		m_key_device->Unacquire();
		m_key_device->Release();
		m_key_device = nullptr;
	}

	// マウスデバイス解放
	if (m_mouse_device != nullptr)
	{
		m_mouse_device->Unacquire();
		m_mouse_device->Release();
		m_mouse_device = nullptr;
	}

	// インターフェース解放
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

#pragma region キーボード

bool Input::CreateKeyDevice(HWND window_handle_)
{
	// IDirectInputDevice8の作成
	// IDirectInputDevice8インターフェイスの取得
	if (FAILED(m_input_interface->CreateDevice(
		GUID_SysKeyboard,	// 使用するデバイスの指定
		&m_key_device,		// IDirectInputDevice8インターフェース受け取り用
		NULL				// 基本nullptrでいい
	)))
	{
		// デバイス作成に失敗
		return false;
	}

	// デバイスのフォーマットの設定
	/*
		SetDataFormat:
			第一引数：	設定する入力デバイスのフォーマットデバイスは
						directInput側で用意されている

			キーボード：		c_dfDIKeyboard
			マウス：			c_dfDIMouse
			ジョイスティック：	c_dfDIJoystick
	*/
	if (FAILED(m_key_device->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

	// 協調モードの設定
	/*
		フォアグラウンドモード：
			ウィンドウがバックグラウンドに移動したら(非アクティブになる)
			デバイスの取得ができない

		バック九ラウンドモード：
			ウィンドウが非アクティブ状態でもデバイスが取得できる


		排他的：
			他のアプリケーションはその入力を取得できない

		非排他的：
			他のアプリケーションでもそのまま入力デバイスの取得ができる
	*/
	if (FAILED(m_key_device->SetCooperativeLevel(
		window_handle_,							// 入力デバイスが関連付けられているウィンドウハンドル
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE	// 協調レベルの設定フラグ))
	)))
	{
		return false;
	}

	// デバイスの取得開始
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

#pragma region マウス

bool Input::CreatMouseDevice()
{
	// マウス用にデバイスオブジェクトを作成
	if (FAILED(m_input_interface->CreateDevice(
		GUID_SysMouse,
		&m_mouse_device,
		NULL
	)))
	{
		return false;
	}

	// 入力フォーマットの指定
	if (FAILED(m_mouse_device->SetDataFormat(&c_dfDIMouse)))
	{
		// データフォーマットに失敗
		return false;
	}

	// モードを設定(フォアグラウンド&非排他モード)
	if (FAILED(m_mouse_device->SetCooperativeLevel(
		FindWindowA(WINDOW_CLASS_NAME, nullptr),
		//DISCL_BACKGROUND | DISCL_NONEXCLUSIVE
		DISCL_NONEXCLUSIVE | DISCL_FOREGROUND
	)))
	{
		// モードの設定に失敗
		return false;
	}

	//// デバイスの設定
	//DIPROPDWORD diprop;
	//diprop.diph.dwSize = sizeof(diprop);
	//diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	//diprop.diph.dwObj = 0;
	//diprop.diph.dwHow = DIPH_DEVICE;
	//diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定(絶対値はDIPROPAXISMODE_ABS)

	//if (FAILED(m_mouse_device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	//{
	//	// デバイスの設定に失敗
	//	return false;
	//}

	// 入力制御開始
	m_mouse_device->Acquire();

	// ポーリング判定
	DIDEVCAPS cap;
	m_mouse_device->GetCapabilities(&cap);
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();
		// ポーリング開始
		if (FAILED(m_mouse_device->Poll()))
		{
			return false;
		}
	}

	return true;
}

void Input::UpdateMouse()
{
	// 更新前に最新のマウスの情報を保存する
	m_prev_mouse_state = m_current_mouses_state;

	// マウスの状態を取得します
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
	// マウス座標(スクリーン座標)を取得する
	GetCursorPos(&p);

	// スクリーン座標にクライアント座標に変換する
	ScreenToClient(FindWindowA(WINDOW_CLASS_NAME, nullptr), &p);

	// 変換した座標を保存
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