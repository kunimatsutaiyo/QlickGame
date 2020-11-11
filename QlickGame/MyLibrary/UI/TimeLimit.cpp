#include "TimeLimit.h"
#include "../Engine/Graphics.h"

#include <sstream>

void TimeLimit::Update()
{
	m_time_limit--;
	if (m_time_limit <= 0)
	{
		m_time_limit = 0;
	}
}

void TimeLimit::Draw()
{
	int seconds = m_time_limit / 60;

	std::ostringstream oss;
	oss << seconds;

	Graphics::GetInstance()->DrawFont(
		oss.str(),
		FONT_SIZE::LARGE, FONT_COLOR::WHITE,
		100.0f, 100.0f);
}
