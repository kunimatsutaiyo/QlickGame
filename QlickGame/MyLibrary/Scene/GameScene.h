#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "../Engine/Input.h"
#include "../Engine/Graphics.h"
#include "../Object/EnemyManager.h"
#include "../Collision/CollisionDetectionOfMouseAndRect.h"
#include "../UI/TimeLimit.h"
#include "../UI/Score.h"

class GameScene
{
public:
	GameScene()
	{
		mp_input = Input::GetInstance();
		mp_graphics = Graphics::GetInstance();

		mp_col_mouse_and_rect = new CollisionDetectionOfMouseAndRect();

		mp_enemy_manager = nullptr;

		mp_time_limit = nullptr;
		mp_score = nullptr;
	}
	~GameScene()
	{

	}

public:
	bool Load();

	void CreateEnemyManager();
	void CreateTimeLimit(int init_seconds_);
	void CreateScore(int init_score_);

	void Update();
	void Draw();

private:	
	bool Collision();

private:
	// Engine
	Input* mp_input;
	Graphics* mp_graphics;

	// Collision
	CollisionDetectionOfMouseAndRect* mp_col_mouse_and_rect;

	// Object
	EnemyManager* mp_enemy_manager;

	// UI
	TimeLimit* mp_time_limit;
	Score* mp_score;

};

#endif