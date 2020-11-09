#include "Graphics.h"

bool Graphics::InitDirectGraphics(HWND window_handle_)
{
	D3DPRESENT_PARAMETERS present_param;

	// �C���^�[�t�F�[�X�쐬
	d3dinterface = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3dinterface == NULL)
	{
		// �쐬���s
		return false;
	}

	// D3DPRESENT_PARAMETERS�̐ݒ�
	// ���L�̃����o�ϐ��ȊO�ɖ����Ȓl���ݒ肳��Ȃ��悤��0�N���A���s��(present_param.BackBuffer...�̂��)
	ZeroMemory(&present_param, sizeof(D3DPRESENT_PARAMETERS));

	// �o�b�N�o�b�t�@�̐�
	present_param.BackBufferCount = 1;
	// �o�b�N�o�b�t�@�̃t�H�[�}�b�g(D3DFMT_UNKNOWN�̓t�H�[�}�b�g��m��Ȃ�)
	present_param.BackBufferFormat = D3DFMT_UNKNOWN;
	// �E�B���h�E���[�h�ݒ聨�萔�Ő؂�ւ�(true:�E�B���h�E�Afalse:�t���X�N���[��)
	present_param.Windowed = true;
	// �X���b�v�G�t�F�N�g��D3DSWAPEFFECT_DISCARD(�����ݒ�)
	present_param.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// ���̃o�b�N�o�b�t�@��yttm���Q�ƁI


	// XFile�Œǉ�����
	// Z�o�b�t�@ON
	present_param.EnableAutoDepthStencil = TRUE;
	present_param.AutoDepthStencilFormat = D3DFMT_D24S8;


	// IDirect3DDevice�̍쐬
	if (FAILED(d3dinterface->CreateDevice(
		D3DADAPTER_DEFAULT,	// �f�B�X�v���C�A�_�v�^�̎��
		D3DDEVTYPE_HAL,		// �f�o�C�X�̎�ނ�ݒ�
		window_handle_,		// �E�B���h�E�n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,	// �f�o�C�X����̑g�ݍ��킹
		&present_param,		// �f�o�C�X��ݒ肷�邽�߂�D3DPRESENT_PARAMETERS�\����
		&d3ddevice)))		// IDirect3DDevice9���i�[����IDirect3DDevice9�̃|�C���^�̃A�h���X
	{
		return false;
	}

	// �r���[�|�[�g�p�����[�^
	D3DVIEWPORT9 view_port;

	// �}���`�v���C���s�����݂����ɉ�ʂ����肽�����ɉ��L��������ꍇ������
	// �r���[�|�[�g�̍�����W
	view_port.X = 0;			// �`��̈��x���W
	view_port.Y = 0;			// �`��̈��y���W

	// �r���[�|�[�g�̃T�C�Y
	view_port.Width = present_param.BackBufferWidth;	// �`��̈�̉���
	view_port.Height = present_param.BackBufferHeight;	// �`��̈�̏c��

	// �r���[�|�[�g�[�x�ݒ�
	view_port.MinZ = 0.0f;		// �N���b�v�̈�̍ŏ��[�x�A��{��0.0f�ł���
	view_port.MaxZ = 1.0f;		// �N���b�v�̈�̍ő�[�x�A��{��1.0f�ł���

	// �r���[�|�[�g�̐ݒ�
	if (FAILED(d3ddevice->SetViewport(&view_port)))
	{
		return false;
	}

	// �t�H���g�f�o�C�X�̍쐬
	if (CreateFontDevice() == false)
	{
		return false;
	}

	Transparency(true);

	return true;
}

bool Graphics::CreateFontDevice()
{
	int size_list[] = {
		m_font_size_small,
		m_font_size_regular,
		m_font_size_large
	};

	for (int i = 0; i < static_cast<int>(FONT_SIZE::MAX_FONT_SIZE); i++)
	{
		if (FAILED(D3DXCreateFont(
			d3ddevice,					// DirectGraphics�f�o�C�X
			size_list[i],				// �t�H���g�̉����̃T�C�Y
			size_list[i] / 2,			// �t�H���g�̗����̃T�C�Y
			FW_REGULAR,
			0,
			FALSE,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			PROOF_QUALITY,
			FIXED_PITCH | FF_SCRIPT,
			TEXT("�l�r�@�o�S�V�b�N"),	// �t�H���g�̎��(�l�r�@�S�V�b�N�ACourier New�Ȃ�)
			&m_font_list[i]				// �t�H���g������p�N���X�ϐ��փ|�C���^���w��
		)))
		{
			return false;
		}
	}

	return true;
}

void Graphics::Transparency(bool true_or_false_)
{
	if (true_or_false_ == true)
	{
		d3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		d3ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		d3ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		d3ddevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		d3ddevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		d3ddevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	}
	else {
		d3ddevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		d3ddevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		d3ddevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	}
}

// �`��̃N���A�֐�
void Graphics::ReleaseDirectGraphics()
{
	// ���ʂȗ��R���Ȃ�����쐬�������Ƌt�̏��ԂŊJ������
	d3ddevice->Release();
	d3dinterface->Release();
}

bool Graphics::StartDraw()
{
	// �@�V�[���N���A
	/*
		2D�F��3�A��4�����̎w����������肷��

		3D�F��3�����Ƀf�v�X�A�X�e���V���̃o�b�t�@�ݒ�
			��5�A��6�����ŃN���A����l���w�肷��
	*/
	d3ddevice->Clear(
		0,									// ��2�����Ŏg�p����const D3DRECT*�̐�
		NULL,								// D3DRECT�̔z��(�N���A�����`�͈̔͂�ύX�������ꍇ�Ɏg�p�Anullptr�Ȃ�r���[�|�[�g�S�̂��N���A)
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,	//D3DCLEAR_TARGET,			// �N���A����o�b�t�@�̎�ނ����߂�t���O�̐ݒ�(D3DCLEAR_TARGET�AD3DCLEAR_STENCIL�AD3DCLEAR_ZBUFFER)
		D3DCOLOR_XRGB(0, 0, 0),				// �o�b�N�o�b�t�@���N���A����F�̏��
		1.0f,								// �f�v�X�o�b�t�@���N���A����l
		0									// �X�e���V���o�b�t�@���N���A����l
	);

#pragma region ���i�͂�������SetRenderState

	d3ddevice->SetRenderState(D3DRS_LIGHTING, FALSE);

#pragma endregion

#pragma region XFile�`�掞��SetRenderState��LightEnable

	//d3ddevice->SetRenderState(D3DRS_LIGHTING, true);
	//d3ddevice->LightEnable(0, true);

#pragma endregion

	// �A�V�[���`��J�n
	if (D3D_OK == d3ddevice->BeginScene())
	{
		return true;
	}

	return false;
}

void Graphics::EndDraw()
{
	// �B�V�[���`��I��
	d3ddevice->EndScene();

	// �C�o�b�N�o�b�t�@�]��
	d3ddevice->Present(
		nullptr,	// �]������`
		nullptr,	// �]�����`
		nullptr,	// �g�p����E�B���h�E�n���h��
		nullptr		// ��{nullptr
	);
}

void Graphics::DrawFont(const std::string text_, FONT_SIZE size_, FONT_COLOR color_, float x_, float y_)
{
	if (m_font_list[static_cast<int>(size_)] == nullptr)
	{
		return;
	}

	RECT rect = {
		(long)x_,
		(long)y_,
		(long)x_ + 800,
		(long)y_ + 300
	};

	int r, g, b;
	r = g = b = 255;

	if (color_ == FONT_COLOR::BLACK)
	{
		r = g = b = 0;
	}
	else if (color_ == FONT_COLOR::RED)
	{
		r = 255;
		g = b = 0;
	}

	m_font_list[static_cast<int>(size_)]->DrawTextA(
		nullptr,
		text_.c_str(),
		-1,
		&rect,
		DT_LEFT,
		D3DCOLOR_XRGB(r, g, b)
	);
}

// ���ߏ������s���Ă���ꍇ�A���l���������Ă������̂܂܂����
void Graphics::DrawPolygon(float x_, float y_, float width_, float height_, UCHAR transparency_)
{
	Transparency(true);

	DWORD color = D3DCOLOR_RGBA(0, 0, 0, transparency_);
	CustomVertex2D cv[] = {
		{x_, y_, 0.0f, 1.0f, color, 0.0f, 0.0f},					// ��0xffffffff	// ����
		{x_ + width_, y_, 0.0f, 1.0f , color, 1.0f, 0.0f},			// ��0xffff0000	// �E��
		{x_ + width_, y_ + height_, 0.0f, 1.0f, color, 1.0f, 1.0f},	// ��0xff00ff00	// �E��
		{x_, y_ + height_, 0.0f, 1.0f, color, 0.0f, 1.0f}			// ��0xff000000	// ����
	};

	d3ddevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,//D3DPT_TRIANGLESTRIP,	// �|���S���̍쐬���@�����X�g�A�X�g���b�v�A�t�@��������
		2,											// �`�悷��|���S���̐�
		cv,											// �|���S����`�悷�邽�߂Ɏg�����_�̔z��
		sizeof(CustomVertex2D)						// ���_���̃T�C�Y
	);
}

// 2D�p
void Graphics::DrawTexture(TextureDataAndSize* p_texture_ds_, float x_, float y_, UCHAR transparency_, int tex_width, int tex_height)
{
	Transparency(true);

	if (tex_width == NULL)
	{
		tex_width = p_texture_ds_->Width;
	}
	if (tex_height == NULL)
	{
		tex_height = p_texture_ds_->Height;
	}

	// ���_���̏���
	// z:0.0f�ARhw:1.0f�Œ�ł���
	// CustomVertex�ɐF����ǉ����遨0xffffffff�̂��

	DWORD color = D3DCOLOR_RGBA(255, 255, 255, transparency_);
	CustomVertex2D cv[] = {
		{x_, y_, 0.0f, 1.0f, color, 0.0f, 0.0f},							// ��0xffffffff	// ����
		{x_ + tex_width, y_, 0.0f, 1.0f , color, 1.0f, 0.0f},				// ��0xffff0000	// �E��
		{x_ + tex_width, y_ + tex_height, 0.0f, 1.0f, color, 1.0f, 1.0f},	// ��0xff00ff00	// �E��
		{x_, y_ + tex_height, 0.0f, 1.0f, color, 0.0f, 1.0f}				// ��0xff000000	// ����
	};

	// SetFVF�Œ��_���̓o�^���s��
	// �o�^�����񂪕������鎞�́u|�v�̋L�����g���Ĉ�x�ɂ��ׂēo�^����
	// �|���S���ɐF��ǉ�����Ƃ���D3DFVF_DIFFUSE��ǉ�
	// D3DFVF_XYZRHW�F�X�N���[�����W�ϊ��܂ł������I�ɂ���Ă���邽�߂̏��Ƃ����F����
	// 2D�̎��̂�D3DFVF_XYZRHW���g��
	d3ddevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);

	// �|���S����`�悷��
	// ���X�̏����͕K��BeginScene��EndScene�̊Ԃōs������
	// ���֐��ŕ����Ă���̂�StartDraw�֐���EndDraw�֐��̊ԂƂ������ƂɂȂ�
	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,//D3DPT_TRIANGLESTRIP,	// �|���S���̍쐬���@�����X�g�A�X�g���b�v�A�t�@��������
		2,											// �`�悷��|���S���̐�
		cv,											// �|���S����`�悷�邽�߂Ɏg�����_�̔z��
		sizeof(CustomVertex2D)						// ���_���̃T�C�Y
	);
}

void Graphics::Animation2D(
	TextureDataAndSize* p_texture_ds_,
	float x_, float y_,
	int split_x_, int split_y_,
	int animation_graph_num_)
{
	// �A�j���[�V��������ԍ�(x��,y��)
	int animation_x = (animation_graph_num_ % split_x_);
	int animation_y = (animation_graph_num_ / split_y_) + 1;

	float tu = 1.0f / split_x_;
	float tv = 1.0f / split_y_;

	float tu_num = tu * animation_x;
	float tv_num = tv * animation_y;

	int graph_width = p_texture_ds_->Width / split_x_;
	int graph_height = p_texture_ds_->Height / split_y_;

	CustomVertex2D cv[] = {
		{x_, y_, 0.0f, 1.0f, 0xffffffff, tu_num, tv_num},
		{x_ + graph_width, y_, 0.0f, 1.0f , 0xffffffff, tu_num + tu, tv_num},
		{x_ + graph_width, y_ + graph_height, 0.0f, 1.0f, 0xffffffff, tu_num + tu, tv_num + tv},
		{x_, y_ + graph_height, 0.0f, 1.0f, 0xffffffff, tu_num, tv_num + tv}
	};

	d3ddevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);
	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		cv,
		sizeof(CustomVertex2D)
	);
}

void Graphics::Slider2D(
	TextureDataAndSize* p_texture_ds_,
	DIRECTION direction_,
	float x_, float y_,
	float max_width_, float max_height_,
	float num_, float min_num_, float max_num_)
{
	// �w�肵���e�N�X�`���̃T�C�Y���g�p���Ă���e�N�X�`���̃T�C�Y�𒴂��Ă�����
	// �g�p���Ă���e�N�X�`���̃T�C�Y�ɕϊ����Ă�
	if (p_texture_ds_->Width < max_width_)
	{
		max_width_ = p_texture_ds_->Width;
	}
	if (p_texture_ds_->Height < max_height_)
	{
		max_height_ = p_texture_ds_->Height;
	}

	// ���݂̒l��䗦�Ƃ��ďo��
	// (���݂̒l - �ŏ��̒l) / (�ő�̒l - �ŏ��̒l)
	float rate = (num_ - min_num_) / (max_num_ - min_num_);

	// ��������������݂ĕ`�悷�镝������
	if (direction_ == DIRECTION::LEFT)
	{
		// �`��J�n�ʒu���E�ɂ���(RIGHT�̋t)
		// �䗦����`��J�n�ʒu�����炷
		x_ = (x_ + max_width_) - (rate * max_width_);
		// �T�C�Y���䗦�ŕύX����
		max_width_ *= rate;
	}
	else if (direction_ == DIRECTION::RIGHT)
	{
		max_width_ *= rate;
	}

	float left = x_ / p_texture_ds_->Width;
	float right = (x_ + max_width_) / p_texture_ds_->Width;
	float top = y_ / p_texture_ds_->Height;
	float bottom = (y_ + max_height_) / p_texture_ds_->Height;

	CustomVertex2D cv[] = {
		{x_, y_, 0.0f, 1.0f, 0xffffffff, left, top},
		{x_ + max_width_, y_, 0.0f, 1.0f , 0xffffffff, right, top},
		{x_ + max_width_, y_ + max_height_ , 0.0f, 1.0f, 0xffffffff, right, bottom},
		{x_, y_ + max_height_, 0.0f, 1.0f, 0xffffffff, left, bottom}
	};

	d3ddevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);
	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		cv,
		sizeof(CustomVertex2D)
	);
}

// 3D�p
void Graphics::DrawTexture3D(TextureDataAndSize* p_texture_ds_, DrawData3D& dd3D_)
{
	float top_tv = dd3D_.y / dd3D_.height;						// ��
	float bottom_tv = (dd3D_.y + dd3D_.height) / dd3D_.height;	// ��
	float left_tu = dd3D_.x / dd3D_.width;					// ��
	float right_tu = (dd3D_.x + dd3D_.width) / dd3D_.width;	// �E

	float harf_x = dd3D_.width / 2.0f;		// �����̒��S
	float harf_y = dd3D_.height / 2.0f;		// �c���̒��S

	// �|���S���̃��[�J�����W�̈ʒu���w��
	CustomVertex3D cv3D[] =
	{
		{-harf_x, harf_y, dd3D_.z, left_tu, top_tv, D3DCOLOR_ARGB(255, 255, 255, 255)},		// ����
		{harf_x, harf_y, dd3D_.z, right_tu, top_tv, D3DCOLOR_ARGB(255, 255, 255, 255)},		// �E��
		{harf_x, -harf_y, dd3D_.z, right_tu, bottom_tv, D3DCOLOR_ARGB(255, 255, 255, 255)},	// �E��
		{-harf_x, -harf_y, dd3D_.z, left_tu, bottom_tv, D3DCOLOR_ARGB(255, 255, 255, 255)},	// ����
	};

	// ���[���h���W�ϊ��p�̍s��̎Z�o start
	D3DXMATRIX mat_world, mat_trans, mat_rot, mat_rotx, mat_roty, mat_rotz, mat_scale;

	// �P�ʍs��
	D3DXMatrixIdentity(&mat_world);
	D3DXMatrixIdentity(&mat_scale);
	D3DXMatrixIdentity(&mat_rot);
	D3DXMatrixIdentity(&mat_trans);

	// �g�k
	D3DXMatrixScaling(&mat_scale, dd3D_.scalex, dd3D_.scaley, 1.0f);
	//m_p_matrix->MatrixScaling(&mat_scale, dd3D_.scalex, dd3D_.scaley, 1.0f);

	// ��]
	D3DXMatrixRotationX(&mat_rotx, D3DXToRadian(dd3D_.rotx));
	D3DXMatrixRotationY(&mat_roty, D3DXToRadian(dd3D_.roty));
	D3DXMatrixRotationZ(&mat_rotz, D3DXToRadian(dd3D_.rotz));
	//m_p_matrix->MatrixRotationX(&mat_rotx, D3DXToRadian(dd3D_.rotx));
	//m_p_matrix->MatrixRotationY(&mat_roty, D3DXToRadian(dd3D_.roty));
	//m_p_matrix->MatrixRotationZ(&mat_rotz, D3DXToRadian(dd3D_.rotz));

	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_rotx);
	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_roty);
	D3DXMatrixMultiply(&mat_rot, &mat_rot, &mat_rotz);
	//m_p_matrix->MatrixMultiply(&mat_rot, &mat_rotx, &mat_roty, &mat_rotz);

	// �ړ�							 
	D3DXMatrixTranslation(&mat_trans, dd3D_.x, dd3D_.y, dd3D_.z);
	//m_p_matrix->MatrixTranslation(&mat_trans, dd3D_.x, dd3D_.y, dd3D_.z);

	// �������킹(�g�k�~��]�~�ړ�)
	mat_world *= mat_scale * mat_rot * mat_trans;
	//m_p_matrix->MatrixMultiply(&mat_world, &mat_scale, &mat_rot, &mat_trans);

	d3ddevice->SetTransform(D3DTS_WORLD, &mat_world);
	// ���[���h���W�ϊ��p�̍s��̎Z�o end

	d3ddevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);

	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		cv3D,
		sizeof(CustomVertex3D)
	);
#pragma region ���m�N�R�[�h

	/*d3ddevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);

	d3ddevice->SetTexture(0, p_texture_ds_->TextureData);

	d3ddevice->DrawPrimitiveUP(
		D3DPT_TRIANGLEFAN,
		2,
		v,
		sizeof(CustomVertex3D)
	);*/

#pragma endregion
}

bool Graphics::CreateTexture(std::string file_name_, TextureDataAndSize* p_texture_ds_)
{
	D3DXIMAGE_INFO info;

	// 2�̗ݏ�o�Ȃ��P�[�X��z�肵�Č��̃T�C�Y���擾����D3DXCreateTextureFromFileEx�Ŏg��
	D3DXGetImageInfoFromFileA(file_name_.c_str(), &info);

	// �e�N�X�`���̓ǂݍ���
	/*D3DXCreateTextureFromFile(
		d3ddevice,
		p_file_name_,
		&texture_data_->TextureData
	);*/

	// �e�N�X�`���̓ǂݍ���(Ex)
	// ���ǂݍ��܂ꂽ�摜�T�C�Y��2�̗ݏ�łȂ������ꍇ�A�����I��2�̗ݏ�ɏC�������
	// ���ǂݍ��񂾃T�C�Y�̂܂܂ɂ������Ƃ��͑�3�A��4�����Ɍ��̃t�@�C���T�C�Y���w�肷��΂���
	if (FAILED(D3DXCreateTextureFromFileExA(
		d3ddevice,
		file_name_.c_str(),
		info.Width,
		info.Height,
		1,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0x0000ff00,
		nullptr,
		nullptr,
		&p_texture_ds_->TextureData
	)))
	{
		return false;
	}
	else {
		// �e�N�X�`���T�C�Y�̎擾
		D3DSURFACE_DESC desc;

		if (FAILED(p_texture_ds_->TextureData->GetLevelDesc(0, &desc)))
		{
			p_texture_ds_->TextureData->Release();
			return false;
		}
		p_texture_ds_->Width = desc.Width;
		p_texture_ds_->Height = desc.Height;
	}

	return true;
}

// ���ؑ��N�̂��Q�l�ɂ��܂���
// �����̌��ɂ���const�͂��̃N���X�̃����o�ϐ���ύX�ł��Ȃ����Ƃ�\���Ă���
// �����o�ϐ�(�t�B�[���h)
// �֐�(���\�b�h)
bool Graphics::SetView(const D3DMATRIX& matView_)const
{
	if (FAILED(d3ddevice->SetTransform(D3DTS_VIEW, &matView_)))
	{
		return false;
	}

	return true;
}
// ���ؑ��N�̂��Q�l�ɂ��܂���
void Graphics::GetViewport_Camera(D3DVIEWPORT9* vp_)
{
	d3ddevice->GetViewport(vp_);
}
// ���ؑ��N�̂��Q�l�ɂ��܂���
bool Graphics::SetMatProj(const D3DMATRIX& matProj_) const
{
	if (FAILED(d3ddevice->SetTransform(D3DTS_PROJECTION, &matProj_)))
	{
		return false;
	}

	return true;
}

// xFile
//bool Graphics::CreateXFile(const char* file_name_, XFileData* xfile_data_)
//{
//	// XFile�̓ǂݍ���
//	if (FAILED(D3DXLoadMeshFromXA(
//		file_name_,					// �t�@�C����(�p�X����)
//		D3DXMESH_SYSTEMMEM,			// ���b�V���쐬�p�̃I�v�V�������w��
//		d3ddevice,					// DirectGraphics�̃f�o�C�X
//		nullptr,					// �אڐ��f�[�^(��{nullptr�ł���)
//		&xfile_data_->Materials,	// �}�e���A���f�[�^
//		nullptr,					// �G�t�F�N�g�f�[�^(��{nullptr�ł���)
//		&xfile_data_->MaterialNum,	// �}�e���A���̐�
//		&xfile_data_->Meshes		// ���b�V���f�[�^
//	)))
//	{
//		return false;
//	}
//
//	// �}�e���A�����
//	// ���b�V���Ɏg�p����Ă���e�N�X�`���p�̔z���p�ӂ���
//	xfile_data_->Textures = new LPDIRECT3DTEXTURE9[xfile_data_->MaterialNum];
//
//	// �o�b�t�@�̐擪�|�C���^��D3DXMATERIAL�ɃL���X�g���Ď擾
//	D3DXMATERIAL* materials = (D3DXMATERIAL*)xfile_data_->Materials->GetBufferPointer();
//
//	// �e���b�V���̃}�e���A�������擾����
//	for (DWORD i = 0; i < xfile_data_->MaterialNum; i++)
//	{
//		// �}�e���A���Őݒ肳��Ă���e�N�X�`���ǂݍ���
//		if (materials[i].pTextureFilename != NULL)
//		{
//			char* file_name = materials[i].pTextureFilename;
//			LPDIRECT3DTEXTURE9 texture = NULL;
//			D3DXCreateTextureFromFileA(
//				d3ddevice,
//				file_name,
//				&xfile_data_->Textures[i]
//			);
//		}
//		else {
//			xfile_data_->Textures[i] = nullptr;
//		}
//	}
//
//	return true;
//}
//
//void Graphics::DrawXFile(float pos_x_, float pos_y_, float pos_z_, XFILE_LIST xf_id_)
//{
//
//	XFileData* xfile = XFile::GetInstance()->GetXFileData(xf_id_);
//
//	if (xfile == nullptr)
//	{
//		return;
//	}
//
//	// �ړ�����
//	D3DXMATRIX matrix;
//	D3DXMatrixIdentity(&matrix);
//	D3DXMatrixTranslation(&matrix, pos_x_, pos_y_, pos_z_);
//	d3ddevice->SetTransform(D3DTS_WORLD, &matrix);
//
//	// XFile�`��
//	D3DXMATERIAL* materials = (D3DXMATERIAL*)xfile->Materials->GetBufferPointer();
//	for (DWORD i = 0; i < xfile->MaterialNum; i++)
//	{
//		// �}�e���A���̐ݒ�
//		d3ddevice->SetMaterial(&materials[i].MatD3D);
//
//		// �e�N�X�`���̐ݒ�
//		d3ddevice->SetTexture(0, xfile->Textures[i]);
//
//		// ���b�V����`��
//		xfile->Meshes->DrawSubset(i);
//	}
//}