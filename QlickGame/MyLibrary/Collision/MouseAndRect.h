#ifndef MOUSE_AND_RECT_H_
#define MOUSE_AND_RECT_H_

#include <d3dx9.h>

class MouseAndRect
{
public:
	MouseAndRect() {}
	~MouseAndRect() {}

public:
	bool Judgment(D3DXVECTOR2 mouse_pos_, D3DXVECTOR2 rect_pos_, D3DXVECTOR2 rect_size_);

};

#endif