#include "Graphics.h"

bool Graphics::InitDirectGraphics(HWND window_handle_)
{
	D3DPRESENT_PARAMETERS present_param;

	// インターフェース作成
	d3dinterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3dinterface == NULL)
	{
		// 作成失敗
		return false;
	}

	// D3DPRESENT_PARAMETERSの設定
	// 下記のメンバ変数以外に無効な値が設定されないように0クリアを行う(present_param.BackBuffer...のやつ)
	ZeroMemory(&present_param, sizeof(D3DPRESENT_PARAMETERS));

	// バックバッファの数
	present_param.BackBufferCount = 1;
	// バックバッファのフォーマット(D3DFMT_UNKNOWNはフォーマットを知らない)
	present_param.BackBufferFormat = D3DFMT_UNKNOWN;
	// ウィンドウモード設定→定数で切り替え(true:ウィンドウ、false:フルスクリーン)
	present_param.Windowed = true;
	// スワップエフェクト→D3DSWAPEFFECT_DISCARD(自動設定)
	present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// 他のバックバッファはyttmを参照！


	// XFileで追加した
	// ZバッファON
	present_param.EnableAutoDepthStencil = TRUE;
	present_param.AutoDepthStencilFormat = D3DFMT_D24S8;


	// IDirect3DDeviceの作成
	if (FAILED(d3dinterface->CreateDevice(
		D3DADAPTER_DEFAULT,	// ディスプレイアダプタの種類
		D3DDEVTYPE_HAL,		// デバイスの種類を設定
		window_handle_,		// ウィンドウハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,	// デバイス制御の組み合わせ
		&present_param,		// デバイスを設定するためのD3DPRESENT_PARAMETERS構造体
		&d3ddevice)))		// IDirect3DDevice9を格納するIDirect3DDevice9のポインタのアドレス
	{
		return false;
	}

	// ビューポートパラメータ
	D3DVIEWPORT9 view_port;

	// マルチプレイを行う時みたいに画面を割りたい時に下記をいじる場合がある
	// ビューポートの左上座標
	view_port.X = 0;			// 描画領域のx座標
	view_port.Y = 0;			// 描画領域のy座標

	// ビューポートのサイズ
	view_port.Width = present_param.BackBufferWidth;	// 描画領域の横幅
	view_port.Height = present_param.BackBufferHeight;	// 描画領域の縦幅

	// ビューポート深度設定
	view_port.MinZ = 0.0f;		// クリップ領域の最小深度、基本は0.0fでいい
	view_port.MaxZ = 1.0f;		// クリップ領域の最大深度、基本は1.0fでいい

	// ビューポートの設定
	if (FAILED(d3ddevice->SetViewport(&view_port)))
	{
		return false;
	}

	// フォントデバイスの作成
	if (CreateFontDevice() == false)
	{
		return false;
	}

	Transparency(true);

	return true;
}

bool Graphics::CreateFontDevice()
{
	int size_list[] = {
		m_font_size_small,
		m_font_size_regular,
		m_font_size_large
	};

	for (int i = 0; i < static_cast<int>(FONT_SIZE::MAX_FONT_SIZE); i++)
	{
		if (FAILED(D3DXCreateFont(
			d3ddevice,					// DirectGraphicsデバイス
			size_list[i],				// フォントの横幅のサイズ
			size_list[i] / 2,			// フォントの立幅のサイズ
			FW_REGULAR,
			0,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			PROOF_QUALITY,
			FIXED_PITCH | FF_SCRIPT,
			TEXT("ＭＳ　Ｐゴシック"),	// フォントの種類(ＭＳ　ゴシック、Courier Newなど)
			&m_font_list[i]				// フォント文字列用クラス変数へポインタを指定
		)))
		{
			return false;
		}
	}

	return true;
}

void Graphics::Transparency(bool true_or_false_)
{
	if (true_or_false_ == true)
	{
		d3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		d3ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		d3ddevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		d3ddevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		d3ddevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
	else {
		d3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		d3ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		d3ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	}
}

// 描画のクリア関数
void Graphics::ReleaseDirectGraphics()
{
	// 特別な理由がない限り作成した時と逆の順番で開放する
	d3ddevice->Release();
	d3dinterface->Release();
}

bool Graphics::StartDraw()
{
	// ①シーンクリア
	/*
		2D：第3、第4引数の指定をしっかりする

		3D：第3引数にデプス、ステンシルのバッファ設定
			第5、第6引数でクリアする値を指定する
	*/
	d3ddevice->Clear(
		0,									// 第2引数で使用するconst D3DRECT*の数
		NULL,								// D3DRECTの配列(クリアする矩形の範囲を変更したい場合に使用、nullptrならビューポート全体をクリア)
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	//D3DCLEAR_TARGET,			// クリアするバッファの種類を決めるフラグの設定(D3DCLEAR_TARGET、D3DCLEAR_STENCIL、D3DCLEAR_ZBUFFER)
		D3DCOLOR_XRGB(0, 0, 0),				// バックバッファをクリアする色の情報
		1.0f,								// デプスバッファをクリアする値
		0									// ステンシルバッファをクリアする値
	);

#pragma region 普段はこっちのSetRenderState

	d3ddevice->SetRenderState(D3DRS_LIGHTING, FALSE);

#pragma endregion

#pragma region XFile描画時のSetRenderStateとLightEnable

	//d3ddevice->SetRenderState(D3DRS_LIGHTING, true);
	//d3ddevice->LightEnable(0, true);

#pragma endregion

	// ②シーン描画開始
	if (D3D_OK == d3ddevice->BeginScene())
	{
		return true;
	}

	return false;
}

void Graphics::EndDraw()
{
	// ③シーン描画終了
	d3ddevice->EndScene();

	// ④バックバッファ転送
	d3ddevice->Present(
		nullptr,	// 転送元矩形
		nullptr,	// 転送先矩形
		nullptr,	// 使用するウィンドウハンドル
		nullptr		// 基本nullptr
	);
}

void Graphics::DrawFont(const std::string text_, FONT_SIZE size_, FONT_COLOR color_, float x_, float y_)
{
	if (m_font_list[static_cast<int>(size_)] == nullptr)
	{
		return;
	}

	RECT rect = {
		(long)x_,
		(long)y_,
		(long)x_ + 800,
		(long)y_ + 300
	};

	int r, g, b;
	r = g = b = 255;

	if (color_ == FONT_COLOR::BLACK)
	{
		r = g = b = 0;
	}
	else if (color_ == FONT_COLOR::RED)
	{
		r = 255;
		g = b = 0;
	}

	m_font_list[static_cast<int>(size_)]->DrawTextA(
		nullptr,
		text_.c_str(),
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_XRGB(r, g, b)
	);
}

// 透過処理を行っている場合、α値をいじっても透明のままやった
void Graphics::DrawPolygon(float x_, float y_, float width_, float height_, UCHAR transparency_)
{
	Transparency(true);

	DWORD color = D3DCOLOR_RGBA(0, 0, 0, transparency_);
	CustomVertex2D cv[] = {
		{x_, y_, 0.0f, 1.0f, color, 0.0f, 0.0f},					// 白0xffffffff	// 左上
		{x_ + width_, y_, 0.0f, 1.0f , color, 1.0f, 0.0f},			// 赤0xffff0000	// 右上
		{x_ + width_, y_ + height_, 0.0f, 1.0f, color, 1.0f, 1.0f},	// 緑0xff00ff00	// 右下
		{x_, y_ + height_, 0.0f, 1.0f, color, 0.0f, 1.0f}			// 黒0xff000000	// 左下
	};

	d3ddevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,//D3DPT_TRIANGLESTRIP,	// ポリゴンの作成方法→リスト、ストリップ、ファンがある
		2,											// 描画するポリゴンの数
		cv,											// ポリゴンを描画するために使う頂点の配列
		sizeof(CustomVertex2D)						// 頂点情報のサイズ
	);
}

// 2D用
void Graphics::DrawTexture(TextureDataAndSize* p_texture_ds_, float x_, float y_, UCHAR transparency_, int tex_width, int tex_height)
{
	Transparency(true);

	if (tex_width == NULL)
	{
		tex_width = p_texture_ds_->Width;
	}
	if (tex_height == NULL)
	{
		tex_height = p_texture_ds_->Height;
	}

	// 頂点情報の準備
	// z:0.0f、Rhw:1.0f固定でいい
	// CustomVertexに色情報を追加する→0xffffffffのやつ

	DWORD color = D3DCOLOR_RGBA(255, 255, 255, transparency_);
	CustomVertex2D cv[] = {
		{x_, y_, 0.0f, 1.0f, color, 0.0f, 0.0f},							// 白0xffffffff	// 左上
		{x_ + tex_width, y_, 0.0f, 1.0f , color, 1.0f, 0.0f},				// 赤0xffff0000	// 右上
		{x_ + tex_width, y_ + tex_height, 0.0f, 1.0f, color, 1.0f, 1.0f},	// 緑0xff00ff00	// 右下
		{x_, y_ + tex_height, 0.0f, 1.0f, color, 0.0f, 1.0f}				// 黒0xff000000	// 左下
	};

	// SetFVFで頂点情報の登録を行う
	// 登録する情報が複数ある時は「|」の記号を使って一度にすべて登録する
	// ポリゴンに色を追加するときはD3DFVF_DIFFUSEを追加
	// D3DFVF_XYZRHW：スクリーン座標変換までを自動的にやってくれるための情報という認識で
	// 2Dの時のみD3DFVF_XYZRHWを使う
	d3ddevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);

	// ポリゴンを描画する
	// ※個々の処理は必ずBeginSceneとEndSceneの間で行うこと
	// ※関数で分けているのでStartDraw関数とEndDraw関数の間ということになる
	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,//D3DPT_TRIANGLESTRIP,	// ポリゴンの作成方法→リスト、ストリップ、ファンがある
		2,											// 描画するポリゴンの数
		cv,											// ポリゴンを描画するために使う頂点の配列
		sizeof(CustomVertex2D)						// 頂点情報のサイズ
	);
}

void Graphics::Animation2D(
	TextureDataAndSize* p_texture_ds_,
	float x_, float y_,
	int split_x_, int split_y_,
	int animation_graph_num_)
{
	// アニメーションする番号(x軸,y軸)
	int animation_x = (animation_graph_num_ % split_x_);
	int animation_y = (animation_graph_num_ / split_y_) + 1;

	float tu = 1.0f / split_x_;
	float tv = 1.0f / split_y_;

	float tu_num = tu * animation_x;
	float tv_num = tv * animation_y;

	int graph_width = p_texture_ds_->Width / split_x_;
	int graph_height = p_texture_ds_->Height / split_y_;

	CustomVertex2D cv[] = {
		{x_, y_, 0.0f, 1.0f, 0xffffffff, tu_num, tv_num},
		{x_ + graph_width, y_, 0.0f, 1.0f , 0xffffffff, tu_num + tu, tv_num},
		{x_ + graph_width, y_ + graph_height, 0.0f, 1.0f, 0xffffffff, tu_num + tu, tv_num + tv},
		{x_, y_ + graph_height, 0.0f, 1.0f, 0xffffffff, tu_num, tv_num + tv}
	};

	d3ddevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);
	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		cv,
		sizeof(CustomVertex2D)
	);
}

void Graphics::Slider2D(
	TextureDataAndSize* p_texture_ds_,
	DIRECTION direction_,
	float x_, float y_,
	float max_width_, float max_height_,
	float num_, float min_num_, float max_num_)
{
	// 指定したテクスチャのサイズが使用しているテクスチャのサイズを超えていたら
	// 使用しているテクスチャのサイズに変換してる
	if (p_texture_ds_->Width < max_width_)
	{
		max_width_ = p_texture_ds_->Width;
	}
	if (p_texture_ds_->Height < max_height_)
	{
		max_height_ = p_texture_ds_->Height;
	}

	// 現在の値を比率として出す
	// (現在の値 - 最小の値) / (最大の値 - 最小の値)
	float rate = (num_ - min_num_) / (max_num_ - min_num_);

	// 増減する向きをみて描画する幅をだす
	if (direction_ == DIRECTION::LEFT)
	{
		// 描画開始位置を右にする(RIGHTの逆)
		// 比率から描画開始位置をずらす
		x_ = (x_ + max_width_) - (rate * max_width_);
		// サイズも比率で変更する
		max_width_ *= rate;
	}
	else if (direction_ == DIRECTION::RIGHT)
	{
		max_width_ *= rate;
	}

	float left = x_ / p_texture_ds_->Width;
	float right = (x_ + max_width_) / p_texture_ds_->Width;
	float top = y_ / p_texture_ds_->Height;
	float bottom = (y_ + max_height_) / p_texture_ds_->Height;

	CustomVertex2D cv[] = {
		{x_, y_, 0.0f, 1.0f, 0xffffffff, left, top},
		{x_ + max_width_, y_, 0.0f, 1.0f , 0xffffffff, right, top},
		{x_ + max_width_, y_ + max_height_ , 0.0f, 1.0f, 0xffffffff, right, bottom},
		{x_, y_ + max_height_, 0.0f, 1.0f, 0xffffffff, left, bottom}
	};

	d3ddevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);
	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		cv,
		sizeof(CustomVertex2D)
	);
}

// 3D用
void Graphics::DrawTexture3D(TextureDataAndSize* p_texture_ds_, DrawData3D& dd3D_)
{
	float top_tv = dd3D_.y / dd3D_.height;						// 上
	float bottom_tv = (dd3D_.y + dd3D_.height) / dd3D_.height;	// 下
	float left_tu = dd3D_.x / dd3D_.width;					// 左
	float right_tu = (dd3D_.x + dd3D_.width) / dd3D_.width;	// 右

	float harf_x = dd3D_.width / 2.0f;		// 横幅の中心
	float harf_y = dd3D_.height / 2.0f;		// 縦幅の中心

	// ポリゴンのローカル座標の位置を指定
	CustomVertex3D cv3D[] =
	{
		{-harf_x, harf_y, dd3D_.z, left_tu, top_tv, D3DCOLOR_ARGB(255, 255, 255, 255)},		// 左上
		{harf_x, harf_y, dd3D_.z, right_tu, top_tv, D3DCOLOR_ARGB(255, 255, 255, 255)},		// 右上
		{harf_x, -harf_y, dd3D_.z, right_tu, bottom_tv, D3DCOLOR_ARGB(255, 255, 255, 255)},	// 右下
		{-harf_x, -harf_y, dd3D_.z, left_tu, bottom_tv, D3DCOLOR_ARGB(255, 255, 255, 255)},	// 左下
	};

	// ワールド座標変換用の行列の算出 start
	D3DXMATRIX mat_world, mat_trans, mat_rot, mat_rotx, mat_roty, mat_rotz, mat_scale;

	// 単位行列化
	D3DXMatrixIdentity(&mat_world);
	D3DXMatrixIdentity(&mat_scale);
	D3DXMatrixIdentity(&mat_rot);
	D3DXMatrixIdentity(&mat_trans);

	// 拡縮
	D3DXMatrixScaling(&mat_scale, dd3D_.scalex, dd3D_.scaley, 1.0f);
	//m_p_matrix->MatrixScaling(&mat_scale, dd3D_.scalex, dd3D_.scaley, 1.0f);

	// 回転
	D3DXMatrixRotationX(&mat_rotx, D3DXToRadian(dd3D_.rotx));
	D3DXMatrixRotationY(&mat_roty, D3DXToRadian(dd3D_.roty));
	D3DXMatrixRotationZ(&mat_rotz, D3DXToRadian(dd3D_.rotz));
	//m_p_matrix->MatrixRotationX(&mat_rotx, D3DXToRadian(dd3D_.rotx));
	//m_p_matrix->MatrixRotationY(&mat_roty, D3DXToRadian(dd3D_.roty));
	//m_p_matrix->MatrixRotationZ(&mat_rotz, D3DXToRadian(dd3D_.rotz));

	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_rotx);
	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_roty);
	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_rotz);
	//m_p_matrix->MatrixMultiply(&mat_rot, &mat_rotx, &mat_roty, &mat_rotz);

	// 移動							 
	D3DXMatrixTranslation(&mat_trans, dd3D_.x, dd3D_.y, dd3D_.z);
	//m_p_matrix->MatrixTranslation(&mat_trans, dd3D_.x, dd3D_.y, dd3D_.z);

	// かけ合わせ(拡縮×回転×移動)
	mat_world *= mat_scale * mat_rot * mat_trans;
	//m_p_matrix->MatrixMultiply(&mat_world, &mat_scale, &mat_rot, &mat_trans);

	d3ddevice->SetTransform(D3DTS_WORLD, &mat_world);
	// ワールド座標変換用の行列の算出 end

	d3ddevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);

	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		cv3D,
		sizeof(CustomVertex3D)
	);
#pragma region レノ君コード

	/*d3ddevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);

	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		v,
		sizeof(CustomVertex3D)
	);*/

#pragma endregion
}

bool Graphics::CreateTexture(std::string file_name_, TextureDataAndSize* p_texture_ds_)
{
	D3DXIMAGE_INFO info;

	// 2の累乗出ないケースを想定して元のサイズを取得してD3DXCreateTextureFromFileExで使う
	D3DXGetImageInfoFromFileA(file_name_.c_str(), &info);

	// テクスチャの読み込み
	/*D3DXCreateTextureFromFile(
		d3ddevice,
		p_file_name_,
		&texture_data_->TextureData
	);*/

	// テクスチャの読み込み(Ex)
	// ※読み込まれた画像サイズが2の累乗でなかった場合、自動的に2の累乗に修正される
	// ※読み込んだサイズのままにしたいときは第3、第4引数に元のファイルサイズを指定すればいい
	if (FAILED(D3DXCreateTextureFromFileExA(
		d3ddevice,
		file_name_.c_str(),
		info.Width,
		info.Height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0x0000ff00,
		nullptr,
		nullptr,
		&p_texture_ds_->TextureData
	)))
	{
		return false;
	}
	else {
		// テクスチャサイズの取得
		D3DSURFACE_DESC desc;

		if (FAILED(p_texture_ds_->TextureData->GetLevelDesc(0, &desc)))
		{
			p_texture_ds_->TextureData->Release();
			return false;
		}
		p_texture_ds_->Width = desc.Width;
		p_texture_ds_->Height = desc.Height;
	}

	return true;
}

// ※木村君のを参考にしました
// 引数の後ろにあるconstはこのクラスのメンバ変数を変更できないことを表している
// メンバ変数(フィールド)
// 関数(メソッド)
bool Graphics::SetView(const D3DMATRIX& matView_)const
{
	if (FAILED(d3ddevice->SetTransform(D3DTS_VIEW, &matView_)))
	{
		return false;
	}

	return true;
}
// ※木村君のを参考にしました
void Graphics::GetViewport_Camera(D3DVIEWPORT9* vp_)
{
	d3ddevice->GetViewport(vp_);
}
// ※木村君のを参考にしました
bool Graphics::SetMatProj(const D3DMATRIX& matProj_) const
{
	if (FAILED(d3ddevice->SetTransform(D3DTS_PROJECTION, &matProj_)))
	{
		return false;
	}

	return true;
}

// xFile
//bool Graphics::CreateXFile(const char* file_name_, XFileData* xfile_data_)
//{
//	// XFileの読み込み
//	if (FAILED(D3DXLoadMeshFromXA(
//		file_name_,					// ファイル名(パス込み)
//		D3DXMESH_SYSTEMMEM,			// メッシュ作成用のオプションを指定
//		d3ddevice,					// DirectGraphicsのデバイス
//		nullptr,					// 隣接性データ(基本nullptrでいい)
//		&xfile_data_->Materials,	// マテリアルデータ
//		nullptr,					// エフェクトデータ(基本nullptrでいい)
//		&xfile_data_->MaterialNum,	// マテリアルの数
//		&xfile_data_->Meshes		// メッシュデータ
//	)))
//	{
//		return false;
//	}
//
//	// マテリアル解析
//	// メッシュに使用されているテクスチャ用の配列を用意する
//	xfile_data_->Textures = new LPDIRECT3DTEXTURE9[xfile_data_->MaterialNum];
//
//	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
//	D3DXMATERIAL* materials = (D3DXMATERIAL*)xfile_data_->Materials->GetBufferPointer();
//
//	// 各メッシュのマテリアル情報を取得する
//	for (DWORD i = 0; i < xfile_data_->MaterialNum; i++)
//	{
//		// マテリアルで設定されているテクスチャ読み込み
//		if (materials[i].pTextureFilename != NULL)
//		{
//			char* file_name = materials[i].pTextureFilename;
//			LPDIRECT3DTEXTURE9 texture = NULL;
//			D3DXCreateTextureFromFileA(
//				d3ddevice,
//				file_name,
//				&xfile_data_->Textures[i]
//			);
//		}
//		else {
//			xfile_data_->Textures[i] = nullptr;
//		}
//	}
//
//	return true;
//}
//
//void Graphics::DrawXFile(float pos_x_, float pos_y_, float pos_z_, XFILE_LIST xf_id_)
//{
//
//	XFileData* xfile = XFile::GetInstance()->GetXFileData(xf_id_);
//
//	if (xfile == nullptr)
//	{
//		return;
//	}
//
//	// 移動処理
//	D3DXMATRIX matrix;
//	D3DXMatrixIdentity(&matrix);
//	D3DXMatrixTranslation(&matrix, pos_x_, pos_y_, pos_z_);
//	d3ddevice->SetTransform(D3DTS_WORLD, &matrix);
//
//	// XFile描画
//	D3DXMATERIAL* materials = (D3DXMATERIAL*)xfile->Materials->GetBufferPointer();
//	for (DWORD i = 0; i < xfile->MaterialNum; i++)
//	{
//		// マテリアルの設定
//		d3ddevice->SetMaterial(&materials[i].MatD3D);
//
//		// テクスチャの設定
//		d3ddevice->SetTexture(0, xfile->Textures[i]);
//
//		// メッシュを描画
//		xfile->Meshes->DrawSubset(i);
//	}
//}