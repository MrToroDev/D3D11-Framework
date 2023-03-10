#include "StateMachine.h"
#include <thread>
#include <future>
#include <memory>

using namespace DX;

StateMachine::StateMachine()
{
    
}

StateMachine::~StateMachine()
{

}

void StateMachine::AddState(StatesRef newState, bool isReplacing)
{
    this->_isAdding = true;
    this->_isReplacing = isReplacing;

    this->_newState = std::move(newState);
}

void StateMachine::RemoveState()
{
    this->_isRemoving = true;
}

void StateMachine::ProcessChanges()
{
    if (this->_isRemoving && !this->_states.empty())
    {
        this->_states.pop();

        if (!this->_states.empty())
        {
            this->_states.top()->Resume();
        }

        this->_isRemoving = false;
    }

    if (this->_isAdding)
    {
        if (!this->_states.empty())
        {
            if (this->_isReplacing)
            {
                this->_states.pop();
            }
            else
            {
                this->_states.top()->Pause();
            }
        }

        this->_states.push(std::move(this->_newState));

        std::thread _l([this]() {
            _states.top()->init();
        });
        if (_l.joinable()) _l.join();
        this->_isAdding = false;
    }
}

StatesRef& StateMachine::GetActiveState()
{
    return this->_states.top();
}