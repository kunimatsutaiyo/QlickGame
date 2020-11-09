#ifndef CAMERA_H_
#define CAMERA_H_

#include <d3dx9.h>

class Camera {
public:
	Camera(D3DXVECTOR3 pos_)
	{
		m_pos = pos_;

		m_eye_pos.x = 0.0f;
		m_eye_pos.y = 0.0f;
		m_eye_pos.z = 1.0f;

		m_upvector.x = 0.0f;
		m_upvector.y = 1.0f;
		m_upvector.z = 0.0f;
	}
	~Camera() {}

public:
	void Update();
	void Move(D3DXVECTOR3 pos_);

private:
	D3DXVECTOR3 m_pos;			// カメラの位置
	D3DXVECTOR3 m_eye_pos;		// 注視点
	D3DXVECTOR3 m_upvector;		// カメラの上向きのベクトル
};

#endif