#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../Utility/Singleton.h"

#include <d3d9.h>
#include <string>
#include <map>

/** @brief テクスチャデータやサイズを保持する構造体 */
struct TextureDataAndSize {
	LPDIRECT3DTEXTURE9 TextureData;	// テクスチャデータ
	int Width;						// 横幅
	int Height;						// 縦幅
};

class Texture : public Singleton<Texture> {
private:
	friend Singleton<Texture>;

public:
	/**
	* @brief テクスチャデータ初期化関数
	* ゲームで使用するテクスチャデータを保存できるようにする
	*/
	void InitTexture();

	/**
	* @brief カテゴリー単位のテクスチャ解放関数
	* 引数で指定されたカテゴリーのテクスチャを全て解放する
	* @param[in] key_name_ 登録したいテクスチャの名前
	*/
	bool ReleaseCategoryTexture(std::string key_name);

	/**
	* @brief テクスチャ全解放関数
	* 読み込んでいる全てのテクスチャを解放する
	* ※この関数はEndEngineで使用されるので開発側で呼ぶ必要はない
	*/
	void AllReleaseTexture();

	/**
	* @brief テクスチャの読み込み関数
	* 指定されたパスのテクスチャを読み込み、カテゴリーに登録する
	* @return 読み込み結果(成功はtrue)
	* @param[in] file_name_読み込むテクスチャ名(パス込み)
	* @param[in] key_name_ 登録したいテクスチャの名前
	*/
	bool LoadTexture(std::string file_name_, std::string key_name_);

	/**
	* @brief テクスチャデータの取得関数
	* 指定されたカテゴリーのテクスチャデータを取得する
	* @return テクスチャデータ(取得失敗時はnullptr)
	* @param[in] key_name 取得したいテクスチャの名前
	*/
	/* テクスチャをIDで登録するときに使用していた方 */
	/*TextureDataAndSize* Texture::GetTexture(int texture_id_) {
		if (IsRangeCheck(texture_id_) == false)
		{
			return nullptr;
		}

		return &g_TextureList[texture_id_];
	}*/
	TextureDataAndSize* GetTexture(std::string key_name_) {
		if (HasKeyName(key_name_) == false)
		{
			return nullptr;
		}
		return m_texture_list[key_name_];
	}

	std::string GetTextureName(TextureDataAndSize* p_texture_data_) {
		for (auto iter = m_texture_list.begin(); iter != m_texture_list.end(); iter++)
		{
			if (p_texture_data_ == iter->second)
			{
				return iter->first;
			}
		}
	}

private:
	Texture() {}
	~Texture() {}

	// コピーコンストラクタの禁止
	Texture(const Texture&);
	Texture& operator = (const Texture&) = delete;
	// ムーブコンストラクタの禁止
	Texture& operator = (const Texture&&) = delete;

private:
	//bool IsRangeCheck(int texture_id_);

	/**
	* @brief TextureListのキーチェック関数
	* 引数で指定されたキーが登録されているかを確認します
	* return 登録されていた場合はtrue、未登録の場合はfalse
	* @param[in] key_name　チェックしたいキー
	*/
	bool HasKeyName(std::string key_name_);

private:
	static Texture* p_TextureInstance;	// Textureのインスタンス

	std::map<std::string, TextureDataAndSize*> m_texture_list;	// Texture構造体保存用ポインタ配列

};

#endif