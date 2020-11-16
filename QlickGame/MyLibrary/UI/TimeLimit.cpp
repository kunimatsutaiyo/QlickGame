#include "TimeLimit.h"
#include "../Engine/Graphics.h"

#include <sstream>

void TimeLimit::Update()
{
	m_fps--;
	if (m_fps <= 0)
	{
		m_fps = 0;
	}
}

void TimeLimit::Draw()
{
	int seconds = m_fps / 60;

	std::ostringstream oss;
	oss << "Time Limit : " << seconds;

	Graphics::GetInstance()->DrawFont(
		oss.str(),
		FONT_SIZE::LARGE, FONT_COLOR::WHITE,
		100.0f, 100.0f);
}
