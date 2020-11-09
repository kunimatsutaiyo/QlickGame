#ifndef SIZE_H_
#define SIZE_H_

//! �T�C�Y�f�[�^�\����
struct Size {
	/** Constructor */
	Size()
	{
		Width = 0.0f;
		Height = 0.0f;
	}

	/**
	* @brief Constructor
	* @param[in] Width	����
	* @param[in] height	�c��
	*/
	Size(float width_, float height_)
	{
		Width = width_;
		Height = height_;
	}

	/**
	* @brief Constructor
	* @param[in] size	�R�s�[�p�T�C�Y�f�[�^
	*/
	Size(const Size& size_)
	{
		this->Width = size_.Width;
		this->Height = size_.Height;
	}

	float Width;	// ����
	float Height;	// �c��
	//float Depth;	// ���s
};

#endif