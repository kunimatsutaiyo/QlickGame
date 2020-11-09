#ifndef ENGINE_H_
#define ENGINE_H_

#include <Windows.h>

class Engine {
public:
	/**
	* @brief �G���W���������֐�
	* �Q�[���Ŏg���G���W������(�`��A����)�̏�����
	* �����̊֐��̓Q�[�����[�v�J�n�O��1�x�������s����
	* @return ����������(true����)
	*/
	bool InitEngine(HINSTANCE hInstance_, HWND window_handle_);

	/**
	* @brief �G���W���I���֐�
	* �Q�[���I����ɃG���W�������̏I�����s��
	* �����̊֐��̓Q�[�����[�v�I�����1�x�������s����
	*/
	void EndEngine();
};

#endif