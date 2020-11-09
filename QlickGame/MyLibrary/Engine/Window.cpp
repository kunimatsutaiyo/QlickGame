#include "Window.h"
#include "../Utility/Size.h"

// ウィンドウプロシージャの作成
LRESULT CALLBACK WindowProcedure(
	HWND	window_handle_,
	UINT	message_id_,
	WPARAM	wparam_,
	LPARAM	lparam_)
{
	// メッセージに対する対応をする
	switch (message_id_)
	{
		// ウィンドウの「×」ボタンがクリックされた時
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		// メッセージの反応をしない
		return DefWindowProc(window_handle_, message_id_, wparam_, lparam_);
		break;
	}

	// メッセージの反応をした→0を返す
	return 0;
}

HWND Window::MakeWindow(HINSTANCE hInstance_, int width_, int height_, const char* title_name_)
{
	// 構造体の登録
	HWND hWnd;
	// WNDCLASSEX構造体に必要な情報を設定する
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEX),				// 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,		// クラスのスタイル
		WindowProcedure,				// ウィンドウプロシージャ
		0,								// 補助メモリ
		0,								// 補助メモリ
		hInstance_,						// このアプリのインスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),	// アイコン画像
		LoadCursor(NULL,IDC_ARROW),		// カーソル画像
		NULL,							// 背景ブラシ(背景色)
		NULL,							// メニュー名
		TEXT(WINDOW_CLASS_NAME),		// クラス名
		NULL							// 小さいアイコン
	};

	// WNDCLASSEX構造体の登録
	if (RegisterClassEx(&window_class) == 0)
	{
		return 0;
	}

	// ウィンドウの作成
	hWnd = CreateWindow(
		TEXT(WINDOW_CLASS_NAME),// 使用するウィンドウクラス名(RegisterClassで登録した名前を指定)
		(LPCWSTR)title_name_,	// ここでウィンドウのタイトル名を変更(東南アジア諸国連合)
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME),	// ウィンドウの種類、WS_THICKFRAMEはサイズ変更可能
		CW_USEDEFAULT,			// ウィンドウの初期座標(x座標)、CW_USEDEFAULTはシステム任せ
		0,						// ウィンドウの初期座標(y座標)、CW_USEDEFAULTはシステム任せ
		width_,					// ウィンドウの横幅
		height_,				// ウィンドウの縦幅
		NULL,					// 親ウィンドウハンドル(なければNULL)
		NULL,					// メニューハンドル(なければNULL)
		hInstance_,				// このウィンドウを保持するアプリのインスタンスハンドル
		NULL					// NULLでいい
	);

	if (hWnd == NULL)
	{
		return 0;
	}

	// ウィンドウのリサイズ
	RECT window_rect;
	RECT client_rect;

	// ウィンドウサイズの取得
	GetWindowRect(hWnd, &window_rect);
	// クライアント領域のサイズ取得
	GetClientRect(hWnd, &client_rect);

	// フレームサイズ算出
	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	// リサイズ用サイズの算出
	Size resize;
	resize.Width = frame_size_x + width_;
	resize.Height = frame_size_y + height_;

	// 表示位置の更新
	SetWindowPos(
		hWnd,				// 更新対象となるウィンドウハンドル
		NULL,				// このウィンドウよりも手前に表示するウィンドウハンドル
		CW_USEDEFAULT,		// ウィンドウの表示位置(x軸)
		CW_USEDEFAULT,		// ウィンドウの表示位置(y軸)
		resize.Width,		// ウィンドウの横幅
		resize.Height,		// ウィンドウの立幅
		SWP_NOMOVE			// ウィンドウサイズと位置の変更に関するフラグを指定
	);

	// ウィンドウ表示
	ShowWindow(
		hWnd,		// 表示設定を変更するウィンドウハンドル
		SW_SHOW		// 表示状態(SW_SHOWはウィンドウをアクティブにして、現在の位置とサイズで表示する)
	);

	// yttmでは触れていなかったやつ
	UpdateWindow(hWnd);

	return hWnd;
}