#pragma once

#include <memory>
#include <stack>
#include <mutex>

#include "States.h"

namespace DX
{

	typedef std::unique_ptr<States> StatesRef;

	class StateMachine
	{
	public:
		StateMachine();
		~StateMachine();

		void AddState(StatesRef newState, bool isReplacing = true);
		void RemoveState();

		void ProcessChanges();

		StatesRef &GetActiveState();

	private:
		std::stack<StatesRef> _states;
		StatesRef _newState;

		bool _isAdding;
		bool _isReplacing;
		bool _isRemoving;
	};
}
