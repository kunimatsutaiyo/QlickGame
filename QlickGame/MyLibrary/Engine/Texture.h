#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "../Utility/Singleton.h"

#include <d3d9.h>
#include <string>
#include <map>

/** @brief �e�N�X�`���f�[�^��T�C�Y��ێ�����\���� */
struct TextureDataAndSize {
	LPDIRECT3DTEXTURE9 TextureData;	// �e�N�X�`���f�[�^
	int Width;						// ����
	int Height;						// �c��
};

class Texture : public Singleton<Texture> {
private:
	friend Singleton<Texture>;

public:
	/**
	* @brief �e�N�X�`���f�[�^�������֐�
	* �Q�[���Ŏg�p����e�N�X�`���f�[�^��ۑ��ł���悤�ɂ���
	*/
	void InitTexture();

	/**
	* @brief �J�e�S���[�P�ʂ̃e�N�X�`������֐�
	* �����Ŏw�肳�ꂽ�J�e�S���[�̃e�N�X�`����S�ĉ������
	* @param[in] key_name_ �o�^�������e�N�X�`���̖��O
	*/
	bool ReleaseCategoryTexture(std::string key_name);

	/**
	* @brief �e�N�X�`���S����֐�
	* �ǂݍ���ł���S�Ẵe�N�X�`�����������
	* �����̊֐���EndEngine�Ŏg�p�����̂ŊJ�����ŌĂԕK�v�͂Ȃ�
	*/
	void AllReleaseTexture();

	/**
	* @brief �e�N�X�`���̓ǂݍ��݊֐�
	* �w�肳�ꂽ�p�X�̃e�N�X�`����ǂݍ��݁A�J�e�S���[�ɓo�^����
	* @return �ǂݍ��݌���(������true)
	* @param[in] file_name_�ǂݍ��ރe�N�X�`����(�p�X����)
	* @param[in] key_name_ �o�^�������e�N�X�`���̖��O
	*/
	bool LoadTexture(std::string file_name_, std::string key_name_);

	/**
	* @brief �e�N�X�`���f�[�^�̎擾�֐�
	* �w�肳�ꂽ�J�e�S���[�̃e�N�X�`���f�[�^���擾����
	* @return �e�N�X�`���f�[�^(�擾���s����nullptr)
	* @param[in] key_name �擾�������e�N�X�`���̖��O
	*/
	/* �e�N�X�`����ID�œo�^����Ƃ��Ɏg�p���Ă����� */
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

	// �R�s�[�R���X�g���N�^�̋֎~
	Texture(const Texture&);
	Texture& operator = (const Texture&) = delete;
	// ���[�u�R���X�g���N�^�̋֎~
	Texture& operator = (const Texture&&) = delete;

private:
	//bool IsRangeCheck(int texture_id_);

	/**
	* @brief TextureList�̃L�[�`�F�b�N�֐�
	* �����Ŏw�肳�ꂽ�L�[���o�^����Ă��邩���m�F���܂�
	* return �o�^����Ă����ꍇ��true�A���o�^�̏ꍇ��false
	* @param[in] key_name�@�`�F�b�N�������L�[
	*/
	bool HasKeyName(std::string key_name_);

private:
	static Texture* p_TextureInstance;	// Texture�̃C���X�^���X

	std::map<std::string, TextureDataAndSize*> m_texture_list;	// Texture�\���̕ۑ��p�|�C���^�z��

};

#endif