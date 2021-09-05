#pragma once
class Player
{
public:
	Player(int index);
	~Player();

private:
	int m_PlayerIndex;
	
public:
	int GetPlayerIndex() const { return m_PlayerIndex; }

	int CarSelected();
};

