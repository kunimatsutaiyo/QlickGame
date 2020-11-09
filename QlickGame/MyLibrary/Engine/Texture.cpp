#include "Texture.h"
#include "Graphics.h"

// テクスチャ保存用配列
//static TextureDataAndSize g_TextureList[static_cast<int>(TEXTURE_LIST::TEXTURE_LIST_MAX)];

void Texture::InitTexture()
{
	AllReleaseTexture();

	/*for (int i = 0; i < static_cast<int>(TEXTURE_LIST::TEXTURE_LIST_MAX); i++)
	{
		g_TextureList[i].TextureData = nullptr;
	}*/
}

bool Texture::ReleaseCategoryTexture(std::string key_name_)
{
	/*for (int i = 0; i < static_cast<int>(TEXTURE_LIST::TEXTURE_LIST_MAX); i++)
	{
		if (g_TextureList[i].TextureData == nullptr)
		{
			continue;
		}

		g_TextureList[i].TextureData->Release();
		g_TextureList[i].TextureData = nullptr;
	}*/

	if (HasKeyName(key_name_) == false)
	{
		return false;
	}
	delete m_texture_list[key_name_];
	m_texture_list.erase(key_name_);

	return true;
}

void Texture::AllReleaseTexture()
{
	for (auto& texture : m_texture_list)
	{
		delete texture.second;
	}

	m_texture_list.clear();
}

/* テクスチャをIDで登録するときに使用していた方 */
//bool Texture::LoadTexture(const char* file_name_, int texture_id_)
//{
//	if (IsRangeCheck(texture_id_) == false)
//	{
//		return false;
//	}
//
//	return Graphics::GetInstance()->CreateTexture(file_name_, &g_TextureList[static_cast<int>(texture_id_)]);
//}
bool Texture::LoadTexture(std::string file_name_, std::string key_name_)
{
	if (HasKeyName(key_name_) == true)
	{
		return false;
	}
	m_texture_list[key_name_] = new TextureDataAndSize;

	return Graphics::GetInstance()->CreateTexture(file_name_, m_texture_list[key_name_]);
}

bool Texture::HasKeyName(std::string key_name_)
{
	auto it = m_texture_list.find(key_name_);

	if (it == m_texture_list.end())
	{
		return false;
	}
	return true;
}

//bool Texture::IsRangeCheck(int texture_id_)
//{
//	if (texture_id_ <= -1 ||
//		texture_id_ >= static_cast<int>(TEXTURE_LIST::TEXTURE_LIST_MAX))
//	{
//		return false;
//	}
//
//	return true;
//}