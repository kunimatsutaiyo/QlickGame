#include "Enemy.h"
#include "../Engine/Graphics.h"

void Enemy::Update()
{
	m_survival_time--;
}

void Enemy::Draw()
{
	Graphics::GetInstance()->DrawTexture(
		Texture::GetInstance()->GetTexture("enemy"),
		m_pos.x, m_pos.y,
		255,
		60, 80);
}
