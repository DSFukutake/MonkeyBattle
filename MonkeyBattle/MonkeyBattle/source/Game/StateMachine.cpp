#include "stdafx.h"
#include "StateMachine.h"
#include "Game.h"

//Singleton handler
StateMachine* StateMachine::GetInstance()
{
	static const std::unique_ptr<StateMachine> s_instance( new StateMachine() );
	return s_instance.get();
}

StateMachine::StateMachine()
{
	LOG("Create StateMachine\n");
	m_currentState = stateM_state::k_state_init;
}

//Handle state machine update and loop
void StateMachine::Update()
{
	auto game = Game::GetInstance();
	switch (m_currentState)
	{
		case stateM_state::k_state_init:
			{
				//Initialize game
				if(game->GetIsInitialized() == false)
				{
					game->Init();
				}
				ChangeState(1);
			}
			break;
		case stateM_state::k_state_loading:
			{
				//Load configurations
				if (game->IsLoading() == false && game->IsReady() == false)
				{
					Game::GetInstance()->Load();
				}
				ChangeState(2);
			}
			break;
		case stateM_state::k_state_game:
			{
				//If game is not setup initialize things and then loop in the state until
				//win conditions are met
				if (!game->IsGameSetup())
				{
					game->StartGame();
				}
				else
				{
					game->Update();
					if (game->ShouldTerminate())
					{
						ChangeState(3);
					}
				}
			}
			break;
		case stateM_state::k_state_finsh:
			{
				//Finalize steps
				Game::GetInstance()->Cleanup();
				ChangeState(4);
				LOG("Shut down");
			}
			break;
		case stateM_state::k_state_done:
			break;
	}
}

//Change to state specified by stateIdx
void StateMachine::ChangeState(int stateIdx)
{
	m_currentState = (stateM_state)(1 <<  stateIdx);
}

//Return the current active state
StateMachine::stateM_state StateMachine::GetCurrentState()
{
	return m_currentState;
}
