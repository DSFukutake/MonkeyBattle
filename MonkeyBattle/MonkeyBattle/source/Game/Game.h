#pragma once


#include "Player/Player.h"

class Game
{
public:
	static Game* GetInstance();

	~Game();
	std::pair<Player*, Player*> m_players;
	
	void Init();
	void Load();
	void StartGame();
	void Update();
	void Finish();
	void Cleanup();

	bool GetIsInitialized() const;
	bool IsLoading() const;
	bool IsReady() const;
	bool ShouldTerminate() const;
	bool IsGameSetup() const;


private:
	Game();
	void BattleStep();
	Unit* SetBattleUnit(int& index, Player* player);
	void DoBattle(Unit* p1Unit, Unit* p2Unit);
	bool CheckWinCondition();


	bool m_isInitialized;
	bool m_isLoading;
	bool m_isReady;
	bool m_shouldTerminate;
	bool m_isGameSetup;
	bool m_isInBattle;

	std::pair<int, int> m_armyIndex;
	int m_round;
};