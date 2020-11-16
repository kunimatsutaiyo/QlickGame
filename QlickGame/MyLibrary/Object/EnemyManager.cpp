#include "EnemyManager.h"
#include "../Engine/Window.h"

#include <time.h>

#define SURVIVAL_SECOND_TIME 5
#define ENEMY_WIDTH 60
#define ENEMY_HEIGHT 80

bool EnemyManager::CreateEnemy()
{
	// ※修正箇所
	// ランダム座標がおかしい
	srand((unsigned)time(NULL));
	D3DXVECTOR2 rand_pos = {
		static_cast<float>(rand() % WINDOW_WIDTH),
		static_cast<float>(rand() % WINDOW_HEIGHT)
	};

	mp_vec_enemy.push_back(new Enemy(rand_pos, SURVIVAL_SECOND_TIME, { ENEMY_WIDTH,ENEMY_HEIGHT }));

	return true;
}

void EnemyManager::Update()
{
	// 生成
	m_count_to_decrease--;
	if (m_count_to_decrease <= 0)
	{
		CreateEnemy();
		m_count_to_decrease = m_interval_to_generate;
	}

	// 更新
	for (size_t i = 0; i < mp_vec_enemy.size(); i++)
	{
		if (mp_vec_enemy[i] != nullptr)
		{
			mp_vec_enemy[i]->Update();
		}
	}

	// 削除
	for (size_t i = 0; i < mp_vec_enemy.size(); i++)
	{
		if (mp_vec_enemy[i] != nullptr)
		{
			if (mp_vec_enemy[i]->GetSurvivalTime() <= 0)
			{
				PartDelete(i);
			}
		}
	}
}

void EnemyManager::Draw()
{
	for (size_t i = 0; i < mp_vec_enemy.size(); i++)
	{
		if (mp_vec_enemy[i] != nullptr)
		{
			mp_vec_enemy[i]->Draw();
		}
	}
}

void EnemyManager::PartDelete(int delete_number_)
{
	if (mp_vec_enemy[delete_number_] != nullptr)
	{
		delete mp_vec_enemy[delete_number_];
		mp_vec_enemy[delete_number_] = nullptr;

		auto itr = mp_vec_enemy.begin() + delete_number_;
		mp_vec_enemy.erase(itr);
	}
}

void EnemyManager::AllDelete()
{
	for (size_t i = 0; i < mp_vec_enemy.size(); i++)
	{
		if (mp_vec_enemy[i] != nullptr)
		{
			delete mp_vec_enemy[i];
			mp_vec_enemy[i] = nullptr;
		}
	}
}
