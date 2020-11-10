#ifndef CSV_H_
#define CSV_H_

#include "Singleton.h"
#include "Math.h"

#include <string>
#include <vector>
#include <map>

enum class RANK
{
	FIRST,
	SECOND,
	THIRD,

	MAX
};

class CSV :public Singleton<CSV>
{
private:
	friend Singleton<CSV>;

public:
#pragma region �R�����g�̏�������
	/**
	 * @fn
	 * �����Ɋ֐��̐���������
	 * @brief �v�����
	 * @param (������) �����̐���
	 * @param (������) �����̐���
	 * @return �߂�l�̐���
	 * @sa �Q�Ƃ��ׂ��֐��������΃����N���\���
	 * @detail �ڍׂȐ���
	 */
#pragma endregion

	 /**
	 *	@brief �e�L�X�g�t�@�C�����쐬����֐�
	 *	@return	?????
	 */
	int CreateFile();

	/**
	*	@brief strin�^�̕�����̋󔒂������֐�
	*	@param[in] str_					string�^�̕����������
	*	@param[in] trim_character_list_	������������
	*	@return	string�^��Ԃ�
	*/
	std::string StringTrim(const std::string& str_, const char* trim_character_list_);

	/**
	*	@brief �����񂪐������ǂ����`�F�b�N����(float)�֐�
	*	@param[in] str_			string�^�̕����������
	*	@param[in] is_float_	?????
	*	@return	bool�^��Ԃ�
	*/
	//bool CheckString(std::string str_);

	/**
	*	@brief �����񂪐������ǂ����`�F�b�N����(float)�֐�
	*	@param[in] str_			string�^�̕����������
	*	@param[in] is_float_	?????
	*	@return	bool�^��Ԃ�
	*/
	bool CheckStringOrNumber(std::string str_);

	/**
	*	@brief �֐�
	*	@param[in] str_	string�^�̕����������
	*	@return
	*/
	void PrintCSV();

	/**
	*	@brief<Get> �w�肳�ꂽ�L�[�̒��̃p�����[�^�擾����֐�
	*	@param[in] key_ key�̖��O������
	*	@return	�L�[�̒��̒l���Q�ƌ^�ŕԂ��H
	*/
	std::vector<std::vector<std::string>>* GetParam(std::string key_)
	{
		if (DataHasKeyName(key_) == true)
		{
			return &m_object_list[key_];
		}

		///*
		//	�L�[�̌���
		//		mp��fmap�I�u�W�F�N�g�Ƃ����
		//		mp.find(�L�[)�ŁA�L�[��mp�ɐݒ肳��邩�ǂ����𒲂ׂ���
		//		�L�[���ݒ肳��Ă���ꍇ�́A�L�[�ƒl�̃y�A�ւ̃C�e���[�^��Ԃ�
		//		�L�[���ݒ肳��Ă��Ȃ��ꍇ�́Aend()�ւ̃C�e���[�^��Ԃ�
		//*/
		//std::map<std::string, std::vector<std::string>>::iterator itr = character_param.find(key_);
		//if (itr == character_param.end())
		//{
		//	return nullptr;
		//}
		//return &character_param[key_];
	}

	/**
	*	@brief csv(txt)�t�@�C����ǂݍ��ފ֐�
	*	@param[in] file_name_ string�^�̕����������
	*	@param[in] key_name_ string�^�̕����������
	*	@return bool�^��Ԃ�
	*/
	bool LoadData(std::string file_name_, std::string key_name_);

	/**
	*	@brief csv(txt)�t�@�C���ɏ������ފ֐�
	*	@param[in] str_		string�^�̕����������
	*	@param[in] vecf_	vector�^��Ԃ�(float�^���i�[����)
	*	@return bool�^��Ԃ�
	*	�����̓����L���O�����l���Ă��Ȃ��̂ŁA�������ޓ��e�͌��܂��Ă���
	*/
	//bool WriteData(std::string str_, std::vector<float> vecf_);

	/*bool WriteData(std::string str_, std::vector<std::string> strvec_);*/

private:

	/**
	*	@brief �����񂪈�v���Ă��邩���肷��֐�
	*	@param[in] str_ string�^�̕����������
	*	@return	bool�^��Ԃ�
	*/
	bool CheckLanuage(std::string str_);

	/**
	*	@brief �R���}��؂�ŕ�����𕪊�����֐�
	*	@param[in] str_			string�^�̕����������
	*	@param[in] delimiter_	��؂肽������������
	*	@return	vector�^��Ԃ�(string�^���i�[����)
	*/
	// delimiter:(�Ӗ�)��؂蕄��
	std::vector<std::string> Split(std::string& str_, char delimiter_);

	/**
	* @brief FileList�̃L�[�`�F�b�N�֐�
	* �����Ŏw�肳�ꂽ�L�[���o�^����Ă��邩���m�F���܂�
	* return �o�^����Ă����ꍇ��true�A���o�^�̏ꍇ��false
	* @param[in] key_name�@�`�F�b�N�������L�[
	*/
	bool FileHasKeyName(std::string key_name_);

	/**
	* @brief DataList�̃L�[�`�F�b�N�֐�
	* �����Ŏw�肳�ꂽ�L�[���o�^����Ă��邩���m�F���܂�
	* return �o�^����Ă����ꍇ��true�A���o�^�̏ꍇ��false
	* @param[in] key_name�@�`�F�b�N�������L�[
	*/
	bool DataHasKeyName(std::string key_name_);

private:
	CSV() {}
	~CSV() {}

	// �R�s�[�R���X�g���N�^�̋֎~
	CSV(const CSV&);
	CSV& operator=(const CSV&) = delete;

	// ���[�u�R���X�g���N�^�̋֎~
	CSV& operator=(const CSV&&) = delete;

private:
	static CSV* p_CSVInstance;

	std::vector<std::string> m_name;
	std::vector<float> m_param;

	//std::map< std::string, std::vector<std::string> > character_param;
	std::vector<std::vector<std::string>> m_object_data;
	std::map<std::string, std::vector<std::vector<std::string>>> m_object_list;

	std::map<std::string, std::string> m_list_of_loaded_files;

};

#endif