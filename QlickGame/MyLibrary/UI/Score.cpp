#include "Score.h"
#include "../Engine/Graphics.h"

#include <sstream>

void Score::AddScore(int score_number_)
{
	m_score += score_number_;
}

void Score::Draw()
{
	std::ostringstream oss;
	oss << "Score : " << m_score;

	Graphics::GetInstance()->DrawFont(
		oss.str(),
		FONT_SIZE::LARGE, FONT_COLOR::WHITE,
		1000.0f, 100.0f);
}
