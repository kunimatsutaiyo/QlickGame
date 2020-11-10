#include "MouseAndRect.h"

bool MouseAndRect::Judgment(D3DXVECTOR2 mouse_pos_, D3DXVECTOR2 rect_pos_, D3DXVECTOR2 rect_size_)
{
	float left = rect_pos_.x;
	float right = rect_pos_.x + rect_size_.x;
	float top = rect_pos_.y;
	float bottom = rect_pos_.y + rect_size_.y;

	if (left <= mouse_pos_.x &&		// �}�E�X�̈ʒu����`�̍������傫��
		right >= mouse_pos_.x &&	// �}�E�X�̈ʒu����`�̉E����菬����
		top <= mouse_pos_.y &&		// �}�E�X�̈ʒu����`�̏㑤���傫��
		bottom >= mouse_pos_.y)		// �}�E�X�̈ʒu����`�̉�����菬����
	{
		return true;
	}

	return false;
}
