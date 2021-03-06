#include "GameScene.h"
#include "../Engine/Texture.h"

#define INTERVAL_TIME 2
#define GAME_END_FPS 0

bool GameScene::Load()
{
	Texture::GetInstance()->LoadTexture("Res/free_enemy.jpg", "enemy");

	return false;
}

void GameScene::CreateEnemyManager()
{
	if (mp_enemy_manager == nullptr)
	{
		mp_enemy_manager = new EnemyManager(INTERVAL_TIME);
	}
}

void GameScene::CreateTimeLimit(int init_seconds_)
{
	if (mp_time_limit == nullptr)
	{
		mp_time_limit = new TimeLimit(init_seconds_);
	}
}

void GameScene::CreateScore(int init_score_)
{
	if (mp_score == nullptr)
	{
		mp_score = new Score(init_score_);
	}
}

void GameScene::Update()
{
	// game処理 //
	// 敵の更新
	mp_enemy_manager->Update();

	// 当たり判定
	Collision();

	// UI更新
	// time_limit
	mp_time_limit->Update();

	// 一時停止
	if (mp_time_limit->GetTime() <= GAME_END_FPS)
	{
		system("PAUSE");
	}
}

void GameScene::Draw()
{
	// draw処理 //
	// 描画開始
	if (mp_graphics->StartDraw() == true)
	{
		// 敵描画
		mp_enemy_manager->Draw();

		// UI描画
		// time_limit
		mp_time_limit->Draw();
		// score
		mp_score->Draw();
	}
	// 描画終了
	mp_graphics->EndDraw();
}

bool GameScene::Collision()
{
	if (mp_input->OnMouseDown(MouseButton::LEFT))
	{
		D3DXVECTOR2 mouse_pos = mp_input->GetMousePos();
		std::vector<Enemy*> vec_enemy = mp_enemy_manager->GetEnemy();

		for (size_t i = 0; i < vec_enemy.size(); i++)
		{
			if (mp_col_mouse_and_rect->Judgment(mouse_pos, vec_enemy[i]->GetPos(), vec_enemy[i]->GetSize()))
			{
				mp_enemy_manager->PartDelete(i);

				// ※変更箇所
				// score加算をここでするのではなく通知するに変更したい
				// Score加算
				mp_score->Add(1);

				return true;
			}
		}
	}

	return false;
}
