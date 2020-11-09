#ifndef ENGINE_H_
#define ENGINE_H_

#include <Windows.h>

class Engine {
public:
	/**
	* @brief エンジン初期化関数
	* ゲームで使うエンジン部分(描画、入力)の初期化
	* ※この関数はゲームループ開始前に1度だけ実行する
	* @return 初期化結果(true成功)
	*/
	bool InitEngine(HINSTANCE hInstance_, HWND window_handle_);

	/**
	* @brief エンジン終了関数
	* ゲーム終了後にエンジン部分の終了を行う
	* ※この関数はゲームループ終了後に1度だけ実行する
	*/
	void EndEngine();
};

#endif