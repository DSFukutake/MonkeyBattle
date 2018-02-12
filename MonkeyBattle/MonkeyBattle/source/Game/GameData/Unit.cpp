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

//Set Unit data from specified unit parameter
void Unit::SetDataFrom(Unit* unit)
{
	if(unit != nullptr)
	{	
		UnitData copyFrom = unit->GetUnitData();
		m_data.m_name = copyFrom.m_name;
		m_data.m_damage = copyFrom.m_damage;
		m_data.m_health = copyFrom.m_health;
		m_data.m_type = copyFrom.m_type;
		m_data.m_preferred = copyFrom.m_preferred;
	}

}

Unit::Unit(UnitData data)
	:m_data(data)
{
}

//Increase this unit's base damage by amount
void Unit::IncreaseDamage(int amount)
{
	m_data.m_damage += amount;
}

//Decrease this unit's health by amount
void Unit::TakeDamage(int amount)
{
	m_data.m_health -= amount;
}

//Get unit's specific data, damage, name, health,etc.
Unit::UnitData Unit::GetUnitData() const
{
	return m_data;
}

//Return true if Unit health is above 0. 
bool Unit::IsAlive()
{
	return (m_data.m_health > 0);
}

//Return true if parameter type  is  equal to the Unit's type or if it is the special all type.
bool Unit::IsPreferred(UnitType type)
{
	return (m_data.m_type == type || type == UnitType::k_type_all);
}

//Return the number of the largest chain of 1s in the binary representation of this unit's damage
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

