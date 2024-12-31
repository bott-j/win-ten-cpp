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

// include project header files
#include "cstatedemo.hpp"
#include "cstateintro.hpp"
#include "winten_constants.hpp"
#include "vector2d.hpp"

/// <summary>
///     Default class constructor.
/// </summary>
CStateDemo::CStateDemo()
    : m_ballDirection(false)
    , m_ballAngle(0)
    , m_ballSpeed(winten_constants::BALL_SPEED)
{
    // Initialise coordinates
    npc.x = winten_constants::PADDLE_X_NPC;
    npc.y = winten_constants::H / 2.0f;
    player.x = winten_constants::PADDLE_X_PLAYER;
    player.y = winten_constants::H / 2.0f;
    ball.x = winten_constants::W / 2.0f;
    ball.y = winten_constants::H / 2.0f;

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
std::unique_ptr<IState> CStateDemo::update(
    float deltaT,
    bool keyUp,
    bool keyDown,
    bool keyEscape,
    bool keyPressed)
{
    std::unique_ptr<IState> nextState(nullptr);

    if (keyPressed)
    {
        nextState = std::move(std::make_unique<CStateIntro>());
        // Enter the intro screen state
        return nextState;
    }
    else
    {
        //updatePlayer(keyUp, keyDown, _paddlePlayer);
        updateNpc(player, ball, deltaT);
        updateNpc(npc, ball, deltaT);
        updateBall(deltaT);
    }

    return nextState;
}

/// <summary>
///     Handle keystroke movement of paddle
/// </summary>
/// <param name="keyUp">Key up state.</param>
/// <param name="keyDown">Key down state.</param>
/// <param name="paddle">Paddle location.</param>
/// <param name="delta">Difference in time between updates.</param>
void CStateDemo::updatePlayer(
    bool keyUp,
    bool keyDown,
    vector2d<float>& paddle,
    float delta)
{
    // If keystroke up
    if (keyUp)
    {
        paddle.y -= winten_constants::PADDLE_SPEED * delta;
        if (paddle.y < winten_constants::PADDLE_MIN_Y)
            paddle.y = winten_constants::PADDLE_MIN_Y;
    }
    // If keystroke down
    if (keyDown)
    {
        paddle.y += winten_constants::PADDLE_SPEED * delta;
        if (paddle.y > winten_constants::PADDLE_MAX_Y)
            paddle.y = winten_constants::PADDLE_MAX_Y;
    }
}

/// <summary>
///     Handle NPC paddle.
/// </summary>
/// <param name="paddle">Position of the NPC paddle.</param>
/// <param name="ball">Position of the ball.</param>
/// <param name="delta">Difference in time between updates.</param>
void CStateDemo::updateNpc(
    vector2d<float>& paddle,
    vector2d<float>& ball,
    float delta)
{
    // If within visible horizon
    if (std::fabs(ball.x - paddle.x) <= winten_constants::NPC_HORIZON)
    {
        // Move in direction of ball
        if (ball.y > paddle.y)
            paddle.y += winten_constants::PADDLE_SPEED * delta;
        if (ball.y < paddle.y)
            paddle.y -= winten_constants::PADDLE_SPEED * delta;
        paddle.y = fmin(paddle.y, winten_constants::PADDLE_MAX_Y);
        paddle.y = fmax(paddle.y, winten_constants::PADDLE_MIN_Y);
    }
}

/// <summary>
///     Collision detectio nbetween ball and paddle.
/// </summary>
/// <param name="paddle">The paddle location</param>
/// <returns>True on collision.</returns>
bool CStateDemo::isCollision(vector2d<float>& paddle)
{
    vector2d<float> distanceFromPaddle;

    distanceFromPaddle = paddle - ball;

    return std::fabs(distanceFromPaddle.x) < (winten_constants::BALL_DIAMETER / 2.0 + winten_constants::PADDLE_WIDTH / 2.0)
        && std::fabs(distanceFromPaddle.y) < (winten_constants::BALL_DIAMETER / 2.0 + winten_constants::PADDLE_HEIGHT / 2.0);
}

/// <summary>
///     Update dynamics of ball.
/// </summary>
/// <param name="delta">Difference in time between updates.</param>
/// <returns>True on collision with left or right surface.</returns>
bool CStateDemo::updateBall(float delta)
{
    bool result = false;
    float newX;
    float newY;

    // Calculate next position of the ball
    newX = ball.x + cos((m_ballDirection ? winten_constants::PI : 0.0f) + m_ballAngle) * m_ballSpeed * delta;
    newY = ball.y + sin((m_ballDirection ? winten_constants::PI : 0.0f) + m_ballAngle) * m_ballSpeed * delta;

    // If collission detected with top or bottom surfaces
    if (newY > winten_constants::BALL_MAX_Y)
    {
        // Reflect about normal
        m_ballAngle = -m_ballAngle;
        newY = winten_constants::BALL_MAX_Y;
    }
    if (newY < winten_constants::BALL_MIN_Y)
    {
        // Reflect about normal
        m_ballAngle = -m_ballAngle;
        newY = winten_constants::BALL_MIN_Y;
    }

    // If colission left or right detected with right surface
    if (ball.x > winten_constants::BALL_MAX_X)
    {
        result = true;
        scoreNpc++;
        m_ballDirection = !m_ballDirection;
        m_ballAngle = -m_ballAngle;
        newX = winten_constants::BALL_MAX_X;
    }
    else if (ball.x < winten_constants::BALL_MIN_X)
    {
        result = true;
        scorePlayer++;
        m_ballDirection = !m_ballDirection;
        m_ballAngle = -m_ballAngle;
        newX = winten_constants::BALL_MIN_X;
    }

    // If collission detected with paddle
    if (isCollision(player))
    {
        m_ballDirection = !m_ballDirection;
        m_ballAngle = -m_ballAngle;
        m_ballAngle += winten_constants::BALL_ANGLE_NOISE * static_cast<float>(std::rand()) / RAND_MAX;
        m_ballAngle = std::fmin(m_ballAngle, winten_constants::BALL_MAX_THETA);
        m_ballAngle = std::fmax(m_ballAngle, winten_constants::BALL_MIN_THETA);

        newX = player.x - winten_constants::PADDLE_WIDTH / 2.0f - winten_constants::BALL_DIAMETER / 2.0f - 0.001f;
    }
    if (isCollision(npc))
    {
        m_ballDirection = !m_ballDirection;
        m_ballAngle = -m_ballAngle;
        m_ballAngle += winten_constants::BALL_ANGLE_NOISE * static_cast<float>(std::rand()) / RAND_MAX;
        m_ballAngle = std::fmin(m_ballAngle, winten_constants::BALL_MAX_THETA);
        m_ballAngle = std::fmax(m_ballAngle, winten_constants::BALL_MIN_THETA);

        newX = npc.x + winten_constants::PADDLE_WIDTH / 2.0f + winten_constants::BALL_DIAMETER / 2.0f + 0.001f;
    }

    // Update the position of the ball
    ball.x = newX;
    ball.y = newY;

    return result;
}
