#pragma once

class StateMachine 
{
	
public:
	
	enum stateM_state 
	{
		k_state_init	= 1,
		k_state_loading = 1 << 1,
		k_state_game	= 1 << 2,
		k_state_finsh	= 1 << 3,
		k_state_done	= 1 << 4
	};

	static StateMachine* GetInstance();
	virtual ~StateMachine() {}


	void Update();
	void ChangeState(int stateIdx);
	stateM_state GetCurrentState();

private:

	StateMachine();
	stateM_state m_currentState;

};