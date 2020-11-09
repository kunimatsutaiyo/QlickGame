#include "Camera.h"
#include "Graphics.h"

#include <d3dx9.h>

void Camera::Update()
{
	D3DXMATRIX matProj, matView;

	// ビュー座標変換用の行列の算出 start
	D3DXVECTOR3 camera_pos(m_pos.x, m_pos.y, m_pos.z);						// カメラの位置
	D3DXVECTOR3 eye_pos(m_eye_pos.x, m_eye_pos.y, m_eye_pos.z);				// 注視点
	D3DXVECTOR3 camera_upvector(m_upvector.x, m_upvector.y, m_upvector.z);	// カメラの向き

	D3DXMatrixIdentity(&matView);
	D3DXMatrixLookAtLH(
		&matView,			// 演算結果を格納するマトリクスのポインタ
		&camera_pos,		// カメラの位置座標のポインタ
		&eye_pos,			// 注視点の位置座標のポインタ
		&camera_upvector	// ワールド座標の上向きのベクトル
	);
	Graphics::GetInstance()->SetView(matView);
	// ビュー座標変換用の行列の算出 end

	// 射影座標変換用の行列の算出 start
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
	// 射影座標変換用の行列の算出 end
}

void Camera::Move(D3DXVECTOR3 pos_)
{
	m_pos = pos_;

	m_eye_pos = { m_pos.x,m_pos.y,m_pos.z + 10.0f };
}