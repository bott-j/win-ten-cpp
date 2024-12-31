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
// Include external header files
#include <memory>
#include <random>
#include <ctime>

// include project header files
#include "cstatedemo.hpp"
#include "cstategame.hpp"
#include "cstateintro.hpp"
#include "winten_constants.hpp"
#include "vector2d.hpp"

/// <summary>
///     Default class constructor.
/// </summary>
CStateIntro::CStateIntro()
    : m_startTime(0)
{
    npc.x = winten_constants::PADDLE_X_NPC;
    npc.y = 0.5;
    player.x = winten_constants::PADDLE_X_PLAYER;
    player.y = 0.5;
    ball.x = 0.5;
    ball.y = 0.5;
    m_ballDirection = false;
    m_ballAngle = 0;
    m_ballSpeed = winten_constants::BALL_SPEED;
    scoreNpc = 0;
    scorePlayer = 0;

    // Save the start time in this state
    m_startTime = std::time(0);

    // Set the intro screen message
    message = "        WINTEN\nPRESS KEY TO START";
    
    // Seed the random number generator with current time
    std::srand(static_cast<unsigned int>(std::time(0)));
}

/// <summary>
///     Updates the game world during this state.
/// </summary>
/// <param name="deltaT">Time difference in seconds between updates.</param>
/// <param name="keyUp">State of the up key.</param>
/// <param name="keyDown">State of the down key.</param>
/// <param name="keyEscape">State of the escape key.</param>
/// <param name="keyPressed">True if any key pressed.</param>
/// <returns>The next state.</returns>
std::unique_ptr<IState> CStateIntro::update(
    float deltaT,
    bool keyUp,
    bool keyDown,
    bool keyEscape,
    bool keyPressed)
{
    std::unique_ptr<IState> nextState(nullptr);
    std::time_t thisTime;

    // Get the current time
    thisTime = std::time(0);

    // If key pressed transition to player-vs-npc game
    if (keyPressed)
    {
        nextState = std::move(std::make_unique<CStateGame>());
        // Enter the player-vs-npc state
        return nextState;
    }
    // If timeout transition to demo game
    else if ((thisTime - m_startTime) > 10)
    {
        nextState = std::move(std::make_unique<CStateDemo>());
        // Enter the player-vs-npc state
        return nextState;
    }

    return nextState;
}