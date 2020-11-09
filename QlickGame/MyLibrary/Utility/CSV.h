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
#pragma region コメントの書き方例
	/**
	 * @fn
	 * ここに関数の説明を書く
	 * @brief 要約説明
	 * @param (引数名) 引数の説明
	 * @param (引数名) 引数の説明
	 * @return 戻り値の説明
	 * @sa 参照すべき関数を書けばリンクが貼れる
	 * @detail 詳細な説明
	 */
#pragma endregion

	 /**
	 *	@brief テキストファイルを作成する関数
	 *	@return	?????
	 */
	int CreateFile();

	/**
	*	@brief strin型の文字列の空白を消す関数
	*	@param[in] str_					string型の文字列を入れる
	*	@param[in] trim_character_list_	消したい文字
	*	@return	string型を返す
	*/
	std::string StringTrim(const std::string& str_, const char* trim_character_list_);

	/**
	*	@brief 文字列が数字かどうかチェックする(float)関数
	*	@param[in] str_			string型の文字列を入れる
	*	@param[in] is_float_	?????
	*	@return	bool型を返す
	*/
	//bool CheckString(std::string str_);

	/**
	*	@brief 文字列が数字かどうかチェックする(float)関数
	*	@param[in] str_			string型の文字列を入れる
	*	@param[in] is_float_	?????
	*	@return	bool型を返す
	*/
	bool CheckStringOrNumber(std::string str_);

	/**
	*	@brief 関数
	*	@param[in] str_	string型の文字列を入れる
	*	@return
	*/
	void PrintCSV();

	/**
	*	@brief<Get> 指定されたキーの中のパラメータ取得する関数
	*	@param[in] key_ keyの名前を入れる
	*	@return	キーの中の値を参照型で返す？
	*/
	std::vector<std::vector<std::string>>* GetParam(std::string key_)
	{
		if (DataHasKeyName(key_) == true)
		{
			return &m_object_list[key_];
		}

		///*
		//	キーの検索
		//		mpをfmapオブジェクトとすると
		//		mp.find(キー)で、キーがmpに設定されるかどうかを調べられる
		//		キーが設定されている場合は、キーと値のペアへのイテレータを返す
		//		キーが設定されていない場合は、end()へのイテレータを返す
		//*/
		//std::map<std::string, std::vector<std::string>>::iterator itr = character_param.find(key_);
		//if (itr == character_param.end())
		//{
		//	return nullptr;
		//}
		//return &character_param[key_];
	}

	/**
	*	@brief csv(txt)ファイルを読み込む関数
	*	@param[in] file_name_ string型の文字列を入れる
	*	@param[in] key_name_ string型の文字列を入れる
	*	@return bool型を返す
	*/
	bool LoadData(std::string file_name_, std::string key_name_);

	/**
	*	@brief csv(txt)ファイルに書き込む関数
	*	@param[in] str_		string型の文字列を入れる
	*	@param[in] vecf_	vector型を返す(float型を格納して)
	*	@return bool型を返す
	*	※今はランキングしか考えていないので、書き込む内容は決まっている
	*/
	//bool WriteData(std::string str_, std::vector<float> vecf_);

	bool WriteData(std::string str_, std::vector<std::string> strvec_);

private:

	/**
	*	@brief 文字列が一致しているか判定する関数
	*	@param[in] str_ string型の文字列を入れる
	*	@return	bool型を返す
	*/
	bool CheckLanuage(std::string str_);

	/**
	*	@brief コンマ区切りで文字列を分割する関数
	*	@param[in] str_			string型の文字列を入れる
	*	@param[in] delimiter_	区切りたい文字を入れる
	*	@return	vector型を返す(string型を格納して)
	*/
	// delimiter:(意味)区切り符号
	std::vector<std::string> Split(std::string& str_, char delimiter_);

	/**
	* @brief FileListのキーチェック関数
	* 引数で指定されたキーが登録されているかを確認します
	* return 登録されていた場合はtrue、未登録の場合はfalse
	* @param[in] key_name　チェックしたいキー
	*/
	bool FileHasKeyName(std::string key_name_);

	/**
	* @brief DataListのキーチェック関数
	* 引数で指定されたキーが登録されているかを確認します
	* return 登録されていた場合はtrue、未登録の場合はfalse
	* @param[in] key_name　チェックしたいキー
	*/
	bool DataHasKeyName(std::string key_name_);

private:
	CSV() {}
	~CSV() {}

	// コピーコンストラクタの禁止
	CSV(const CSV&);
	CSV& operator=(const CSV&) = delete;

	// ムーブコンストラクタの禁止
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