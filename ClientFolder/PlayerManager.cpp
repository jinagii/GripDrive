#include "PlayerManager.h"
#include "CarMK3.h"
#include "Player.h"
#include "KeyInput.h"


PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{

}

Player* PlayerManager::CreatePlayer(int playerIndex)
{
	Player* _newPlayer = new Player(playerIndex + 1);

	v_Players.push_back(_newPlayer);

	return _newPlayer;

}
