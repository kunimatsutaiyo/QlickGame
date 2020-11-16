#ifndef COLLISION_DETECTION_OF_MOUSE_AND_RECT_H_
#define COLLISION_DETECTION_OF_MOUSE_AND_RECT_H_

#include <d3dx9.h>

class CollisionDetectionOfMouseAndRect
{
public:
	CollisionDetectionOfMouseAndRect() {}
	~CollisionDetectionOfMouseAndRect() {}

public:
	static bool Judgment(D3DXVECTOR2 mouse_pos_, D3DXVECTOR2 rect_pos_, D3DXVECTOR2 rect_size_);

};

#endif