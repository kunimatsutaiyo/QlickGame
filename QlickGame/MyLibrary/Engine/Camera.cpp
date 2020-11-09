#include "Camera.h"
#include "Graphics.h"

#include <d3dx9.h>

void Camera::Update()
{
	D3DXMATRIX matProj, matView;

	// �r���[���W�ϊ��p�̍s��̎Z�o start
	D3DXVECTOR3 camera_pos(m_pos.x, m_pos.y, m_pos.z);						// �J�����̈ʒu
	D3DXVECTOR3 eye_pos(m_eye_pos.x, m_eye_pos.y, m_eye_pos.z);				// �����_
	D3DXVECTOR3 camera_upvector(m_upvector.x, m_upvector.y, m_upvector.z);	// �J�����̌���

	D3DXMatrixIdentity(&matView);
	D3DXMatrixLookAtLH(
		&matView,			// ���Z���ʂ��i�[����}�g���N�X�̃|�C���^
		&camera_pos,		// �J�����̈ʒu���W�̃|�C���^
		&eye_pos,			// �����_�̈ʒu���W�̃|�C���^
		&camera_upvector	// ���[���h���W�̏�����̃x�N�g��
	);
	Graphics::GetInstance()->SetView(matView);
	// �r���[���W�ϊ��p�̍s��̎Z�o end

	// �ˉe���W�ϊ��p�̍s��̎Z�o start
	D3DVIEWPORT9 vp;
	Graphics::GetInstance()->GetViewport_Camera(&vp);
	float aspect = (float)vp.Width / (float)vp.Height;

	float view_frustum_near = 0.1f;
	float view_frustum_far = 700.0f;
	int view_angle = 45;

	D3DXMatrixPerspectiveFovLH(
		&matProj,
		D3DXToRadian(view_angle),
		aspect,
		view_frustum_near,
		view_frustum_far
	);
	Graphics::GetInstance()->SetMatProj(matProj);
	// �ˉe���W�ϊ��p�̍s��̎Z�o end
}

void Camera::Move(D3DXVECTOR3 pos_)
{
	m_pos = pos_;

	m_eye_pos = { m_pos.x,m_pos.y,m_pos.z + 10.0f };
}