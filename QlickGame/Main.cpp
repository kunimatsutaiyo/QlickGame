#include <Windows.h>

#include "MyLibrary/Engine/Window.h"
#include "MyLibrary/Engine/Engine.h"
#include "MyLibrary/Engine/Input.h"
#include "MyLibrary/Engine/Graphics.h"
#include "MyLibrary/Engine/Texture.h"
#include "MyLibrary/Engine/Sound.h"

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
			




		}
	}

	// エンジン終了
	engine.EndEngine();

	//return msg.wParam;
	return 0;
}