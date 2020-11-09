#ifndef SIZE_H_
#define SIZE_H_

//! サイズデータ構造体
struct Size {
	/** Constructor */
	Size()
	{
		Width = 0.0f;
		Height = 0.0f;
	}

	/**
	* @brief Constructor
	* @param[in] Width	横幅
	* @param[in] height	縦幅
	*/
	Size(float width_, float height_)
	{
		Width = width_;
		Height = height_;
	}

	/**
	* @brief Constructor
	* @param[in] size	コピー用サイズデータ
	*/
	Size(const Size& size_)
	{
		this->Width = size_.Width;
		this->Height = size_.Height;
	}

	float Width;	// 横幅
	float Height;	// 縦幅
	//float Depth;	// 奥行
};

#endif