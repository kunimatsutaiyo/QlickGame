#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include "Enemy.h"

#include <vector>

class EnemyManager
{
public:
	EnemyManager(int interval_to_gererate_)
	{
		m_interval_to_generate = interval_to_gererate_ * 60;
		m_count_to_decrease = m_interval_to_generate;
	}
	~EnemyManager()
	{
		AllDelete();
	}

public:
	void Update();
	void Draw();

	void PartDelete(int delete_number_);

	std::vector<Enemy*> GetEnemy() {
		return mp_vec_enemy;
	}

private:
	bool CreateEnemy();

	void AllDelete();

private:
	std::vector<Enemy*> mp_vec_enemy;
	int m_interval_to_generate;			// ¶¬‚·‚éŠÔŠu
	int m_count_to_decrease;			// Œ¸‚ç‚·ƒJƒEƒ“ƒg

};

#endif