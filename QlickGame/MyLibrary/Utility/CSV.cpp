#include "CSV.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

bool CSV::LoadData(std::string str_, std::string key_name_)
{
	if (FileHasKeyName(key_name_) == true)
	{
		return false;
	}
	m_list_of_loaded_files[key_name_] = str_;

	std::ifstream ifs(m_list_of_loaded_files[key_name_].c_str(), std::ios_base::in);

	// �t�@�C�����J���Ƃ��Ɏ��s�����Ƃ�
	if (ifs.fail())
	{
		//std::cerr << "Failed to open file." << std::endl;
		return -1;
	}

	std::string str;
	// getline()��1�s���ǂݍ���
	while (getline(ifs, str))
	{
		// �t�@�C������string�^�Ɋi�[
		ifs >> str;

		std::vector<std::string> strvec = Split(str, ',');

		// �����񂪋󂩂ǂ�������
		/*�@empty
			��܂��͑O�ɋ󔒂���	�Ftrue(1)
			�����񂠂�				�Ffalse(0)
		*/
		if (strvec.empty() == false)
		{
			// csv�̕ύX�_
			std::string key_name = strvec[0];
			m_object_list[key_name].push_back(strvec);



			//std::string key_name = strvec[0];
			//if (character_param.find(key_name) == character_param.end())
			//{
			//	// �L�[���ݒ肳��Ă���ꍇ
			//	//�v�f��������������Ă���push_back
			//	character_param.erase(key_name);
			//	for (size_t i = 1; i < strvec.size(); ++i)
			//	{
			//		character_param[key_name].push_back(strvec[i]);
			//	}
			//}
			//else {
			//	// �L�[���ݒ肳��Ă��Ȃ��ꍇ
			//	for (size_t i = 1; i < strvec.size(); ++i)
			//	{
			//		character_param[key_name].push_back(strvec[i]);
			//	}
			//}
		}
	}

	// close�͎g��Ȃ��Ă�����ɕ��Ă����
	// close���Ă΂Ȃ��Ă������̂�ifstream�������ĂɌĂ�ł���Ă��邩��
	ifs.close();

	return false;
}

//bool CSV::WriteData(std::string str_, std::vector<std::string> strvec_)
//{
//	//std::ofstream ofs(str_, std::ios_base::out);
//
//	//// �t�@�C�����J���Ƃ��Ɏ��s�����Ƃ�
//	//if (ofs.fail())
//	//{
//	//	//std::cerr << "Failed to open file." << std::endl;
//	//	return -1;
//	//}
//
//	//std::vector<int> score_table;
//	//// �\�[�g����O�ɕ�����𐔎��ɂ��A�S�ĕb�P�ʂɂ���
//	//for (int i = 0; i < strvec_.size(); i++)
//	//{
//	//	std::vector<int> num;
//	//	for (int j = 0; j < strvec_[i].size(); j++)
//	//	{
//	//		if (strvec_[i][j] == ':')
//	//		{
//	//			continue;
//	//		}
//	//		else {
//	//			// ASCII�R�[�h�\��48�`57�͕�����'0'�`'9'�ɑΉ����Ă���̂ł���𗘗p����
//	//			num.push_back(strvec_[i][j] - '0');
//	//		}
//	//	}
//	//	// �����L���O�̕\�������P�ʂȂ̂ŁA�b�P�ʂɒ����Ċi�[
//	//	score_table.push_back((num[0] * 600) + (num[1] * 60) + (num[2] * 10) + (num[3]));
//	//}
//
//	///*
//	//	�\�[�g����
//	//	���בւ��邱�Ƃ��ł���̂́A�ʏ�̔z���vector�^�Ȃǂ�
//	//	�����_���A�N�Z�X�C�e���[�^���g����R���e�i�̒��g
//	//	list�Ȃǂ̓����_���A�N�Z�X�ł��Ȃ��̂�sort�͂ł��Ȃ�
//	//	list�ɂ͎��̂�list::sort�Ƃ����֐����p�ӂ���Ă���
//	//*/
//	//std::sort(score_table.begin(), score_table.end());
//
//	//// 0����Ԍ���
//	//int count = 0;
//	//for (int i = 0; i < score_table.size(); i++)
//	//{
//	//	if (score_table[i] == 0)
//	//	{
//	//		score_table.erase(score_table.begin() + i);
//	//		score_table.push_back(0);
//	//		i--;
//	//	}
//	//	count++;
//	//	if (count >= score_table.size())
//	//	{
//	//		break;
//	//	}
//	//}
//
//	//// �����L���O�̕\�����b�P�ʂȂ̂ŁA���P�ʂɖ߂�
//	//std::vector<std::string> ranking;
//	//ranking = m_math.FixTheMinute(score_table);
//
//	//ofs << "RANKING" << std::endl;
//	//ofs << "Ranking" << ','
//	//	<< ranking[static_cast<int>(RANK::FIRST)] << ','
//	//	<< ranking[static_cast<int>(RANK::SECOND)] << ','
//	//	<< ranking[static_cast<int>(RANK::THIRD)]
//	//	<< std::endl;
//
//	//ofs.close();
//}

int CSV::CreateFile()
{
	const char* fileName = "test.txt";

	std::ofstream ofs(fileName);
	if (!ofs)
	{
		std::cout << "�t�@�C�����J���܂���ł����B" << std::endl;
		std::cin.get();
		return 0;
	}

	ofs << "player\n" << std::endl;		// �薼�Ƃ�����������
	std::cout << fileName << "�ɏ������݂܂����B" << std::endl;

	std::cin.get();
}

std::string CSV::StringTrim(const std::string& str_, const char* trim_character_list_)
{
	std::string result;
	/*
		find_fist_not_of�F�w�肵�������Z�b�g�ȊO�̕����������ŏ��̈ʒu���擾
		find_last_not_of�F�w�肵�������Z�b�g�ȊO�̕����������Ō�̈ʒu���擾
	*/
	std::string::size_type left = str_.find_first_not_of(trim_character_list_);
	if (left != std::string::npos)
	{
		std::string::size_type right = str_.find_last_not_of(trim_character_list_);

		/*
			substr�F�������̐��l�̈ʒu����
					�������̐��l�̗v�f���̕������Ԃ�
			�������F������̈ʒu
			�������F�������Ԃ��v�f��
		*/
		result = str_.substr(left, right - left + 1);
	}

	return result;
}

bool CSV::CheckLanuage(std::string str_)
{
	// ��v���Ă���Ȃ�\��
	if (str_ == "enemy")
	{
		std::cout << "�G�l�~�[" << std::endl;
		return true;
	}
	if (str_ == "����������")
	{
		std::cout << "����������" << std::endl;
		return true;
	}

	return false;
}

std::vector<std::string> CSV::Split(std::string& input_, char delimiter_)
{
	// istringstream�̓t�H�[�}�b�g�����܂��Ă��邱�Ƃ��O��炵��
	std::istringstream stream(input_);
	std::string field;
	std::vector<std::string> result;
	/** getline�֐�
		��1�����F�ǂ�����f�[�^��ǂݎ��̂����K�肷��
		��2�����F�f�[�^�̊i�[��
		��3�����F�����炭��3�����ɓ��ꂽ�������Ƃɋ�؂��ēǂݍ���(�����炭�󔒂ł���)
	*/
	while (getline(stream, field, delimiter_))
	{
		result.push_back(field);
	}

	return result;
}

bool CSV::FileHasKeyName(std::string key_name_)
{
	auto it = m_list_of_loaded_files.find(key_name_);
	if (it != m_list_of_loaded_files.end())
	{
		return true;
	}

	return false;
}

bool CSV::DataHasKeyName(std::string key_name_)
{
	auto it = m_object_list.find(key_name_);
	if (it != m_object_list.end())
	{
		return true;
	}

	return false;
}

//std::string CheckString(std::string str_)
//{
//	/*
//		try		�F��O�����������鏈��������
//		catch	�F��O�����������ꍇ�ɍs��������������
//				��O�N���X
//				std::invalid_argument	�F�����Ȉ���
//				std::out_of_range		�F�͈͊O�̓Y����
//	*/
//	try {
//		// float�^�ɕϊ����Ċi�[
//		result = std::stof(str_);
//	}
//	catch (const std::invalid_argument&) {
//		//std::cout << "Error: The string '" << str_ << "' is not float." << std::endl;
//
//
//
//		return result;
//	}
//	catch (const std::out_of_range&) {
//		//std::cout << "Error: The string '" << str_ << "' is float but out of range." << std::endl;
//
//
//
//		return result;
//	}
//	//std::cout << "'" << str_ << "' -> " << result << std::endl;
//}

bool CSV::CheckStringOrNumber(std::string str_)
{
#pragma region �������Ffloat�ւ̕ϊ��͂ł�����float�^�Ƃ��ĔF���͂���Ȃ�����
	// �������@
	//if (std::all_of(str_.cbegin(), str_.cend(), isdigit))
	//{
	//	// �`�F�b�N���ʂ�Ε\��(int�^�Fstoi)
	//	//std::cout << strtod(str_.c_str(), NULL) << std::endl;
	//	std::cout << std::stof(str_) << std::endl;

	//	return true;
	//}
	//std::cout << "Not float , " << str_ << std::endl;


	// �������A
	/*using namespace std;
	if (std::regex_match(str_, std::regex(("((\+|-)?[[:digit:]]+)(\.(([[:digit:]]+)?))?"))))
	{
		std::cout << std::stof(str_) << std::endl;
		return true;
	}
	std::cout << "Not float , " << str_ << std::endl;*/
#pragma endregion

	float result = 0.0f;

	/*
		try		�F��O�����������鏈��������
		catch	�F��O�����������ꍇ�ɍs��������������
				��O�N���X
				std::invalid_argument	�F�����Ȉ���
				std::out_of_range		�F�͈͊O�̓Y����
	*/
	try {
		// float�^�ɕϊ����Ċi�[
		result = std::stof(str_);
	}
	catch (const std::invalid_argument&) {
		//std::cout << "Error: The string '" << str_ << "' is not float" << std::endl;
		//std::cout << str_ << std::endl;

		std::istringstream stream(str_);
		std::string field;
		while (getline(stream, field))
		{
			m_name.push_back(str_);
		}

		return result;
	}
	catch (const std::out_of_range&) {
		//std::cout << "Error: The string '" << str_ << "' is float but out of range." << std::endl;
		//std::cout << str_ << std::endl;

		std::istringstream stream(str_);
		std::string field;
		while (getline(stream, field))
		{
			m_name.push_back(field);
		}

		return result;
	}
	//std::cout << "'" << str_ << "' -> " << result << std::endl;
	//std::cout << result << std::endl;

	m_param.push_back(result);


	/*�@all_of	�F�͈͓��̗v�f�̂��ׂĂ��w�肳�ꂽ�����𖞂����Ƃ�ture
	�������F�͈͂̊J�n�ʒu�������C�e���[�^
	�������F�͈̖͂��[�������C�e���[�^
	��O�����F��������Ɏg���֐�
			�@�w�肵���C�e���[�^�̒l���Ƃ���bool�^�Ƃ��Ė߂�l��]���ł�����̂Ȃ�֐��ł��\���̂ł���
			  ��F	isdigit���������ǂ���
					isalpha���A���t�@�x�b�g���ǂ���
	*/
	/*if (std::all_of(str_.cbegin(), str_.cend(), isdigit))
	{
		result = std::stof(str_);

		std::cout << result << std::endl;
		return true;
	}
	std::cout << "Not float" << result << std::endl;
	return false;*/

	/*if (std::isdigit(result))
	{
		std::cout << "'" << str_ << "' -> " << result << std::endl;
	}
	else {
		return false;
	}
	return true;*/

	return result;
}

void CSV::PrintCSV()
{
	for (int i = 0; i < m_name.size(); i++)
	{
		std::cout << m_name[i] << std::endl;

		for (int i = 0; i < m_param.size(); i++)
		{
			std::cout << m_param[i] << std::endl;
		}
	}
}