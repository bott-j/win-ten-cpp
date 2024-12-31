/*
* Copyright (C) 2024 James Bott
*
* This program is free software: you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program. If not, see <https://www.gnu.org/licenses/>.
*/
#ifndef WINTEN_CStateIntro_HPP
#define WINTEN_CStateIntro_HPP

// Include external header fiels
#include <ctime>
#include <memory>

// Include project header files
#include "istate.hpp"
#include "vector2d.hpp"

/// <summary>
///     Implements the player vs NPC game state.
/// </summary>
class CStateIntro : public IState
{
private:
    // State of ball velocity
    float m_ballSpeed;
    float m_ballAngle;
    bool m_ballDirection;
    std::time_t m_startTime;

public:
    CStateIntro();
    std::unique_ptr<IState> update(
        float deltaT,
        bool keyUp,
        bool keyDown,
        bool keyEscape,
        bool keyPressed) override;
};

#endif