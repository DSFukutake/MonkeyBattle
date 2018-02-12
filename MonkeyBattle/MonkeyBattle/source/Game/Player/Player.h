#pragma once

#include "Game/GameData/Army.h"
#include "Game/GameData/Unit.h"

class Player
{
public:

	Player();
	Player(Army army);
	~Player();

	void Init();
	Unit GetMVP() const;
	bool IsDefeated();
	void SetArmyIdName(std::string idName);
	int GetArmySize() const;
	Unit* GetUnit(int index);
	std::string GetArmyId() const;
	void PrintWinnerArmy();
private:

	Army* m_playerArmy;
	Unit* m_mvp;
	std::string m_playerArmyId;
};