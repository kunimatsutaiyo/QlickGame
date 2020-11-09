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

	// ファイルを開くときに失敗したとき
	if (ifs.fail())
	{
		//std::cerr << "Failed to open file." << std::endl;
		return -1;
	}

	std::string str;
	// getline()で1行ずつ読み込む
	while (getline(ifs, str))
	{
		// ファイルからstring型に格納
		ifs >> str;

		std::vector<std::string> strvec = Split(str, ',');

		// 文字列が空かどうか判定
		/*　empty
			空または前に空白あり	：true(1)
			文字列あり				：false(0)
		*/
		if (strvec.empty() == false)
		{
			// csvの変更点
			std::string key_name = strvec[0];
			m_object_list[key_name].push_back(strvec);



			//std::string key_name = strvec[0];
			//if (character_param.find(key_name) == character_param.end())
			//{
			//	// キーが設定されている場合
			//	//要素をいったん消してからpush_back
			//	character_param.erase(key_name);
			//	for (size_t i = 1; i < strvec.size(); ++i)
			//	{
			//		character_param[key_name].push_back(strvec[i]);
			//	}
			//}
			//else {
			//	// キーが設定されていない場合
			//	for (size_t i = 1; i < strvec.size(); ++i)
			//	{
			//		character_param[key_name].push_back(strvec[i]);
			//	}
			//}
		}
	}

	// closeは使わなくても勝手に閉じてくれる
	// closeを呼ばなくてもいいのはifstreamがかってに呼んでくれているから
	ifs.close();

	return false;
}

bool CSV::WriteData(std::string str_, std::vector<std::string> strvec_)
{
	//std::ofstream ofs(str_, std::ios_base::out);

	//// ファイルを開くときに失敗したとき
	//if (ofs.fail())
	//{
	//	//std::cerr << "Failed to open file." << std::endl;
	//	return -1;
	//}

	//std::vector<int> score_table;
	//// ソートする前に文字列を数字にし、全て秒単位にする
	//for (int i = 0; i < strvec_.size(); i++)
	//{
	//	std::vector<int> num;
	//	for (int j = 0; j < strvec_[i].size(); j++)
	//	{
	//		if (strvec_[i][j] == ':')
	//		{
	//			continue;
	//		}
	//		else {
	//			// ASCIIコード表の48～57は文字の'0'～'9'に対応しているのでこれを利用する
	//			num.push_back(strvec_[i][j] - '0');
	//		}
	//	}
	//	// ランキングの表示が分単位なので、秒単位に直して格納
	//	score_table.push_back((num[0] * 600) + (num[1] * 60) + (num[2] * 10) + (num[3]));
	//}

	///*
	//	ソート処理
	//	並べ替えることができるのは、通常の配列やvector型などの
	//	ランダムアクセスイテレータが使えるコンテナの中身
	//	listなどはランダムアクセスできないのでsortはできない
	//	listには実体にlist::sortという関数が用意されている
	//*/
	//std::sort(score_table.begin(), score_table.end());

	//// 0を一番後ろに
	//int count = 0;
	//for (int i = 0; i < score_table.size(); i++)
	//{
	//	if (score_table[i] == 0)
	//	{
	//		score_table.erase(score_table.begin() + i);
	//		score_table.push_back(0);
	//		i--;
	//	}
	//	count++;
	//	if (count >= score_table.size())
	//	{
	//		break;
	//	}
	//}

	//// ランキングの表示が秒単位なので、分単位に戻す
	//std::vector<std::string> ranking;
	//ranking = m_math.FixTheMinute(score_table);

	//ofs << "RANKING" << std::endl;
	//ofs << "Ranking" << ','
	//	<< ranking[static_cast<int>(RANK::FIRST)] << ','
	//	<< ranking[static_cast<int>(RANK::SECOND)] << ','
	//	<< ranking[static_cast<int>(RANK::THIRD)]
	//	<< std::endl;

	//ofs.close();
}

int CSV::CreateFile()
{
	const char* fileName = "test.txt";

	std::ofstream ofs(fileName);
	if (!ofs)
	{
		std::cout << "ファイルが開けませんでした。" << std::endl;
		std::cin.get();
		return 0;
	}

	ofs << "player\n" << std::endl;		// 題名とかがいいかも
	std::cout << fileName << "に書き込みました。" << std::endl;

	std::cin.get();
}

std::string CSV::StringTrim(const std::string& str_, const char* trim_character_list_)
{
	std::string result;
	/*
		find_fist_not_of：指定した文字セット以外の文字が現れる最初の位置を取得
		find_last_not_of：指定した文字セット以外の文字が現れる最後の位置を取得
	*/
	std::string::size_type left = str_.find_first_not_of(trim_character_list_);
	if (left != std::string::npos)
	{
		std::string::size_type right = str_.find_last_not_of(trim_character_list_);

		/*
			substr：第一引数の数値の位置から
					第二引数の数値の要素分の文字列を返す
			第一引数：文字列の位置
			第二引数：文字列を返す要素数
		*/
		result = str_.substr(left, right - left + 1);
	}

	return result;
}

bool CSV::CheckLanuage(std::string str_)
{
	// 一致しているなら表示
	if (str_ == "enemy")
	{
		std::cout << "エネミー" << std::endl;
		return true;
	}
	if (str_ == "あいうえお")
	{
		std::cout << "あいうえお" << std::endl;
		return true;
	}

	return false;
}

std::vector<std::string> CSV::Split(std::string& input_, char delimiter_)
{
	// istringstreamはフォーマットが決まっていることが前提らしい
	std::istringstream stream(input_);
	std::string field;
	std::vector<std::string> result;
	/** getline関数
		第1引数：どこからデータを読み取るのかを規定する
		第2引数：データの格納先
		第3引数：おそらく第3引数に入れた文字ごとに区切って読み込む(おそらく空白でも可)
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
//		try		：例外が発生しうる処理を書く
//		catch	：例外が発生した場合に行いう処理を書く
//				例外クラス
//				std::invalid_argument	：無効な引数
//				std::out_of_range		：範囲外の添え字
//	*/
//	try {
//		// float型に変換して格納
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
#pragma region お試し：floatへの変換はできたがfloat型として認識はされなかった
	// お試し①
	//if (std::all_of(str_.cbegin(), str_.cend(), isdigit))
	//{
	//	// チェックが通れば表示(int型：stoi)
	//	//std::cout << strtod(str_.c_str(), NULL) << std::endl;
	//	std::cout << std::stof(str_) << std::endl;

	//	return true;
	//}
	//std::cout << "Not float , " << str_ << std::endl;


	// お試し②
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
		try		：例外が発生しうる処理を書く
		catch	：例外が発生した場合に行いう処理を書く
				例外クラス
				std::invalid_argument	：無効な引数
				std::out_of_range		：範囲外の添え字
	*/
	try {
		// float型に変換して格納
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


	/*　all_of	：範囲内の要素のすべてが指定された条件を満たすときture
	第一引数：範囲の開始位置を示すイテレータ
	第二引数：範囲の末端を示すイテレータ
	第三引数：条件判定に使う関数
			　指定したイテレータの値をとってbool型として戻り値を評価できるものなら関数でも構造体でも可
			  例：	isdigit→数字かどうか
					isalpha→アルファベットかどうか
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