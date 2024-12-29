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

// Include project header files
#include "contextcontroller.hpp"

/// <summary>
///		Class constructor.
/// </summary>
/// <param name="state">The initial state object.</param>
ContextController::ContextController(IState* state)
	: m_state(state)
	, m_view()
	, m_lastTime(0)
	, m_latency(0)
	, m_keyUp(false)
	, m_keyDown(false)
	, m_keyEscape(false)
	, m_keyPressed(false)
{
}

/// <summary>
///		Set the view ibject in use.
/// </summary>
void ContextController::setView(std::unique_ptr<IView> view)
{
	// Replace view
	std::unique_ptr<IView> p2;
	p2 = std::move(view);
	this->m_view = std::move(p2);// = std::move(view);
}

/// <summary>
///		Transition the context state.
/// </summary>
/// <param name="state">The new state.</param>
void ContextController::transitionTo(IState* state)
{
	m_state.reset(state);
}

/// <summary>
///		Update the current state.
/// </summary>
/// <param name="thisTime">Current timestamp in seconds.</param>
void ContextController::update(long double thisTime)
{
	// Time difference
	long double deltaT = thisTime - m_lastTime;

	// Update the current state
	if (!m_lastTime == 0)
		m_state->update(
			deltaT,
			m_keyUp,
			m_keyDown,
			m_keyEscape,
			m_keyPressed);

	// Render the current state
	m_view->DrawAll(m_state.get(), 1 / deltaT, m_latency);

	// Save time for future update
	m_lastTime = thisTime;

	// Reset key press
	m_keyPressed = false;
}

/// <summary>
///		Down key press.
/// </summary>
/// <param name="state">State of the key.</param>
void ContextController::keyDown(bool state)
{
	m_keyDown = state;
}

/// <summary>
///		Up key press.
/// </summary>
/// <param name="state">State of the key.</param>
void ContextController::keyUp(bool state)
{
	m_keyUp = state;
}

/// <summary>
///		Escape key press.
/// </summary>
/// <param name="state">State of the key.</param>
void ContextController::keyEscape(bool state)
{
	m_keyEscape = state;
}

/// <summary>
///		Any key pressed.
/// </summary>
/// <param name="">True if any key has been pressed.</param>
void ContextController::keyPressed(void)
{
	m_keyPressed = true;
}

/// <summary>
///		Stores the timestamp after update for performance monitoring.
/// </summary>
/// <param name="timeAfterUpdate">Timestamp in seconds after update.</param>
void ContextController::setAfterUpdateTime(long double timeAfterUpdate)
{
	m_latency = timeAfterUpdate - m_lastTime;
}

/// <summary>
///		Initialize the display.
/// </summary>
/// <param name="newXOffset">Margin on left of view in pixels.</param>
/// <param name="newYOffset">Margin at top of view in pixels.</param>
/// <param name="newWidth">Width of view in pixels.</param>
/// <param name="newHeight">Height of view in pixels.</param>
void ContextController::initialize(
	int newXOffset,
	int newYOffset,
	int newWidth,
	int newHeight)
{
	m_view->initialize(newXOffset, newYOffset, newWidth, newHeight);
}

/// <summary>
///		Shuts dow the view.
/// </summary>
void ContextController::shutdown(void)
{
	m_view->shutdown();
}