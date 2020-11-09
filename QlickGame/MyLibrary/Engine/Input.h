#ifndef INPUT_H_
#define INPUT_H_

#include <Windows.h>
#include <dinput.h>
#include <d3dx9.h>

#include "../Utility/Singleton.h"
#include "../Utility/Vec.h"

// マウスのボタンの種類
enum MouseButton
{
	LEFT,
	RIGHT,
	CENTER,

	MAX_MOUSE_BUTTON
};

// キーの種類
enum KEY_TYPE {
	ESCAPE_KEY,
	ENTER_KEY,
	SPACE_KEY,
	LEFT_SHIFT_KEY,
	W_KEY,
	A_KEY,
	S_KEY,
	D_KEY,
	E_KEY,
	Q_KEY,

	L_KEY,	// デバッグ用：ボールレベルの変化

	MAX_KEY_INFO
};

enum INPUT_STATE {
	NOT_PUSH,		// 押されていない
	PUSH_DOWN,		// 押された瞬間
	PUSH,			// 押されている
	RELEASE			// 離した瞬間
};

class Input : public Singleton<Input> {
private:
	friend Singleton<Input>;

public:
	/**
	* @brief Input機能の初期化関数
	* デバイスの入力に必要な初期化を行います
	* InitEngineで実行されるので開発者が実行する必要はない
	* @reutrn 初期化結果、成功：ture
	* @param[in] instance_handle_ インスタンスハンドル
	* @param[in] window_handle_ ウィンドウハンドル
	*/
	bool Init(HINSTANCE instance_handle_, HWND window_hadle_);

	/**
	* @brief Input機能の終了関数
	* Input機能を終了させる
	* EndEngineで実行されるので開発者が実行する必要はない
	*/
	void Release();

	void Update();

#pragma region キーボード

	bool CreateKeyDevice(HWND window_handle_);

	/**
	* @grief キーボードの入力情報の更新関数
	* デバイス入力情報の更新を行う
	* 毎フレーム実行する必要がある
	*/
	void UpdateKeyState();

	/**
	* @brief キーが押されている状態の判定をする関数
	* 指定したキーが押されているかを判定する
	* @reutrn 判定結果、押されているなら：ture
	* @param[in] key_ 判定したキー
	*/
	bool GetKey(KEY_TYPE key_);

	/**
	* @brief キーが押された瞬間の判定関数
	* 指定したキーが押された瞬間を判定する
	* @reutrn 判定結果、成功：ture
	* @param[in] key_ 判定したキー
	*/
	bool GetKeyDown(KEY_TYPE key_);

	/**
	* @brief キーが離された瞬間の判定関数
	* 指定したキーが離された瞬間を判定する
	* @reutrn 判定結果、成功：ture
	* @param[in] key_ 判定したキー
	*/
	bool GetKeyUp(KEY_TYPE key_);

#pragma endregion

#pragma region マウス

	bool CreatMouseDevice();

	/**
	* @grief マウスの入力情報の更新関数
	* デバイス入力情報の更新を行う
	* 毎フレーム実行する必要がある
	*/
	void UpdateMouse();

	/**
	* @brief ボタンが押されているかの判定関数
	* @reutrn 判定結果、押されているなら：ture
	* @param[in] mouse_button_ 判定したいボタンの種類
	*/
	bool OnMousePush(MouseButton mouse_button_);

	/**
	* @brief ボタンが押された瞬間の判定関数
	* @reutrn 判定結果、押されているなら：ture
	* @param[in] mouse_button_ 判定したいボタンの種類
	*/
	bool OnMouseDown(MouseButton mouse_button_);

	/**
	* @brief ボタンを離した瞬間の判定関数
	* @reutrn 判定結果、押されているなら：ture
	* @param[in] mouse_button_ 判定したいボタンの種類
	*/
	bool OnMouseUp(MouseButton mouse_button_);

	D3DXVECTOR2 GetMousePos() {
		return m_mouse_pos;
	}

#pragma endregion

private:
	Input()
	{
		m_input_interface = nullptr;
		m_key_device = nullptr;
		for (int i = 0; i < KEY_TYPE::MAX_KEY_INFO; i++)
		{
			m_input_state[i] = INPUT_STATE::NOT_PUSH;
		}
		// 押したいやつとDirectInputのキーと合致させてやる
		m_key_info[KEY_TYPE::ESCAPE_KEY] = DIK_ESCAPE;
		m_key_info[KEY_TYPE::ENTER_KEY] = DIK_RETURN;
		m_key_info[KEY_TYPE::SPACE_KEY] = DIK_SPACE;
		m_key_info[KEY_TYPE::LEFT_SHIFT_KEY] = DIK_LSHIFT;
		m_key_info[KEY_TYPE::W_KEY] = DIK_W;
		m_key_info[KEY_TYPE::A_KEY] = DIK_A;
		m_key_info[KEY_TYPE::S_KEY] = DIK_S;
		m_key_info[KEY_TYPE::D_KEY] = DIK_D;
		m_key_info[KEY_TYPE::E_KEY] = DIK_E;
		m_key_info[KEY_TYPE::Q_KEY] = DIK_Q;
		m_key_info[KEY_TYPE::L_KEY] = DIK_L;

		m_mouse_device = nullptr;
		m_mouse_pos = { 0.0f,0.0f };
		m_mouse_move_vec = { 0.0f,0.0f };
	}
	~Input() {}

	// コピーコンストラクタの禁止
	Input(const Input&);
	Input& operator=(const Input&) = delete;

	// ムーブコンストラクタの禁止
	Input& operator=(const Input&&) = delete;

private:
	static Input* p_InputInstance;

	static LPDIRECTINPUT8 m_input_interface;	// インプットインターフェイス
	LPDIRECTINPUTDEVICE8 m_key_device;			// インプットデバイス(キーボード)
	LPDIRECTINPUTDEVICE8 m_mouse_device;		// インプットデバイス(マウス)

	DIMOUSESTATE m_prev_mouse_state;			// マウスの1フレーム前の入力情報
	DIMOUSESTATE m_current_mouses_state;		// マウスの現在の入力情報

	D3DXVECTOR2 m_mouse_pos;					// マウスの位置
	D3DXVECTOR2 m_mouse_move_vec;				// マウスの向いている方向

	INPUT_STATE m_input_state[KEY_TYPE::MAX_KEY_INFO];
	int m_key_info[KEY_TYPE::MAX_KEY_INFO];
};

#endif