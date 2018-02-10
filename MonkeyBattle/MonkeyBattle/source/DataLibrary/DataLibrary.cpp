#include "stdafx.h"
#include "DataLibrary.h"
#include <fstream>



DataLibrary* DataLibrary::GetInstance()
{
	static const std::unique_ptr<DataLibrary> s_instance(new DataLibrary());
	return s_instance.get();
}

DataLibrary::DataLibrary()
{
	Cleanup();
}

void DataLibrary::Cleanup()
{
	m_units.clear();
	m_armies.clear();
}
void DataLibrary::LoadConfig()
{
	std::ifstream config_file(CONFIG_FILE, std::ifstream::binary);

	Json::Value config;
	config_file >> config;

	if (config.isMember("units"))
	{
		//Read all the unit info
		Json::Value units = config["units"];
		for (Json::ValueIterator it = units.begin(); it != units.end();it++)
		{
			std::string name = it.key().asString();			
			Json::Value unitInfo = units[name];
			if (unitInfo.isMember("base_unit"))
			{
				std::string baseName = unitInfo["base_unit"].asString();
				Json::Value baseUnit = units[baseName];
				InitUnit(name, unitInfo, baseUnit);
			}
			else 
			{
				InitUnit(name, unitInfo);
			}
		}
	}
	if (config.isMember("army_presets"))
	{
		Json::Value armies = config["army_presets"];
		for (Json::ValueIterator it = armies.begin(); it != armies.end(); it++)
		{
			std::string name = it.key().asString();
			Json::Value armyInfo = armies[name];
			InitArmy(armyInfo, name);

		}
	}

	
	config_file.close();
}


Unit DataLibrary::GetUnit(std::string unitName) const
{
	std::map<std::string, Unit>::const_iterator it = m_units.find(unitName);
	if (it != m_units.cend())
	{
		return it->second;
	}
	return Unit();
}

Army DataLibrary::GetArmy(std::string armyId) const
{
	std::map<std::string, Army>::const_iterator it = m_armies.find(armyId);
	if (it != m_armies.cend())
	{
		return it->second;
	}
	return Army();
}


void DataLibrary::InitUnit(std::string unitName, Json::Value unitInfo, Json::Value unitAlterInfo)
{
	Unit::UnitData newUnitData;

	newUnitData.m_name = unitName;
	
	if (unitInfo.isMember("damage"))
	{
		newUnitData.m_damage = unitInfo["damage"].asInt();
	}
	else
	{
		newUnitData.m_damage = unitAlterInfo["damage"].asInt();
	}
	//health
	if (unitInfo.isMember("health"))
	{
		newUnitData.m_health = unitInfo["health"].asInt();
	}
	else
	{
		newUnitData.m_health = unitAlterInfo["health"].asInt();
	}
	//Type
	if (unitInfo.isMember("type"))
	{
		std::string type = unitInfo["type"].asString();
		newUnitData.m_type = GetTypeFromString(type);
	}
	else
	{
		std::string type = unitAlterInfo["type"].asString();
		newUnitData.m_type = GetTypeFromString(type);
	}
	//Preffered
	if (unitInfo.isMember("preferred_target"))
	{
		std::string type = unitInfo["preferred_target"].asString();
		newUnitData.m_preferred = GetTypeFromString(type);
	}
	else
	{
		std::string type = unitAlterInfo["preferred_target"].asString();
		newUnitData.m_preferred = GetTypeFromString(type);
	}

	Unit newUnit = Unit(newUnitData);
	m_units.emplace(unitName, newUnit);
}

void DataLibrary::InitArmy(Json::Value armyInfo, std::string armyId)
{
	int size = armyInfo["size"].asInt();
	Json::Value orderArray = armyInfo["order"];
	std::vector<std::string> unitOrder;
	unitOrder.clear();
	for (Json::ValueIterator it = orderArray.begin(); it != orderArray.end(); it++)
	{
		std::string unitName = orderArray[it.index()].asString();
		unitOrder.push_back(unitName);
	}

	Army newArmy = Army(unitOrder, size);
	m_armies.emplace(armyId, newArmy);
}



Unit::UnitType DataLibrary::GetTypeFromString(std::string typeStr)
{
	Unit::UnitType type = Unit::UnitType::k_type_none;
	if (typeStr == "Air")
	{
		type = Unit::UnitType::k_type_air;
	}
	else if (typeStr == "Ground")
	{
		type = Unit::UnitType::k_type_ground;
	}
	else if (typeStr == "All")
	{
		type = Unit::UnitType::k_type_all;
	}

	return type;
}