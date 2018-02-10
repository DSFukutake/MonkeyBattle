#include "stdafx.h"
#include "Game.h"
#include "DataLibrary/DataLibrary.h"


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
}


void Game::Init()
{
	//Call game config
	LOG("Initialize Game\n");
	m_isInitialized = true;
}

void Game::Load()
{
	m_isLoading = true;
	DataLibrary::GetInstance()->LoadConfig();
	LOG("Loading game data\n");

	m_isLoading = false;
	m_isReady = true;
}

void Game::StartGame()
{
	Army army1 = DataLibrary::GetInstance()->GetArmy("type1");
	Army army2 = DataLibrary::GetInstance()->GetArmy("type2");
	Player player1 = Player(army1);
	player1.SetArmyIdName("Army 1");
	player1.Init();
	
	Player player2 = Player(army2);
	player2.SetArmyIdName("Army 2");
	player2.Init();

	m_players = std::make_pair(player1, player2);
	m_armyIndex = std::make_pair(0, 0);
	m_isGameSetup = true;
	m_isInBattle = true;
	m_round = 1;
	LOG_GAME_INFO("\n -- BATTLE -- \n");
}


void Game::Update()
{
	if (m_isInBattle)
	{
		BattleStep();
	}
}

void Game::Finish()
{
	m_shouldTerminate = true;
}

void Game::Cleanup()
{
	DataLibrary::GetInstance()->Cleanup();
}

void Game::BattleStep()
{
	//Select units for battle, unit must be alive on the player's army.
	//Each player could have different indexes so we keep an index for each separately.
	Unit* p1Unit = SetBattleUnit(m_armyIndex.first, m_players.first);
	m_armyIndex.first++;
	Unit* p2Unit = SetBattleUnit(m_armyIndex.second, m_players.second);
	m_armyIndex.second++;
	DoBattle(p1Unit, p2Unit);
	bool gameFinished = CheckWinCondition();
	if (gameFinished)
	{
		Finish();
	}
	
}


Unit* Game::SetBattleUnit(int& index, Player player)
{
	int armySize = player.GetArmySize();
	
	if (index >= armySize)
	{
		index = 0;
	}

	if (player.GetUnit(index)->IsAlive())
	{
		return player.GetUnit(index);
	}
	else
	{
		index++;
		return SetBattleUnit(index, player);
	}
}

void Game::DoBattle(Unit* p1Unit, Unit* p2Unit)
{

	Unit::UnitData u1Data = p1Unit->GetUnitData();
	Unit::UnitData u2Data = p2Unit->GetUnitData();
	int dmgToP1 = (p1Unit->IsPreferred(u2Data.m_preferred)) ? u2Data.m_damage : u2Data.m_damage / 2;
	int dmgToP2 = (p2Unit->IsPreferred(u1Data.m_preferred)) ? u1Data.m_damage : u1Data.m_damage / 2;
	
	LOG_GAME_INFO("Round %d >> ", m_round);
	LOG_GAME_INFO("Player 1: %s (health=%d, totalDamage=%d) ", u1Data.m_name.c_str(), u1Data.m_health, u1Data.m_damage);
	LOG_GAME_INFO(" vs Player 2: %s (health=%d, totalDamage=%d) \n", u2Data.m_name.c_str(), u2Data.m_health, u2Data.m_damage);
	int healthRemain1 = u1Data.m_health;
	int healthRemain2 = u2Data.m_health;
	p1Unit->TakeDamage(dmgToP1);
	p2Unit->TakeDamage(dmgToP2);

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

bool Game::CheckWinCondition()
{
	bool result = false;
	if (m_players.first.IsDefeated())
	{
		result = true;
		m_players.second.PrintWinnerArmy();
	}
	else if (m_players.second.IsDefeated())
	{
		result = true;
		m_players.first.PrintWinnerArmy();
	}

	return result;
}


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