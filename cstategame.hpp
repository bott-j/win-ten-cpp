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
#ifndef WINTEN_CSTATEGAME_HPP
#define WINTEN_CSTATEGAME_HPP

// Include project header files
#include "cstategame.hpp"
#include "istate.hpp"
#include "vector2d.hpp"

/// <summary>
///     Implements the player vs NPC game state.
/// </summary>
class CStateGame : public IState
{
private:
    // State of ball velocity
    double m_ballSpeed;
    double m_ballAngle;
    bool m_ballDirection;

public:
    CStateGame();
    IState* update(
        double deltaT,
        bool keyUp,
        bool keyDown,
        bool keyEscape,
        bool keyPressed) override;
    // Handle keystroke movement of paddle
    void updatePlayer(
        bool keyUp, 
        bool keyDown, 
        vector2d<double>& paddle, 
        double delta);
    // Handle NPC paddle
    void updateNpc(
        vector2d<double>& paddle, 
        vector2d<double>& ball, 
        double delta);
    bool isCollision(vector2d<double>& paddle);
    // Handle dynamics of ball
    bool updateBall(double delta);
};

#endif