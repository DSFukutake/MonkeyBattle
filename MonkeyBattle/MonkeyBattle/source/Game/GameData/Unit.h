#pragma once

class Unit
{
public:

	enum UnitType
	{
		k_type_none = 0,
		k_type_ground,
		k_type_air,
		k_type_all,
	};

	struct UnitData
	{
		std::string m_name;
		int m_damage;
		int m_health;
		UnitType m_type;
		UnitType m_preferred;
	};

	Unit();
	Unit(UnitData data);
	virtual ~Unit() {}

	void SetDataFrom(Unit* unit);

	void IncreaseDamage(int amount);
	void TakeDamage(int amount);
	UnitData GetUnitData() const;
	bool IsAlive();
	bool IsPreferred(UnitType type);
	int GetDamageBinaryMVPValue();


private:

	UnitData m_data;

};