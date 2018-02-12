#include "stdafx.h"
#include "Player.h"

Player::Player()
	: m_playerArmyId("")
	, m_playerArmy(nullptr)
{
}


Player::Player(Army* army)
	:m_playerArmyId("")
{
	if(army != nullptr)
	{
		m_playerArmy = new Army();
		m_playerArmy->ConstructFrom(army);
	}
}

Player::~Player()
{
	delete m_playerArmy;
}

//Initialize the player's army and print the composition
void Player::Init() 
{
	if(m_playerArmy != nullptr)
	{
		m_playerArmy->InitArmy();
		int sz = m_playerArmy->GetArmySize();
	
		LOG_GAME_INFO("%s - Initial Troops\n", m_playerArmyId.c_str());
		for (int i = 0; i < sz; i++)
		{
			Unit* pUnit = m_playerArmy->GetArmyUnitAt(i);
			if(pUnit != nullptr)
			{
				LOG_GAME_INFO("%d - %s\n", i+1, pUnit->GetUnitData().m_name.c_str());
			}
		}
		LOG_GAME_INFO(" \n\n");
	}
}

//Set the name of the army
void Player::SetArmyIdName(std::string idName)
{
	m_playerArmyId = idName;
}

//Return true if all units in the player's army are dead
bool Player::IsDefeated()
{
	if (m_playerArmy == nullptr)
	{
		return true;
	}

	int sz = m_playerArmy->GetArmySize();
	for (int i = 0; i < sz; i++)
	{
		Unit* pUnit = m_playerArmy->GetArmyUnitAt(i);
		if (pUnit != nullptr)
		{
			if (pUnit->IsAlive())
			{
				//at least one unit is alive
				return false;
			}
		}
	}
	//no unit was found alive
	return true;
}

//Return size of player army
int Player::GetArmySize() const
{
	if (m_playerArmy == nullptr)
	{
		return 0;
	}

	return m_playerArmy->GetArmySize();
}

//Get unit at index from player's army
//Can return nullptr from GetArmyUnitAt
Unit* Player::GetUnit(int index)
{
	return m_playerArmy->GetArmyUnitAt(index);
}

//Return the name id of the army
std::string Player::GetArmyId() const
{
	return m_playerArmyId;
}

//Print winner's report, surviving troops and most outstanding troop
void Player::PrintWinnerArmy()
{
	
	LOG_GAME_INFO("\n ---- WINNER ----- \n");
	LOG_GAME_INFO("%s Is the Winner - Remaining Troops\n", m_playerArmyId.c_str());
	int sz = m_playerArmy->GetArmySize();
	int aliveCount = 0;
	int highestDamageBinChainValue = INT32_MIN;
	
	for (int i = 0; i < sz; i++)
	{
		Unit* uni = m_playerArmy->GetArmyUnitAt(i);
		
		if (uni && uni->IsAlive())
		{
			int dmgVal = uni->GetDamageBinaryMVPValue();
			if (dmgVal > highestDamageBinChainValue)
			{
				highestDamageBinChainValue = dmgVal;
				m_mvp = uni;
			}
			Unit::UnitData uData = uni->GetUnitData();	
			LOG_GAME_INFO("%d - %s(health = %d, totalDamage = %d)\n", ++aliveCount, uData.m_name.c_str(), uData.m_health, uData.m_damage);
		}
	}
	LOG_GAME_INFO(" \n\n");
	if (m_mvp != nullptr)
	{
		Unit::UnitData mvpData = m_mvp->GetUnitData();
		LOG_GAME_INFO("The most outstanding troop is: %s health=%d damage=%d\n\n", mvpData.m_name.c_str(), mvpData.m_health, mvpData.m_damage);
	}
}