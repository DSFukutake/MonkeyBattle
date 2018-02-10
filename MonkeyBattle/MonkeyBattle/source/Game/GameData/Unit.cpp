#include "stdafx.h"
#include "Unit.h"

Unit::Unit()
{
	m_data.m_name = "";
	m_data.m_damage = 0;
	m_data.m_health = 0;
	m_data.m_type = UnitType::k_type_none;
	m_data.m_preferred= UnitType::k_type_none;
}


void Unit::SetDataFrom(Unit unit)
{
	m_data.m_name = unit.GetUnitData().m_name;
	m_data.m_damage = unit.GetUnitData().m_damage;
	m_data.m_health = unit.GetUnitData().m_health;
	m_data.m_type = unit.GetUnitData().m_type;
	m_data.m_preferred = unit.GetUnitData().m_preferred;
}

Unit::Unit(UnitData data)
	:m_data(data)
{
}

void Unit::IncreaseDamage(int amount)
{
	m_data.m_damage += amount;
}

void Unit::TakeDamage(int amount)
{
	m_data.m_health -= amount;
}

Unit::UnitData Unit::GetUnitData() const
{
	return m_data;
}

bool Unit::IsAlive()
{
	return (m_data.m_health > 0);
}


bool Unit::IsPreferred(UnitType type)
{
	if (m_data.m_type == type || type == UnitType::k_type_all)
	{
		return true;
	}
	return false;
}

int Unit::GetDamageBinaryMVPValue()
{
	int num = m_data.m_damage; //store the number since we will be altering it
	int biggest = 0;
	int count = 0;
	while (num > 0)
	{
		int lsb = (num & 1); //check if LSB
		num >>= 1; // Remove the lsb
		count = (lsb == 1) ? (count + 1) : 0; //if lsb is 1 , increment chain counter, otherwise reset
		if(count > biggest)
		{
			//update with the largest chain
			biggest = count;
		}
	}
	CLOG("Biggest chain of 1s in %d is %d\n", m_data.m_damage, biggest);
	return biggest;
}

