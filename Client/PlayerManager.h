#pragma once
#include "GlobalDefine.h"

class CarMK3;
class Player;

class PlayerManager
{
public:
	PlayerManager();
	~PlayerManager();

private:

	int m_PlayerNumber;
	int m_CarIndex;

	vector<Player*> v_Players;

public:	
	// Getter / Setter
	int GetPlayerNumber() const { return m_PlayerNumber; }

	Player* CreatePlayer(int playerIndex);
};

