/*
 *      Copyright (C) 2020 Johnathan Law
 *
 *      This file is part of SWL.
 *
 *      SWL is free software: you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation, either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      SWL is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with SWL.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

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
