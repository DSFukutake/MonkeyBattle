#include "stdafx.h"
#include "DataLibrary.h"
#include <fstream>

//Handle the DataLibrary singleton
DataLibrary* DataLibrary::GetInstance()
{
	static const std::unique_ptr<DataLibrary> s_instance(new DataLibrary());
	return s_instance.get();
}

//Ctor
DataLibrary::DataLibrary()
{
	Cleanup();
}

//Clean internal data.
void DataLibrary::Cleanup()
{
	m_units.clear();
	m_armies.clear();
}

//Load the json config with all the data.
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
	//close file stream
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
	SetIntData(newUnitData.m_damage, unitInfo["damage"], unitAlterInfo["damage"]);
	SetIntData(newUnitData.m_health, unitInfo["health"], unitAlterInfo["health"]);
	SetTypeData(newUnitData.m_type, unitInfo["type"], unitAlterInfo["type"]);
	SetTypeData(newUnitData.m_preferred, unitInfo["preferred_target"], unitAlterInfo["preferred_target"]);
	
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

//Set the value to integer field from the fieldValue or if null from alterFieldValue instead
void DataLibrary::SetIntData(int& field, Json::Value fieldValue, Json::Value alterFieldValue)
{
	if (fieldValue == Json::Value::null)
	{
		field = alterFieldValue.asInt();
	}
	else
	{
		field = fieldValue.asInt();
	}
}

//Set the value to UnitType field from the fieldValue or if null from alterFieldValue instead
void DataLibrary::SetTypeData(Unit::UnitType& field, Json::Value fieldValue, Json::Value alterFieldValue)
{
	std::string type = "";
	if (fieldValue == Json::Value::null)
	{
		type = alterFieldValue.asString();
	}
	else
	{
		type = fieldValue.asString();
	}

	field = GetTypeFromString(type);
}
