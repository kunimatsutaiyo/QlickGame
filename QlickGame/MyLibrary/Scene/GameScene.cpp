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
	// gameˆ— //
	// “G‚ÌXV
	mp_enemy_manager->Update();

	// “–‚½‚è”»’è
	Collision();

	// UIXV
	// time_limit
	mp_time_limit->Update();

	// ˆêŽž’âŽ~
	if (mp_time_limit->GetTime() <= GAME_END_FPS)
	{
		system("PAUSE");
	}
}

void GameScene::Draw()
{
	// drawˆ— //
	// •`‰æŠJŽn
	if (mp_graphics->StartDraw() == true)
	{
		// “G•`‰æ
		mp_enemy_manager->Draw();

		// UI•`‰æ
		// time_limit
		mp_time_limit->Draw();
		// score
		mp_score->Draw();
	}
	// •`‰æI—¹
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

				// ¦•ÏX‰ÓŠ
				// score‰ÁŽZ‚ð‚±‚±‚Å‚·‚é‚Ì‚Å‚Í‚È‚­’Ê’m‚·‚é‚É•ÏX‚µ‚½‚¢
				// Score‰ÁŽZ
				mp_score->AddScore(1);

				return true;
			}
		}
	}

	return false;
}
