#pragma once
class Unit;

class Army
{
public:

	Army();
	Army(std::vector<std::string> order, int size);
	virtual ~Army() {}

	void InitArmy();
	Unit* GetArmyUnitAt(int index) const;
	int GetArmySize() const;


private:
	int m_size;
	std::vector<std::string> m_order;
	std::vector<Unit*> m_army;
};