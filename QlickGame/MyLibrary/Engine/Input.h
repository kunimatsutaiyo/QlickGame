#ifndef INPUT_H_
#define INPUT_H_

#include <Windows.h>
#include <dinput.h>
#include <d3dx9.h>

#include "../Utility/Singleton.h"
#include "../Utility/Vec.h"

// �}�E�X�̃{�^���̎��
enum MouseButton
{
	LEFT,
	RIGHT,
	CENTER,

	MAX_MOUSE_BUTTON
};

// �L�[�̎��
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

	L_KEY,	// �f�o�b�O�p�F�{�[�����x���̕ω�

	MAX_KEY_INFO
};

enum INPUT_STATE {
	NOT_PUSH,		// ������Ă��Ȃ�
	PUSH_DOWN,		// �����ꂽ�u��
	PUSH,			// ������Ă���
	RELEASE			// �������u��
};

class Input : public Singleton<Input> {
private:
	friend Singleton<Input>;

public:
	/**
	* @brief Input�@�\�̏������֐�
	* �f�o�C�X�̓��͂ɕK�v�ȏ��������s���܂�
	* InitEngine�Ŏ��s�����̂ŊJ���҂����s����K�v�͂Ȃ�
	* @reutrn ���������ʁA�����Fture
	* @param[in] instance_handle_ �C���X�^���X�n���h��
	* @param[in] window_handle_ �E�B���h�E�n���h��
	*/
	bool Init(HINSTANCE instance_handle_, HWND window_hadle_);

	/**
	* @brief Input�@�\�̏I���֐�
	* Input�@�\���I��������
	* EndEngine�Ŏ��s�����̂ŊJ���҂����s����K�v�͂Ȃ�
	*/
	void Release();

	void Update();

#pragma region �L�[�{�[�h

	bool CreateKeyDevice(HWND window_handle_);

	/**
	* @grief �L�[�{�[�h�̓��͏��̍X�V�֐�
	* �f�o�C�X���͏��̍X�V���s��
	* ���t���[�����s����K�v������
	*/
	void UpdateKeyState();

	/**
	* @brief �L�[��������Ă����Ԃ̔��������֐�
	* �w�肵���L�[��������Ă��邩�𔻒肷��
	* @reutrn ���茋�ʁA������Ă���Ȃ�Fture
	* @param[in] key_ ���肵���L�[
	*/
	bool GetKey(KEY_TYPE key_);

	/**
	* @brief �L�[�������ꂽ�u�Ԃ̔���֐�
	* �w�肵���L�[�������ꂽ�u�Ԃ𔻒肷��
	* @reutrn ���茋�ʁA�����Fture
	* @param[in] key_ ���肵���L�[
	*/
	bool GetKeyDown(KEY_TYPE key_);

	/**
	* @brief �L�[�������ꂽ�u�Ԃ̔���֐�
	* �w�肵���L�[�������ꂽ�u�Ԃ𔻒肷��
	* @reutrn ���茋�ʁA�����Fture
	* @param[in] key_ ���肵���L�[
	*/
	bool GetKeyUp(KEY_TYPE key_);

#pragma endregion

#pragma region �}�E�X

	bool CreatMouseDevice();

	/**
	* @grief �}�E�X�̓��͏��̍X�V�֐�
	* �f�o�C�X���͏��̍X�V���s��
	* ���t���[�����s����K�v������
	*/
	void UpdateMouse();

	/**
	* @brief �{�^����������Ă��邩�̔���֐�
	* @reutrn ���茋�ʁA������Ă���Ȃ�Fture
	* @param[in] mouse_button_ ���肵�����{�^���̎��
	*/
	bool OnMousePush(MouseButton mouse_button_);

	/**
	* @brief �{�^���������ꂽ�u�Ԃ̔���֐�
	* @reutrn ���茋�ʁA������Ă���Ȃ�Fture
	* @param[in] mouse_button_ ���肵�����{�^���̎��
	*/
	bool OnMouseDown(MouseButton mouse_button_);

	/**
	* @brief �{�^���𗣂����u�Ԃ̔���֐�
	* @reutrn ���茋�ʁA������Ă���Ȃ�Fture
	* @param[in] mouse_button_ ���肵�����{�^���̎��
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
		// �����������DirectInput�̃L�[�ƍ��v�����Ă��
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

	// �R�s�[�R���X�g���N�^�̋֎~
	Input(const Input&);
	Input& operator=(const Input&) = delete;

	// ���[�u�R���X�g���N�^�̋֎~
	Input& operator=(const Input&&) = delete;

private:
	static Input* p_InputInstance;

	static LPDIRECTINPUT8 m_input_interface;	// �C���v�b�g�C���^�[�t�F�C�X
	LPDIRECTINPUTDEVICE8 m_key_device;			// �C���v�b�g�f�o�C�X(�L�[�{�[�h)
	LPDIRECTINPUTDEVICE8 m_mouse_device;		// �C���v�b�g�f�o�C�X(�}�E�X)

	DIMOUSESTATE m_prev_mouse_state;			// �}�E�X��1�t���[���O�̓��͏��
	DIMOUSESTATE m_current_mouses_state;		// �}�E�X�̌��݂̓��͏��

	D3DXVECTOR2 m_mouse_pos;					// �}�E�X�̈ʒu
	D3DXVECTOR2 m_mouse_move_vec;				// �}�E�X�̌����Ă������

	INPUT_STATE m_input_state[KEY_TYPE::MAX_KEY_INFO];
	int m_key_info[KEY_TYPE::MAX_KEY_INFO];
};

#endif