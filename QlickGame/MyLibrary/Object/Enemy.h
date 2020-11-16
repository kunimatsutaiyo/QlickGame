#ifndef ENEMY_H_
#define ENEMY_H_

#include <d3dx9.h>

class Enemy
{
public:
	Enemy(D3DXVECTOR2 pos_, int survival_seconds_time_, D3DXVECTOR2 size_)
	{
		m_pos = pos_;
		m_survival_fps_time = survival_seconds_time_ * 60;
		m_size = size_;
	}
	~Enemy()
	{

	}

public:
	void Update();
	void Draw();

	D3DXVECTOR2 GetPos() {
		return m_pos;
	}
	D3DXVECTOR2 GetSize() {
		return m_size;
	}
	int GetSurvivalTime() {
		return m_survival_fps_time;
	}

private:
	D3DXVECTOR2 m_pos;
	int m_survival_fps_time;
	D3DXVECTOR2 m_size;

};

#endif