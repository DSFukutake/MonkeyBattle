#include "stdafx.h"
#include "Player.h"

Player::Player()
	: m_playerArmyId("")
	, m_playerArmy(nullptr)
{
}


Player::Player(Army army)
	:m_playerArmyId("")
{
	m_playerArmy = new Army();
	m_playerArmy->ConstructFrom(army);
}

Player::~Player()
{
	m_playerArmy = nullptr;
	delete m_playerArmy;
}

void Player::Init() 
{
	m_playerArmy->InitArmy();
	int sz = m_playerArmy->GetArmySize();
	
	LOG_GAME_INFO("%s - Initial Troops\n", m_playerArmyId.c_str());
	for (int i = 0; i < sz; i++)
	{
		LOG_GAME_INFO("%d - %s\n", i+1, m_playerArmy->GetArmyUnitAt(i)->GetUnitData().m_name.c_str());
	}
	LOG_GAME_INFO(" \n\n");

}

void Player::SetArmyIdName(std::string idName)
{
	m_playerArmyId = idName;
}

bool Player::IsDefeated()
{
	int sz = m_playerArmy->GetArmySize();
	for (int i = 0; i < sz; i++)
	{
		if (m_playerArmy->GetArmyUnitAt(i)->IsAlive())
		{
			//at least one unit is alive
			return false;
		}
	}
	//no unit was found alive
	return true;
}

int Player::GetArmySize() const
{
	return m_playerArmy->GetArmySize();
}

Unit* Player::GetUnit(int index)
{
	return m_playerArmy->GetArmyUnitAt(index);
}


std::string Player::GetArmyId() const
{
	return m_playerArmyId;
}

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
	Unit::UnitData mvpData = m_mvp->GetUnitData();
	LOG_GAME_INFO("The most outstanding troop is: %s health=%d damage=%d", mvpData.m_name.c_str(), mvpData.m_health, mvpData.m_damage);

}