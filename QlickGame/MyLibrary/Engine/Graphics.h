#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

#include "../Utility/Singleton.h"
#include "Texture.h"
//#include "XFile.h"

// ��@�����p�̃}�N�����`
#define SAFE_RELEASE(x) { if(x) { (x)->Release(); (x) = NULL; } }

// �t�H���g�̃T�C�Y
enum class FONT_SIZE
{
	SMALL,		// ��
	REGULAR,	// ��
	LARGE,		// ��

	MAX_FONT_SIZE
};

// �t�H���g�̃J���[
enum class FONT_COLOR
{
	BLACK,	// ��
	WHITE,	// ��
	RED,	// ��

	MAX_FONT_COLOR
};

// �X���C�_�[�̑������������
enum class DIRECTION
{
	LEFT,
	RIGHT,

	MAX_DIRECTION
};

// �`��ŕK�v�Ƃ��钸�_���W���߂�(2D)
struct CustomVertex2D {
	// ���_���W
	float x;		// x�����W
	float y;		// y�����W
	float z;		// z�����W	��3D�|���S����2D�|���S���Ƃ��ĉ��H���邽�ߕK�v

	// ���Z��
	float rhw;		// ���Z��	��3D����2D�։��H����̂ɕK�v�ȃp�����[�^

	// �|���S���̐F
	DWORD color;	// �F���

	// �e�N�X�`�����W
	float tu;		// u�e�N�X�`�����W
	float tv;		// v�e�N�X�`�����W
};

// �`��ŕK�v�Ƃ��钸�_���W���߂�(3D)
struct CustomVertex3D {
	float x;
	float y;
	float z;

	float tu;
	float tv;

	DWORD color;
};

// 3D��ԏ�Ƀe�N�X�`���𒣂�t���邽�߂̃f�[�^
struct DrawData3D {
	float x;
	float y;
	float z;

	float width;
	float height;

	DWORD color;

	float rotx;		// x��]
	float roty;		// y��]
	float rotz;		// z��]

	float scalex;	// �g�kx
	float scaley;	// �g�ky
					// �g�kz�̓e�N�X�`���Ȃ̂łȂ�
};

class Graphics : public Singleton<Graphics> {
private:
	friend Singleton<Graphics>;

public:
	/**
	* @brief DirectGraphics������������֐�
	* @param[in] window_handle_	�E�B���h�E�n���h��
	*/
	bool InitDirectGraphics(HWND window_handle_);

	/**
	* @prief �t�H���g��`�悷�邽�߂̏�����
	*/
	bool CreateFontDevice();

	/**
	* @brief ���ߏ������s�����߂̏���������
	* �����ߏ������s�������e�N�X�`���̑O��
	*/
	void Transparency(bool true_or_false_);

	/**
	* @brief �f�o�C�X�E�C���^�[�t�F�[�X���J������֐�
	*/
	void ReleaseDirectGraphics();

	/**
	* @brief �`��J�n�֐�
	*/
	bool StartDraw();

	/**
	* @brief �`��I���֐�
	*/
	void EndDraw();

	/**
	* @brief �t�H���g�`��֐�
	* @param[in] text_ �`�悷��e�L�X�g
	* @param[in] size_ �`�悷��e�L�X�g�̃T�C�Y
	* @param[in] color_ �`�悷��e�L�X�g�̐F
	* @param[in] x_ x���W
	* @param[in] y_ y���W
	*/
	void DrawFont(const std::string text_, FONT_SIZE size_, FONT_COLOR color_, float x_, float y_);

	/**
	* @brief 2D�p�`��֐�
	* @param[in] x_ x���W
	* @param[in] y_ y���W
	* @param[in] width_ ����
	* @param[in] height_ �c��
	* @param[in] transparency_ �e�N�X�`���̓��ߓx
	*/
	void DrawPolygon(float x_, float y_, float width_, float height_, UCHAR transparency_ = 255);

	/**
	* @brief 2D�p�`��֐�
	* @param[in] p_texture_ds_ �e�N�X�`���̃f�[�^
	* @param[in] x_ x���W
	* @param[in] y_ y���W
	* @param[in] transparency_ �e�N�X�`���̓��ߓx
	* @param[in] tex_width �e�N�X�`���̉���
	* @param[in] tex_height �e�N�X�`���̏c��
	*/
	void DrawTexture(TextureDataAndSize* p_texture_ds_, float x_, float y_, UCHAR transparency_ = 255, int tex_width = NULL, int tex_height = NULL);

	/**
	* @brief 2D�p�A�j���[�V�����֐�
	* @param[in] p_texture_ds_ �e�N�X�`���̃f�[�^
	* @param[in] x_ x���W
	* @param[in] y_ y���W
	* @param[in] split_x_ ���̕�����
	* @param[in] split_y_ �c�̕�����
	* @param[in] animation_graph_num_ �A�j���[�V��������e�N�X�`���̔ԍ�(���ォ��E���Ɍ�����)
	*/
	void Animation2D(
		TextureDataAndSize* p_texture_ds_,
		float x_,
		float y_,
		int split_x_,
		int split_y_,
		int animation_graph_num_);

	/**
	* @brief 2D�p�X���C�_�[�֐�
	* @param[in] p_texture_ds_ �e�N�X�`���̃f�[�^
	* @param[in] direction_ �������������(�m�肵������+�A�t��-)
	* @param[in] x_ x���W
	* @param[in] y_ y���W
	* @param[in] max_width_ �`�悵������
	* @param[in] max_height_ �`�悵��������
	* @param[in] num_ ���݂̒l
	* @param[in] min_num_ �ŏ��l
	* @param[in] max_num_ �ő�l
	*/
	void Slider2D(
		TextureDataAndSize* p_texture_ds_,
		DIRECTION direction_,
		float x_,
		float y_,
		float max_width_,
		float max_height_,
		float num_,
		float min_num_,
		float max_num_
	);

	/**
	* @brief 3D�p�`��֐�
	*/
	void DrawTexture3D(TextureDataAndSize* p_texture_ds_, DrawData3D& dd3D_);

	/**
	* @brief ~~~~~~~~~~~~~~~~�֐�
	*/
	bool CreateTexture(std::string file_name_, TextureDataAndSize* p_texture_ds_);

	bool SetView(const D3DMATRIX& matView_)const;

	void GetViewport_Camera(D3DVIEWPORT9* vp_);

	bool SetMatProj(const D3DMATRIX& matProj_)const;

#pragma region XFile�p

	/*bool CreateXFile(const char* file_name_, XFileData* xfile_data_);

	void DrawXFile(float pos_x_, float pos_y_, float pos_z_, XFILE_LIST xf_id_);*/

#pragma endregion

	const LPDIRECT3DDEVICE9 GetD3DDevice(void) {
		return d3ddevice;
	}

private:
	Graphics()
	{
		d3ddevice = nullptr;
		d3dinterface = nullptr;

		m_font_size_small = 16;
		m_font_size_regular = 24;
		m_font_size_large = 32;
	}
	~Graphics() {}

	// �R�s�[�R���X�g���N�^�̋֎~
	Graphics(const Graphics&);
	Graphics& operator=(const Graphics&) = delete;

	// ���[�u�R���X�g���N�^�̋֎~
	Graphics& operator=(const Graphics&&) = delete;

private:
	static Graphics* p_GraphicsInstance;	// Graphics�̃C���X�^���X

	LPDIRECT3D9 d3dinterface;		// DirectGraphics�C���^�[�t�F�C�X
	LPDIRECT3DDEVICE9 d3ddevice;	// DirectGraphics�f�o�C�X

	LPD3DXFONT m_font_list[static_cast<int>(FONT_SIZE::MAX_FONT_SIZE)];
	int m_font_size_small;
	int m_font_size_regular;
	int m_font_size_large;

};

#endif