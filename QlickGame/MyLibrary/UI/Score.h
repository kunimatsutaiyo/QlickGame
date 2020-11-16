#ifndef SCORE_H_
#define SCORE_H_

class Score
{
public:
	Score(int init_score_)
	{
		m_score = init_score_;
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