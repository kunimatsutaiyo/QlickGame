#ifndef TIME_LIMIT_H_
#define TIME_LIMIT_H_

class TimeLimit
{
public:
	TimeLimit(int seconds_)
	{
		m_time_limit = seconds_ * 60;
	}
	~TimeLimit()
	{

	}

public:
	void Update();
	void Draw();

	int GetTime() {
		return m_time_limit;
	}

private:
	int m_time_limit;

};

#endif