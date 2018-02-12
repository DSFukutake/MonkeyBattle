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

void Army::InitArmy()
{
	int sequenceIndex = 0;
	for (int i = 0; i < m_size; i++)
	{
		std::string unitName = m_order.at(sequenceIndex);
		Unit armyUnit = DataLibrary::GetInstance()->GetUnit(unitName);
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


Unit* Army::GetArmyUnitAt(int index) const
{
	if (index < m_army.size())
	{
		return m_army.at(index);
	}

	return nullptr;
}


int Army::GetArmySize() const
{
	return m_size;
}