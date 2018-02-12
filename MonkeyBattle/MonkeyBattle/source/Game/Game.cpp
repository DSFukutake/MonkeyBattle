#include "stdafx.h"
#include "Game.h"
#include "DataLibrary/DataLibrary.h"

//Singleton handler
Game* Game::GetInstance()
{
	static const std::unique_ptr<Game> s_instance(new Game());
	return s_instance.get();
}


Game::Game()
	:m_isInitialized(false)
	, m_isLoading(false)
	, m_isReady(false)
	, m_shouldTerminate(false)
	, m_isGameSetup(false)
	, m_isInBattle(false)
	, m_round(0)
{
	LOG("Create Game");
}

Game::~Game()
{
	delete m_players.first;
	delete m_players.second;
}

//Initialize Game
void Game::Init()
{
	LOG("Initialize Game\n");
	m_isInitialized = true;
}

//Load configurations from Json, general loading process.
void Game::Load()
{
	m_isLoading = true;
	DataLibrary::GetInstance()->LoadConfig();
	LOG("Loading game data\n");

	m_isLoading = false;
	m_isReady = true;
}

//Initialize both players and their armies
void Game::StartGame()
{
	Army* army1 = DataLibrary::GetInstance()->GetArmy("type1");
	if (army1 != nullptr)
	{
		m_players.first = new  Player(army1);
		m_players.first->SetArmyIdName("Army 1");
		m_players.first->Init();
	}

	Army* army2 = DataLibrary::GetInstance()->GetArmy("type2");
	if(army2 != nullptr)
	{
		m_players.second = new Player(army2);
		m_players.second->SetArmyIdName("Army 2");
		m_players.second ->Init();
	}

	m_armyIndex = std::make_pair(0, 0);
	m_isGameSetup = true;
	m_isInBattle = true;
	m_round = 1;
	LOG_GAME_INFO("\n -- BATTLE -- \n");
}

//Game Loop
void Game::Update()
{
	if (m_isInBattle)
	{
		BattleStep();
	}
}

//Finalize steps
void Game::Finish()
{
	m_shouldTerminate = true;
}

//Do any needed cleanup
void Game::Cleanup()
{
	//Handle DataLibrary finish
}

//Execute a battle step which consists of units fighting and then checking for win conditions
void Game::BattleStep()
{
	//Select units for battle, unit must be alive on the player's army.
	//Each player could have different indexes so we keep an index for each separately.
	Unit* p1Unit = SetBattleUnit(m_armyIndex.first, m_players.first);
	m_armyIndex.first++;
	Unit* p2Unit = SetBattleUnit(m_armyIndex.second, m_players.second);
	m_armyIndex.second++;
	if(p1Unit != nullptr && p2Unit != nullptr)
	{
		DoBattle(p1Unit, p2Unit);
		bool gameFinished = CheckWinCondition();
		if (gameFinished)
		{
			Finish();
		}
	}
}

//Returns the unit a player will use
//The selected unit will be the immediate next unit available(is alive)
Unit* Game::SetBattleUnit(int& index, Player* player)
{
	if (player == nullptr)
	{
		return nullptr;
	}

	//no neede to select if player already been defeated
	//This will also prevent it from looping infinitely as a player that is not defeated
	//will have at least 1 unit alive
	if (player->IsDefeated() == false)
	{

		int armySize = player->GetArmySize();

		if (index >= armySize)
		{
			index = 0;
		}

		Unit* playerUnit = player->GetUnit(index);
		//Check if the unit is alive
		if (playerUnit != nullptr && playerUnit->IsAlive())
		{
			return playerUnit;
		}
		else
		{
			//Increment index and call recursively and wrap around if needed
			index++;
			return SetBattleUnit(index, player);
		}
	}
}

//Execute battle, deal damage to each unit and make neccessary adjustments
void Game::DoBattle(Unit* p1Unit, Unit* p2Unit)
{
	//Sanity check
	if(p1Unit == nullptr || p2Unit == nullptr)
	{
		return;
	}

	Unit::UnitData u1Data = p1Unit->GetUnitData();
	Unit::UnitData u2Data = p2Unit->GetUnitData();
	//If a unit is not the preferred type of the attacker it will take half damage
	int dmgToP1 = (p1Unit->IsPreferred(u2Data.m_preferred)) ? u2Data.m_damage : u2Data.m_damage / 2;
	int dmgToP2 = (p2Unit->IsPreferred(u1Data.m_preferred)) ? u1Data.m_damage : u1Data.m_damage / 2;
	
	LOG_GAME_INFO("Round %d >> ", m_round);
	LOG_GAME_INFO("Player 1: %s (health=%d, totalDamage=%d) ", u1Data.m_name.c_str(), u1Data.m_health, u1Data.m_damage);
	LOG_GAME_INFO(" vs Player 2: %s (health=%d, totalDamage=%d) \n", u2Data.m_name.c_str(), u2Data.m_health, u2Data.m_damage);
	//Store the current health of the units
	int healthRemain1 = u1Data.m_health;
	int healthRemain2 = u2Data.m_health;
	//Apply damage
	p1Unit->TakeDamage(dmgToP1);
	p2Unit->TakeDamage(dmgToP2);

	//Post Combat if a unit is killed, the attacker gains damage equal to the remaining health
	//before the battle.
	if (!p1Unit->IsAlive())
	{
		p2Unit->IncreaseDamage(healthRemain1);
		LOG_GAME_INFO("Player 1: %s was killed \n", u1Data.m_name.c_str());
	}
	
	if (!p2Unit->IsAlive())
	{
		p1Unit->IncreaseDamage(healthRemain2);
		LOG_GAME_INFO("Player 2: %s was killed \n", u2Data.m_name.c_str());
	}

	m_round++;
	
}
//Check win conditions, if all units in a player's army are killed that player lost
bool Game::CheckWinCondition()
{
	bool result = false;
	if (m_players.first != nullptr && m_players.first->IsDefeated())
	{
		result = true;
		m_players.second->PrintWinnerArmy();
	}
	else if (m_players.second != nullptr && m_players.second->IsDefeated())
	{
		result = true;
		m_players.first->PrintWinnerArmy();
	}

	return result;
}

//Accesor functions for members in Game
bool Game::GetIsInitialized() const
{
	return m_isInitialized;
}

bool Game::IsLoading() const
{
	return m_isLoading;
}
bool Game::IsReady() const
{
	return m_isReady;
}
bool Game::ShouldTerminate() const
{
	return m_shouldTerminate;
}

bool Game::IsGameSetup() const
{
	return m_isGameSetup;
}