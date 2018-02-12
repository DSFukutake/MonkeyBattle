#pragma once
#include "../libs/jsoncpp/dist/json/json.h"
#include "Game/GameData/Unit.h"
#include "Game/GameData/Army.h"

class Army;

class DataLibrary
{
public:
	static DataLibrary* GetInstance();

	void Cleanup();
	void LoadConfig();
	Unit GetUnit(std::string unitName) const;
	Army GetArmy(std::string armyId) const;

private:
	DataLibrary();

	std::map<std::string, Unit> m_units;
	std::map<std::string, Army> m_armies;

	void InitUnit(std::string unitName, Json::Value unitInfo, Json::Value unitAlterInfo = Json::Value::null);
	void InitArmy(Json::Value armyInfo, std::string armyId);

	Unit::UnitType GetTypeFromString(std::string typeStr);

};