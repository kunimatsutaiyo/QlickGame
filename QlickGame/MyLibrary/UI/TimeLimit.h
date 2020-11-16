#ifndef TIME_LIMIT_H_
#define TIME_LIMIT_H_

class TimeLimit
{
public:
	TimeLimit(int seconds_)
	{
		m_fps = seconds_ * 60;
	}
	~TimeLimit()
	{

	}

public:
	void Update();
	void Draw();

	int GetTime() {
		return m_fps;
	}

private:
	int m_fps;

};

#endif