

#include "statemachine.hpp"


void StateMachine::update_state()
{
    if (!m_has_next_state)
        return;
    if (m_has_current_state && m_current_state == m_next_state)
        return;
    
    m_current_state = m_next_state;
    m_has_current_state = true;
    m_has_next_state = false;
    
    if (m_update_action)
        m_update_action();
    
    if (m_action_map[m_current_state])
        m_action_map[m_current_state]();
}
