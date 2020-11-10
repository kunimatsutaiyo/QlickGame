#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include "Enemy.h"

#include <vector>

class EnemyManager
{
public:
	EnemyManager(int interval_to_gererate_, int number_of_generations_ = 100)
	{
		m_interval_to_generate = interval_to_gererate_ * 60;
		m_count_to_decrease = m_interval_to_generate;
	}
	~EnemyManager()
	{
		AllDelete();
	}

public:
	bool CreateEnemy();

	void Update();
	void Draw();

	void PartDelete(int delete_number_);
	void AllDelete();

private:
	std::vector<Enemy*> m_p_vec_enemy;
	int m_interval_to_generate;			// ¶¬‚·‚éŠÔŠu
	int m_count_to_decrease;			// Œ¸‚ç‚·ƒJƒEƒ“ƒg

};

#endif