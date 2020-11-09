#ifndef SINGLETON_H_
#define SINGLETON_H_

// Template_Class
template<class T>
class Singleton {
	// シングルトンデザインパターン
public:
	// 実体を作る関数
	static void CreateInstance()
	{
		if (CheckNull() == true)
		{
			p_Instance = new T();
		}
	}

	// 実体を破棄する関数
	static void DestroyInstance()
	{
		if (!CheckNull() == false)
		{
			delete p_Instance;
			p_Instance = nullptr;
		}
	}

	// 実体が存在するか確認する関数
	static bool CheckNull()
	{
		return p_Instance == nullptr;
	}

	// 実体を取得する関数
	static T* GetInstance()
	{
		if (CheckNull() == true)
		{
			CreateInstance();
		}
		return p_Instance;
	}

protected:
	Singleton() {}		// コンストラクタ抑制
	~Singleton() {}		// デストラクタ抑制

private:
	static T* p_Instance;	// 自分自身の唯一の実体

};

// staticなメンバ変数を初期化
template<class T>
T* Singleton<T>::p_Instance = nullptr;

#endif