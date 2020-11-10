#include <Windows.h>

#include "MyLibrary/Engine/Window.h"
#include "MyLibrary/Engine/Engine.h"
#include "MyLibrary/Engine/Input.h"
#include "MyLibrary/Engine/Graphics.h"
#include "MyLibrary/Engine/Texture.h"
#include "MyLibrary/Engine/Sound.h"

#include "MyLibrary/Object/EnemyManager.h"
#include "MyLibrary/Collision/MouseAndRect.h"

int APIENTRY WinMain(
	HINSTANCE	hInstance_,		// インスタンスハンドル
	HINSTANCE	hPrevInstance_,	// 旧仕様(NULLが常に入っているので無視)
	LPSTR		lpCmpLine_,		// コマンドライン
	INT			nCmdShow_		// 表示状態
)
{
	Window window;
	Engine engine;
	HWND window_handle = window.MakeWindow(hInstance_, 1920, 1080, "ポチポチゲーム");
	engine.InitEngine(hInstance_, window_handle);



	Texture::GetInstance()->LoadTexture("Res/free_enemy.jpg", "enemy");
	Graphics* gp = Graphics::GetInstance();

	EnemyManager enemy_manager(2);

	MouseAndRect mouse_and_rect;



	// メインループ
	while (true)
	{
		// メッセージ構造体
		MSG msg;

		// メッセージ受信
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// WM_QUITで終了
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else {
				// メッセージ翻訳
				TranslateMessage(&msg);
				// プロシージャにメッセージ送信
				DispatchMessage(&msg);
			}
		}
		else {
			// Inputの処理
			// escキーで終了
			Input::GetInstance()->Update();
			if (Input::GetInstance()->GetKey(KEY_TYPE::ESCAPE_KEY))
			{
				break;
			}



			// game処理 //
			// 敵の更新
			enemy_manager.Update();

			// 当たり判定
			if (Input::GetInstance()->OnMouseDown(MouseButton::LEFT))
			{
				D3DXVECTOR2 mouse_pos = Input::GetInstance()->GetMousePos();
				std::vector<Enemy*> vec_enmey = enemy_manager.GetEnemy();

				for (size_t i = 0; i < vec_enmey.size(); i++)
				{
					if (mouse_and_rect.Judgment(mouse_pos, vec_enmey[i]->GetPos(), vec_enmey[i]->GetSize()))
					{
						enemy_manager.PartDelete(i);
					}
				}
			}



			// draw処理 //
			// 描画開始
			if (gp->StartDraw() == true)
			{
				enemy_manager.Draw();
			}
			// 描画終了
			gp->EndDraw();
		}
	}

	// エンジン終了
	engine.EndEngine();

	//return msg.wParam;
	return 0;
}