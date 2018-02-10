#include "stdafx.h"
#include "StateMachine.h"
#include "Game.h"


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


void StateMachine::Update()
{
	auto game = Game::GetInstance();
	switch (m_currentState)
	{
		case stateM_state::k_state_init:
			{
				if(game->GetIsInitialized() == false)
				{
					game->Init();
				}
				else 
				{
					ChangeState(1);
				}
			}
			break;
		case stateM_state::k_state_loading:
			{
				if (game->IsLoading() == false && game->IsReady() == false)
				{
					Game::GetInstance()->Load();
				}
				if (game->IsLoading() == false && game->IsReady() == true)
				{
					ChangeState(2);
				}
			}
			break;
		case stateM_state::k_state_game:
			{
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
				Game::GetInstance()->Cleanup();
				LOG("Shut down");
			}
			break;
	}
}

//Handle state changes
void StateMachine::ChangeState(int stateIdx)
{
	m_currentState = (stateM_state)(1 <<  stateIdx);
}

//Return the current active state
StateMachine::stateM_state StateMachine::GetCurrentState()
{
	return m_currentState;
}
