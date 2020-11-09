#ifndef SOUND_H_
#define SOUND_H_

#include "../Utility/Singleton.h"

#include <dsound.h>
#include <string>
#include <map>

struct WavData
{
	WAVEFORMATEX wav_format;
	char* sound_buffer;
	DWORD size;
};

class Sound :public Singleton<Sound>
{
private:
	friend Singleton<Sound>;

public:
	bool InitSound();

	bool LoadSound(std::string file_name_, std::string file_list_name_);

	void PlaySoundFile(std::string file_list_name_, bool is_loop_, int volume_);

	void StopSoundFile(std::string file_list_name_);

	void ReleaseSound();

private:
	bool LoadWavFile(const std::string file_name_, WavData* out_wav_data_);

	/**
	* @brief SoundList�̃L�[�`�F�b�N�֐�
	* �����Ŏw�肳�ꂽ�L�[���o�^����Ă��邩���m�F���܂�
	* return �o�^����Ă����ꍇ��true�A���o�^�̏ꍇ��false
	* @param[in] key_name�@�`�F�b�N�������L�[
	*/
	bool HasKeyName(std::string file_list_name_);

private:
	Sound()
	{
		/*for (size_t i = 0; i < static_cast<size_t>(SOUND_FILE::MAX_SOUND_FILE); i++)
		{
			m_sound_buffer_list[i] = nullptr;
		}*/
	}
	~Sound() {}

	// �R�s�[�R���X�g���N�^�̋֎~
	Sound(const Sound&);
	Sound& operator=(const Sound&) = delete;

	// ���[�u�R���X�g���N�^�̋֎~
	Sound& operator=(const Sound&&) = delete;

private:
	static Sound* p_instance;

	static LPDIRECTSOUND8 m_sound_interface;
	//LPDIRECTSOUNDBUFFER m_sound_buffer_list[static_cast<size_t>(SOUND_FILE::MAX_SOUND_FILE)];

	std::map<std::string, LPDIRECTSOUNDBUFFER> m_sound_buffer_list;

};

#endif