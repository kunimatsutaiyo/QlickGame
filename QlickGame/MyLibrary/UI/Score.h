#ifndef SCORE_H_
#define SCORE_H_

class Score
{
public:
	Score()
	{
		m_score = 0;
	}
	~Score()
	{

	}

public:
	void AddScore(int score_number_);

	void Draw();

private:
	int m_score;

};

#endif