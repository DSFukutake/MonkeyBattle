#include "stdafx.h"
#include "Game/StateMachine.h"


int main()
{
	auto gameSM = StateMachine::GetInstance();
	
	if(gameSM != nullptr)
	{
		while (gameSM->GetCurrentState() != StateMachine::stateM_state::k_state_done)
		{
			gameSM->Update();
		}
	}
	system("pause");
    return 0;
}

