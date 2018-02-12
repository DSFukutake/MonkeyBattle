#include "stdafx.h"
#include "Army.h"
#include "DataLibrary/DataLibrary.h"

Army::Army()
{
	m_order.clear();
	m_size = 0;
}
Army::Army(std::vector<std::string> order, int size)
	: m_order(order)
	, m_size(size)
{}

Army::~Army()
{
	for (auto unit : m_army)
	{
		delete unit;
	}
	m_army.clear();
}

//Set the current army info based on army parameter
void Army::ConstructFrom(Army* army)
{
	m_size = army->GetArmySize();
	m_order = army->GetArmyOrder();
}
//Initialize the army based on the sequence from the preset
void Army::InitArmy()
{
	int sequenceIndex = 0;
	for (int i = 0; i < m_size; i++)
	{
		std::string unitName = m_order.at(sequenceIndex);
		Unit* armyUnit = DataLibrary::GetInstance()->GetUnit(unitName);
		if(armyUnit != nullptr)
		{
			Unit* newArmyUnit = new Unit();
			newArmyUnit->SetDataFrom(armyUnit);
			m_army.push_back(newArmyUnit);
			if (sequenceIndex >= m_order.size() - 1 )
			{
				sequenceIndex = 0;
			}
			else 
			{
				sequenceIndex++;
			}
		}
	}
}

//Return the Unit at index in m_army
Unit* Army::GetArmyUnitAt(int index) const
{
	if (index < m_army.size())
	{
		return m_army.at(index);
	}

	return nullptr;
}

//Return army size
int Army::GetArmySize() const
{
	return m_size;
}

//return order/sequence of army 
std::vector<std::string> Army::GetArmyOrder() const
{
	return m_order;
}