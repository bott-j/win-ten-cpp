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
#ifndef WINTEN_CSTATEDEMO_HPP
#define WINTEN_CSTATEDEMO_HPP

// Include external header fiels
#include <memory>

// Include project header files
#include "istate.hpp"
#include "vector2d.hpp"

/// <summary>
///     Implements the player vs NPC game state.
/// </summary>
class CStateDemo : public IState
{
private:
    // State of ball velocity
    float m_ballSpeed;
    float m_ballAngle;
    bool m_ballDirection;

public:
    CStateDemo();
    std::unique_ptr<IState> update(
        float deltaT,
        bool keyUp,
        bool keyDown,
        bool keyEscape,
        bool keyPressed) override;
    // Handle keystroke movement of paddle
    void updatePlayer(
        bool keyUp,
        bool keyDown,
        vector2d<float>& paddle,
        float delta);
    // Handle NPC paddle
    void updateNpc(
        vector2d<float>& paddle,
        vector2d<float>& ball,
        float delta);
    bool isCollision(vector2d<float>& paddle);
    // Handle dynamics of ball
    bool updateBall(float delta);
};

#endif