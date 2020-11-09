#ifndef VEC_H_
#define VEC_H_

struct Vec2
{
	Vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vec2(float x_, float y_)
	{
		x = x_;
		y = y_;
	}

	float x;
	float y;
};

#endif