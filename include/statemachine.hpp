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

#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <functional>
#include <map>
#include <type_traits>

/**
 * @brief   Manages states, represented by integer-compatible enums.
 */
class StateMachine
{
public:
    using Action = std::function<void()>;
    
public:
    /// modifiers:
    /**
     * @brief   Adds a state action. This action will be called in update_state()
     *          AFTER the current state changes AND the action set by set_update_action().
     * @note    Each state can only have one action.
     */
    template<class Enum, typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
    void add_state_action(Enum state, Action action);
    
    /**
     * @brief   Ready the next state. DOES NOT UPDATE THE CURRENT STATE!
     *          Use update_state() to update the readied state.
     */
    template<class Enum, typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
    void set_next_state(Enum state);
    
    /**
     * @brief   Set an action that will be called BEFORE the next state's action.
     * @note    Only the latest update action will be executed.
     */
    void set_update_action(Action action);
    
    /**
     * @brief   Updates the current state.
     */
    void update_state();
    
    /// accessors:
    template<class Enum, typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
    Enum get_state() const;
    
    template<class Enum, typename = typename std::enable_if<std::is_enum<Enum>::value>::type>
    bool state_is(Enum other) const;
    
private:
    std::map<int, Action> m_action_map;
    
    int m_current_state;
    bool m_has_current_state = false;
    
    int m_next_state;
    bool m_has_next_state = false;
    
    Action m_update_action;
};


/// inline implementation:
template<class Enum, typename>
inline void StateMachine::add_state_action(Enum state, Action action)
{
    m_action_map[static_cast<int>(state)] = action;
}

template<class Enum, typename>
inline void StateMachine::set_next_state(Enum state)
{
    m_next_state = static_cast<int>(state);
    m_has_next_state = true;
}

inline void StateMachine::set_update_action(Action action)
{
    m_update_action = action;
}

template<class Enum, typename>
inline Enum StateMachine::get_state() const
{
    return static_cast<Enum>(m_current_state);
}

template<class Enum, typename>
inline bool StateMachine::state_is(Enum other) const
{
    return m_has_current_state && m_current_state == static_cast<int>(other);
}

#endif /* statemachine_hpp */
