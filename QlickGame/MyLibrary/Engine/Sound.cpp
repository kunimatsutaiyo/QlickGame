#include "Sound.h"
#include "Window.h"

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxguid.lib")
// mmio�ŕK�v
#pragma comment(lib,"winmm.lib")

Sound* Sound::p_instance = nullptr;
LPDIRECTSOUND8 Sound::m_sound_interface = nullptr;

bool Sound::InitSound()
{
	// DirectSound�C���^�[�t�F�C�X���쐬����
	/*
		�������F�T�E���h�f�o�C�X������GUID
			NULL						�F�f�t�H���g�̃f�o�C�X
			DSDEVID_DefaultPlayback		�F�f�t�H���g�̃f�o�C�X
			DSDEVID_DefaultVoicePlayback�F�f�t�H���g�̉����Đ��f�o�C�X

		�������F�쐬���ꂽ�C���^�[�t�F�C�X��ۑ�����ϐ��|�C���^

		��O�����FNULL�Œ�
	*/
	if (FAILED(DirectSoundCreate8(
		NULL,				// GUID
		&m_sound_interface,	// ��������DirectSound�ۑ���
		NULL				// NULL�Œ�
	)))
	{
		return false;
	}

	// �������x���̐ݒ�
	/*
		�W���������x��(DSSCL_NORMAL)
			�����̐ݒ�͌Œ�Ȃ̂ŁA�v���O�������ł͕ύX�s��
			���L�̃v���C�}���o�b�t�@�g�p����
				�E22KHz
				�E�X�e���I
				�E8�r�b�g�T���v�����O

		�D�拦�����x��(DSSCL_PRIORITY)
			�W���������x�������v���C�}���o�b�t�@�ɑ΂��Đݒ�ł��鍀�ڂ�������
			�T���v�����O���[�g�Ɛ[�x�r�b�g���A�v�����Őݒ�\

		�������ݗD�惌�x��(DSSCL_WRITEPRIMARY)
			�v���C�}���o�b�t�@�ɒ��ڃA�N�Z�X�ł���悤�ɂȂ邪�A
			�Z�J���_���o�b�t�@���g�p���邱�Ƃ��ł��Ȃ��Ȃ�
			���T�E���h�ɑ΂��Ă̒m���ƋZ�p���K�v�ɂȂ��Ă���
	*/
	if (FAILED(m_sound_interface->SetCooperativeLevel(
		FindWindowA(WINDOW_CLASS_NAME, nullptr),
		DSSCL_NORMAL
	)))
	{
		return false;
	}

	return true;
}

bool Sound::LoadSound(std::string file_name_, std::string file_list_name_)
{
	// �Z�J���_���o�b�t�@�̍쐬
	/*
		�쐬�̎菇
		1.�T�E���h�t�@�C���̓ǂݍ���
			�Z�J���_���o�b�t�@�̖����͕ۑ����ꂽ�T�E���h�f�[�^���g���ĉ���炷
			���̂��߂ɁA�o�b�t�@�ɃT�E���h�f�[�^��ۑ�������

			���T�E���h�t�@�C���̓ǂݍ��݂�DirectSound�ł̓T�|�[�g���Ă��Ȃ�
			�@�J�����Ńt�@�C����ǂݍ��܂Ȃ���΂Ȃ�Ȃ�
			���o�b�t�@�Ƃ��Ďg�p�ł���̂�WAV�t�@�C����PCM�`��
			�@�ǂݍ��ލۂ�WAV�t�@�C�����AMP3�AOGG���f�R�[�h����PCM�`���ɂ���K�v������

		2.�o�b�t�@�쐬

		3.�T�E���h�f�[�^�̔��f
	*/

	// ���łɓǂݍ���ł��邩���ׂ�
	// �ǂݍ��܂�Ă���Ȃ�false�A�ǂݍ��܂�Ă��Ȃ��Ȃ�treu
	if (HasKeyName(file_list_name_) == true)
	{
		return false;
	}

	// WAV�t�@�C���ǂݍ���
	WavData wav_data;
	if (LoadWavFile(file_name_, &wav_data) == false)
	{
		return false;
	}

	// �o�b�t�@�̍쐬�@
	/*
		�o�b�t�@�̍쐬��DSBUFFERDESC�\���̂Őݒ���s��
		DirectSound�̃o�b�t�@�̏��ݒ�p�\����
			�EdwSize�F�\���̂̃T�C�Y
			�EdwFlags�F�T�E���h�o�b�t�@�̐ݒ�t���O�őg�ݍ��킹�Ďg�p����
				���ꕔ
				DSBCAPS_CTRLPAN			�F���������E�Ɉړ��ł���
				DSBCAPS_CTRLVOLUME		�F�{�����[����ύX�ł���
				DSBCAPS_CTRLFREQUENCY	�F���g����ύX�ł���
			�EdwBufferBytes�F�쐬����o�b�t�@�̃T�C�Y
			�EdwReserved�F�\��0�Œ�
			�ElpwfxFormat�F�g�`�t�H�[�}�b�g
			�Eguid3DAlgorithm�FDirectSound3D�̃n�[�h�E�F�A�V���~�����[�V�����Ŏg��
				�X�s�[�J�[���z���A���S���Y��
				2D�T�E���h�̏ꍇ��DS3DALG_DEFAULT�Ŗ��Ȃ�


			�|�C���g�ƂȂ郁���o
				dwFlags
					�쐬���ꂽ�o�b�t�@���ύX�ł��鍀�ڂ̎w�������
				dwBufferBytes
					�o�b�t�@�̃T�C�Y
					�T�E���h�f�[�^�̃T�C�Y���w��A�T�E���h�t�@�C���̓ǂݍ��݂Ŏ擾�\
				lpwfxFormat
					WAV�t�H�[�}�b�g���
					�T�E���h�t�@�C���̓ǂݍ��݂Ŏ擾�\
					Windows��WAV�t�@�C������舵��mmio�Ƃ���API�����邪�A
					����API�ŗp�ӂ���Ă���WAVEFORMATEX�\���̂��w�肷��
					WAVEFORMATEX�\���̂̕ϐ������ׂĊJ�����Őݒ肵�Ă��L���Ȃ���Ȃ�
	*/
	DSBUFFERDESC dsbd;
	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;	// DSBCAPS_CTRLPAN�F�p���̕ύX | DSBCAPS_CTRLVOLUME�F�{�����[���̕ύX
	dsbd.dwBufferBytes = wav_data.size;
	dsbd.guid3DAlgorithm = DS3DALG_DEFAULT;
	dsbd.lpwfxFormat = &wav_data.wav_format;

	// �o�b�t�@�̍쐬�A
	/*
		IDirectSoundBuffer��CreateSoundBuffer�֐��ō쐬����
		DSBUFFERDESC�̐ݒ�ŃT�E���h�o�b�t�@���쐬
			�������F�쐬����o�b�t�@���
			�������F�쐬���ꂽ�o�b�t�@�̕ۑ���
			��O�����FNULL�Œ�
	*/
	if (FAILED(m_sound_interface->CreateSoundBuffer(
		&dsbd,									// �o�b�t�@���
		&m_sound_buffer_list[file_list_name_],	// �쐬���ꂽ�o�b�t�@�̕ۑ���
		NULL
	)))
	{
		delete[] wav_data.sound_buffer;
		return false;
	}

	// �T�E���h�f�[�^�̔��f
	/*
		�o�b�t�@�ɃT�E���h�f�[�^�̔��f������
		�o�b�t�@�̔��f��IDirectSoundBuffer��Lock�֐��ŃA�N�Z�X���𓾂Ă���
		�f�[�^�𔽉f���AUnlock�֐��ŃA�N�Z�X������������
		������2�̊֐��̓Z�b�g�Ƃ��Ďg������
	*/
	// �g�`�f�[�^���������ނ��߂̃o�b�t�@�ƃT�C�Y
	void* buffer;
	DWORD buffer_size;

	// Lock
	/*
		�w�肵���͈͂Ńo�b�t�@�����b�N����
		�o�b�t�@�̃T�C�Y�ɂ���āA���b�N����o�b�t�@��2�̃p�[�g�ɕ������邪
		��܈����A��Z������NULL���w�肷��Ε������邱�Ƃ͂Ȃ�

		�������F�o�b�t�@�̃I�t�Z�b�g�w��
		�������F�o�b�t�@�̃T�C�Y
		��O�����F���b�N���ꂽ�o�b�t�@�p�[�g1�̕ۑ���ϐ�
		��l�����F�o�b�t�@�p�[�g1�̃T�C�Y
		��܈����F�o�b�t�@�p�[�g2�̃T�C�Y
		��Z�����F���b�N�̃I�v�V�����ݒ�
			�EDSBLOCK_FROMWRITECURSOR
				�J�����g�|�W�V�������烍�b�N����
			�EDSBLOCK_ENTIREBUFFER
				�o�b�t�@�S�̂����b�N����
				��dwBytes�͖��������
	*/
	if (FAILED(m_sound_buffer_list[file_list_name_]->Lock(
		0,				// �I�t�Z�b�g
		wav_data.size,	// ���b�N����o�b�t�@�T�C�Y
		&buffer,		// ���b�N���ꂽ�o�b�t�@�p�[�g1�̕ۑ���
		&buffer_size,	// ���b�N���ꂽ�o�b�t�@�p�[�g1�̃T�C�Y
		NULL,			// ���b�N���ꂽ�o�b�t�@�p�[�g2�̕ۑ���
		NULL,			// ���b�N���ꂽ�o�b�t�@�p�[�g2�̃T�C�Y
		0				// ���b�N�I�v�V����
	)))
	{
		delete[] wav_data.sound_buffer;
		return false;
	}

	// �o�b�t�@�Ƀf�[�^�𔽉f
	memcpy(buffer, wav_data.sound_buffer, buffer_size);

	// Unlock
	/*
		���b�N���ꂽ�o�b�t�@���A�����b�N����

		�������F�A�����b�N����o�b�t�@�p�[�g1
		�������F�o�b�t�@�p�[�g1�̃T�C�Y
		��O�����F�A�����b�N����o�b�t�@�p�[�g2
		��l�����F�o�b�t�@�p�[�g2�̃T�C�Y
	*/
	m_sound_buffer_list[file_list_name_]->Unlock(
		&buffer,		// �A�����b�N����o�b�t�@�p�[�g1
		buffer_size,	// �p�[�g1�̃o�b�t�@�T�C�Y
		NULL,			// �A�����b�N����o�b�t�@�p�[�g2
		NULL			// �p�[�g2�̃o�b�t�@�T�C�Y
	);

	// �R�s�[���I������̂ŃT�E���h�f�[�^�����
	delete[] wav_data.sound_buffer;

	return true;
}

void Sound::PlaySoundFile(std::string file_list_name_, bool is_loop_, int volume_)
{
	if (m_sound_buffer_list[file_list_name_] == nullptr)
	{
		return;
	}

	// ? ���ɕω����Ȃ� ?
	//m_sound_buffer_list[file_list_name_]->SetVolume(volume_);

	// �Đ�����
	/*
		�������F�\������@���0
		�������F�D�揇��
				�@0(�Œ�)�`0xffffffff(�ő�)
		��O�����F�o�b�t�@�̍Đ����@�̎w��
				�@�t���O�̎�ނ���������A�g�ݍ��킹�Ďg��

				 DSBPLAY_LOOPING�F�Đ����ꂽ�T�E���h�Ƀ��[�v�ݒ��ǉ�����
	*/
	int loop_bit = is_loop_ == true ? 1 : 0;
	m_sound_buffer_list[file_list_name_]->Play(
		0,							// ���0
		0,							// �D�揇��
		DSBPLAY_LOOPING & loop_bit	// ���[�v�ݒ�
	);
}

void Sound::StopSoundFile(std::string file_list_name_)
{
	if (m_sound_buffer_list[file_list_name_] == nullptr)
	{
		return;
	}

	// ��~
	m_sound_buffer_list[file_list_name_]->Stop();
	// �Đ��ʒu��擪�ɖ߂�
	/*
		Stop�֐��͍Đ����~�߂邾���Ȃ̂�
		�o�b�t�@�̈ʒu���ς��Ȃ�

		�����ŏ�����Đ��������Ƃ��Ɏg��
		�����F0���w�肵�Ă��
	*/
	m_sound_buffer_list[file_list_name_]->SetCurrentPosition(NULL);
}

void Sound::ReleaseSound()
{
	// �Z�J���_���o�b�t�@�̉��

	for (auto& list : m_sound_buffer_list)
	{
		if (list.second != nullptr)
		{
			list.second->Stop();
			list.second->Release();
			list.second = nullptr;
		}
	}
	m_sound_buffer_list.clear();

	// DirectSound�C���^�[�t�F�[�X�̉��
	if (m_sound_interface != nullptr)
	{
		m_sound_interface->Release();
		m_sound_interface = nullptr;
	}
}

bool Sound::LoadWavFile(const std::string file_name_, WavData* out_wav_data_)
{
	// WAV�t�@�C���̑�����@
	/*
		WAV�t�@�C���̑����WinAPI��mmio(Multi Media IO)���g�p����
		mmio��RIFF�`���̃t�@�C���𑀍삷��֐����p�ӂ���Ă���

		�t�@�C���̓ǂݏ����Ŏg�p����t�@�C������̈ꗗ
		1.�t�@�C���̃I�[�v���ƃN���[�Y
		2.�`�����N�ւ̐i���Ƒޏo
		3.�`�����N�̓ǂݍ���
		4.�`�����N�̏�������

		WAV�t�@�C���̓ǂݍ��ݎ菇
		1.�t�@�C�����J��
		2.RIFF�`�����N�֐i������
		3.�`�����N�֐i������
		4.�`�����N�œǂݏ���������
		5.�`�����N����ޏo����
		6.3�`5���J��Ԃ�
		7.�t�@�C�������
		�@2�`6�Ŏ��s�̎����s��
	*/

	// Windows�}���`���f�B�AAPI�̃n���h��
	HMMIO mmio_handle = NULL;

	// �`�����N���
	MMCKINFO ck_info;
	// RIFF�`�����N�p
	MMCKINFO riffck_info;

	// WAV�t�@�C�����̉��T���v���̃T�C�Y
	DWORD dw_wav_size = 0;

	// �t�@�C���I�[�v��
	/*
		WAV�t�@�C�����ʏ�̃t�@�C���Ɠ��l�ŃI�[�v������
		�t�@�C���ɑ΂��鏈�����s���A�p���ς񂾂�N���[�Y����

		mmioOpen�֐��͈����Ńt�@�C�����ƃ��[�h�̎w����s��
		���ʁFMMIO�̏����擾
			�@HMMIO���擾(MMIO�̃n���h��)

		�������F�J���\��̃t�@�C����
		�������FMMIOINFO�̃|�C���^
				�@DirectSound�̓ǂݍ��݂ł͕s�v�Ȃ̂�NULL
		��O�����F�J������̃t�@�C���̈������̐ݒ�
			MMIO_READ�F�ǂݍ��ݐ�p
			MMIO_WRITE�F�������ݐ�p
			MMIO_READWRITE�F�ǂݏ�����
			�����ɂ��I�v�V�����ݒ�͂���
	*/
	mmio_handle = mmioOpenA(
		(char*)file_name_.c_str(),	// �t�@�C����
		NULL,						// MMIO���
		MMIO_READ					// ���[�h
	);
	if (mmio_handle == NULL)
	{
		return false;
	}

	// data�t�H�[�}�b�g�ւ̐i���ݒ�
	/*
		mmioFOURCC�̓}�N���ɂȂ��Ă���
		������4�̕������w��ł���
		���̕������r�b�g�V�t�g�ł��炵�āA
		4�o�C�g��1�������������z�u���ꂽ�������쐬���Ă���

		���̃}�N���ɂ���āAWAV�t�H�[�}�b�g��
		ckid��fccType�̕������ݒ肳�ꂽ�������ȒP�ɍ쐬�ł���悤�ɂȂ�
	*/
	// WAV�t�@�C���̐擪�`�����N�ł���RIFF�`�����N�ɐi������
	// RIFF�`�����N�ɐi�����邽�߂�fccType��WAVE��ݒ肷��
	riffck_info.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// �`�����N�֐i���܂��͑ޏo
	/*
		WAV�t�@�C���̓`�����N���ƂɃf�[�^��������Ă���̂ŃA�N�Z�X����`�����N�����߂�
		������u�i���v�Ƃ���
		�ʂ̃`�����N�ɃA�N�Z�X����Ƃ��͑I�𒆂̃`�����N���L�����Z�����Ȃ���΂Ȃ�Ȃ�
		������u�ޏo�v�Ƃ���

		�`�����N�ւ̐i���FmmioDescend�֐�
			�w�肳�ꂽ�`�����N�ɐi������

			�������F�i���Ώۂ̃`�����N������MMIO�n���h��
			�������F�i�������`�����N���
			��O�����F�e�`�����N
			��l�����F�i������`�����N�̎��

			���������ɂ���
				���̈����͓��͂Əo�͂����˂Ă���
				mmioDescend�֐��͑�������MMCKINFO�́uckid�v�ufccType�v��
				��l�����̏��̎�ނ̑g�ݍ��킹�Ői������`�����N��T���Ă���
				(�g�ݍ��킹��yttm�̃T�E���h��)

		�`�����N����ޏo�FmmioAscend�֐�
			�`�����N��ޏo����(��̊K�w�ɖ߂�)

			�������F�ޏo�Ώۂ�MMIO�n���h��
			�������F�ޏo��̃`�����N���
			��O�����F�ޏo��̃`�����N���

			�n���h���Ɛi�����̃`�����N���w�肷��΃`�����N����ޏo�ł���
	*/
	// RIFF�`�����N�ɐi������
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,
		&riffck_info,
		NULL,
		MMIO_FINDRIFF
	))
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// �ŏ���WAV�t�@�C���̃T���v�����O���[�g�Ȃǂ�ێ����Ă���ufmt�`�����N�v�ɐi������
	// �i����̃`�����N��"fmt"�Ƃ��Đݒ肷��
	ck_info.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,
		&ck_info,
		&riffck_info,
		MMIO_FINDCHUNK
	))
	{
		// fmt�`�����N���Ȃ�
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// �`�����N�̓ǂݍ���
	/*
		�`�����N�f�[�^�̓ǂݍ��݂͐i�����̃`�����N�ɑ΂��Ă̂ݍs����
		��mmioOpen�œǂݏ����̐ݒ�����Ă��邪�A���̐ݒ�ɔ������������ł��Ȃ�
		�@(�ǂݍ��ݐ�p�Ńt�@�C�����J���Ă���ꍇ�́A�������݂��ł��Ȃ�)

		�������F�擾�Ώۂ�MMIO�n���h��
		�������F�`�����N��ǂݍ��񂾃f�[�^��ۑ�����o�b�t�@
		��O�����F�o�b�t�@�̃T�C�Y
	*/
	// fmt�`�����N��ǂݍ���
	LONG read_size = mmioRead(
		mmio_handle,						// �n���h��
		(HPSTR)&out_wav_data_->wav_format,	// �ǂݍ��ݐ�̃o�b�t�@
		sizeof(out_wav_data_->wav_format)	// �o�b�t�@�̃T�C�Y
	);
	if (read_size != sizeof(out_wav_data_->wav_format))
	{
		/*
			�T�C�Y����v���Ȃ��̂ŃG���[
				��O�����œǂݍ��񂾃o�b�t�@�̃T�C�Y���߂�l�Ŋm�ۂł��Ă��邪�A
				���ꂪ�\���̂ƃT�C�Y���Ⴄ�Ƃ��A��肪����Ƃ�������
		*/
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// �t�H�[�}�b�g�̃`�F�b�N
	if (out_wav_data_->wav_format.wFormatTag != WAVE_FORMAT_PCM)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// �ǂݍ��݂����������̂őޏo����
	if (MMSYSERR_NOERROR != mmioAscend(
		mmio_handle,
		&ck_info,
		0
	))
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// ���f�[�^��ێ����Ă���data�`�����N�ɐi������
	ck_info.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (MMSYSERR_NOERROR != mmioDescend(
		mmio_handle,
		&ck_info,
		&riffck_info,
		MMIO_FINDCHUNK
	))
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		return false;
	}

	// �T�C�Y��ۑ�
	out_wav_data_->size = ck_info.cksize;

	// ���f�[�^��ǂݍ���
	out_wav_data_->sound_buffer = new char[ck_info.cksize];
	read_size = mmioRead(
		mmio_handle,
		(HPSTR)out_wav_data_->sound_buffer,
		ck_info.cksize);
	if (read_size != ck_info.cksize)
	{
		mmioClose(mmio_handle, MMIO_FHOPEN);
		delete[] out_wav_data_->sound_buffer;
		return false;
	}



	// �`�����N�̏�������
	/*
		�`�����N�f�[�^�ɏ������ނ̂̓`�����N�ɐi�����Ă��Ȃ��Ă��\
		��mmioOpen�œǂݏ����̐ݒ�����Ă��邪�A���̐ݒ�ɔ������������ł��Ȃ�
		�@(�ǂݍ��ݐ�p�Ńt�@�C�����J���Ă���ꍇ�́A�������݂��ł��Ȃ�)

		 �������F�擾�Ώۂ�MMIO�n���h��
		 �������F�t�@�C���ɏ������ރo�b�t�@
		 ��O�����F�o�b�t�@�̃T�C�Y
	*/
	//mmiowrite(
	//	mmio_handle,	// �n���h��
	//	buffer,			// �������݃o�b�t�@
	//	buffer_size		// �o�b�t�@�̃T�C�Y
	//);

	// �t�@�C���N���[�Y
	/*
		�t�@�C�������

		�������F����\���mmio�n���h��
		�������F�I�v�V����
				�@MMIO_FHOPEN
					��������MMIO�t�@�C���ȊO���w�肳�ꂽ�ꍇ�͖������āA
					MMIO�t�@�C�����������悤�ɂ���
	*/
	mmioClose(
		mmio_handle,	// �N���[�Y�Ώۂ̃n���h��
		MMIO_FHOPEN		// �I�v�V����(�ݒ�Ȃ���0)
	);

	return true;
}

bool Sound::HasKeyName(std::string file_list_name_)
{
	auto it = m_sound_buffer_list.find(file_list_name_);
	if (it != m_sound_buffer_list.end())
	{
		return true;
	}
	return false;
}