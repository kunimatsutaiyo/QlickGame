#include "MouseAndRect.h"

bool MouseAndRect::Judgment(D3DXVECTOR2 mouse_pos_, D3DXVECTOR2 rect_pos_, D3DXVECTOR2 rect_size_)
{
	float left = rect_pos_.x;
	float right = rect_pos_.x + rect_size_.x;
	float top = rect_pos_.y;
	float bottom = rect_pos_.y + rect_size_.y;

	if (left <= mouse_pos_.x &&		// マウスの位置が矩形の左側より大きい
		right >= mouse_pos_.x &&	// マウスの位置が矩形の右側より小さい
		top <= mouse_pos_.y &&		// マウスの位置が矩形の上側より大きい
		bottom >= mouse_pos_.y)		// マウスの位置が矩形の下側より小さい
	{
		return true;
	}

	return false;
}
