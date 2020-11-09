#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "../Utility/Singleton.h"
#include "Texture.h"
//#include "XFile.h"

// 解法処理用のマクロを定義
#define SAFE_RELEASE(x) { if(x) { (x)->Release(); (x) = NULL; } }

// フォントのサイズ
enum class FONT_SIZE
{
	SMALL,		// 小
	REGULAR,	// 中
	LARGE,		// 大

	MAX_FONT_SIZE
};

// フォントのカラー
enum class FONT_COLOR
{
	BLACK,	// 黒
	WHITE,	// 白
	RED,	// 赤

	MAX_FONT_COLOR
};

// スライダーの増減させる向き
enum class DIRECTION
{
	LEFT,
	RIGHT,

	MAX_DIRECTION
};

// 描画で必要とする頂点座標決める(2D)
struct CustomVertex2D {
	// 頂点座標
	float x;		// x軸座標
	float y;		// y軸座標
	float z;		// z軸座標	※3Dポリゴンを2Dポリゴンとして加工するため必要

	// 除算数
	float rhw;		// 除算数	※3Dから2Dへ加工するのに必要なパラメータ

	// ポリゴンの色
	DWORD color;	// 色情報

	// テクスチャ座標
	float tu;		// uテクスチャ座標
	float tv;		// vテクスチャ座標
};

// 描画で必要とする頂点座標決める(3D)
struct CustomVertex3D {
	float x;
	float y;
	float z;

	float tu;
	float tv;

	DWORD color;
};

// 3D空間上にテクスチャを張り付けるためのデータ
struct DrawData3D {
	float x;
	float y;
	float z;

	float width;
	float height;

	DWORD color;

	float rotx;		// x回転
	float roty;		// y回転
	float rotz;		// z回転

	float scalex;	// 拡縮x
	float scaley;	// 拡縮y
					// 拡縮zはテクスチャなのでない
};

class Graphics : public Singleton<Graphics> {
private:
	friend Singleton<Graphics>;

public:
	/**
	* @brief DirectGraphicsを初期化する関数
	* @param[in] window_handle_	ウィンドウハンドル
	*/
	bool InitDirectGraphics(HWND window_handle_);

	/**
	* @prief フォントを描画するための初期化
	*/
	bool CreateFontDevice();

	/**
	* @brief 透過処理を行うための初期化処理
	* ※透過処理を行いたいテクスチャの前に
	*/
	void Transparency(bool true_or_false_);

	/**
	* @brief デバイス・インターフェースを開放する関数
	*/
	void ReleaseDirectGraphics();

	/**
	* @brief 描画開始関数
	*/
	bool StartDraw();

	/**
	* @brief 描画終了関数
	*/
	void EndDraw();

	/**
	* @brief フォント描画関数
	* @param[in] text_ 描画するテキスト
	* @param[in] size_ 描画するテキストのサイズ
	* @param[in] color_ 描画するテキストの色
	* @param[in] x_ x座標
	* @param[in] y_ y座標
	*/
	void DrawFont(const std::string text_, FONT_SIZE size_, FONT_COLOR color_, float x_, float y_);

	/**
	* @brief 2D用描画関数
	* @param[in] x_ x座標
	* @param[in] y_ y座標
	* @param[in] width_ 横幅
	* @param[in] height_ 縦幅
	* @param[in] transparency_ テクスチャの透過度
	*/
	void DrawPolygon(float x_, float y_, float width_, float height_, UCHAR transparency_ = 255);

	/**
	* @brief 2D用描画関数
	* @param[in] p_texture_ds_ テクスチャのデータ
	* @param[in] x_ x座標
	* @param[in] y_ y座標
	* @param[in] transparency_ テクスチャの透過度
	* @param[in] tex_width テクスチャの横幅
	* @param[in] tex_height テクスチャの縦幅
	*/
	void DrawTexture(TextureDataAndSize* p_texture_ds_, float x_, float y_, UCHAR transparency_ = 255, int tex_width = NULL, int tex_height = NULL);

	/**
	* @brief 2D用アニメーション関数
	* @param[in] p_texture_ds_ テクスチャのデータ
	* @param[in] x_ x座標
	* @param[in] y_ y座標
	* @param[in] split_x_ 横の分割数
	* @param[in] split_y_ 縦の分割数
	* @param[in] animation_graph_num_ アニメーションするテクスチャの番号(左上から右下に向けて)
	*/
	void Animation2D(
		TextureDataAndSize* p_texture_ds_,
		float x_,
		float y_,
		int split_x_,
		int split_y_,
		int animation_graph_num_);

	/**
	* @brief 2D用スライダー関数
	* @param[in] p_texture_ds_ テクスチャのデータ
	* @param[in] direction_ 増減させる向き(確定した方を+、逆を-)
	* @param[in] x_ x座標
	* @param[in] y_ y座標
	* @param[in] max_width_ 描画したい幅
	* @param[in] max_height_ 描画したい高さ
	* @param[in] num_ 現在の値
	* @param[in] min_num_ 最小値
	* @param[in] max_num_ 最大値
	*/
	void Slider2D(
		TextureDataAndSize* p_texture_ds_,
		DIRECTION direction_,
		float x_,
		float y_,
		float max_width_,
		float max_height_,
		float num_,
		float min_num_,
		float max_num_
	);

	/**
	* @brief 3D用描画関数
	*/
	void DrawTexture3D(TextureDataAndSize* p_texture_ds_, DrawData3D& dd3D_);

	/**
	* @brief ~~~~~~~~~~~~~~~~関数
	*/
	bool CreateTexture(std::string file_name_, TextureDataAndSize* p_texture_ds_);

	bool SetView(const D3DMATRIX& matView_)const;

	void GetViewport_Camera(D3DVIEWPORT9* vp_);

	bool SetMatProj(const D3DMATRIX& matProj_)const;

#pragma region XFile用

	/*bool CreateXFile(const char* file_name_, XFileData* xfile_data_);

	void DrawXFile(float pos_x_, float pos_y_, float pos_z_, XFILE_LIST xf_id_);*/

#pragma endregion

	const LPDIRECT3DDEVICE9 GetD3DDevice(void) {
		return d3ddevice;
	}

private:
	Graphics()
	{
		d3ddevice = nullptr;
		d3dinterface = nullptr;

		m_font_size_small = 16;
		m_font_size_regular = 24;
		m_font_size_large = 32;
	}
	~Graphics() {}

	// コピーコンストラクタの禁止
	Graphics(const Graphics&);
	Graphics& operator=(const Graphics&) = delete;

	// ムーブコンストラクタの禁止
	Graphics& operator=(const Graphics&&) = delete;

private:
	static Graphics* p_GraphicsInstance;	// Graphicsのインスタンス

	LPDIRECT3D9 d3dinterface;		// DirectGraphicsインターフェイス
	LPDIRECT3DDEVICE9 d3ddevice;	// DirectGraphicsデバイス

	LPD3DXFONT m_font_list[static_cast<int>(FONT_SIZE::MAX_FONT_SIZE)];
	int m_font_size_small;
	int m_font_size_regular;
	int m_font_size_large;

};

#endif