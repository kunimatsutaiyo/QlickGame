#ifndef ENEMY_H_
#define ENEMY_H_

#include <d3dx9.h>

class Enemy
{
public:
	Enemy(D3DXVECTOR2 pos_, int survival_time_)
	{
		m_pos = pos_;
		m_survival_time = survival_time_ * 60;
	}
	~Enemy()
	{

	}

public:
	void Update();
	void Draw();

	int GetSurvivalTime() {
		return m_survival_time;
	}

private:
	D3DXVECTOR2 m_pos;
	int m_survival_time;

};

#endif