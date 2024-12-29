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
#ifndef WINTEN_CONTEXTCONTROLLER_HPP
#define WINTEN_CONTEXTCONTROLLER_HPP

// Include external header files
#include <memory>

// Include project header files
#include "istate.hpp"
#include "iview.hpp"

/// <summary>
///		The context for the state pattern which responds to controller actions.
/// </summary>
class ContextController
{
private:
	std::unique_ptr<IState> m_state;
	long double m_lastTime;
	long double m_latency;
	bool m_keyUp;
	bool m_keyDown;
	bool m_keyEscape;
	bool m_keyPressed;

public:
	std::unique_ptr<IView> m_view;
	ContextController(IState* state);
	void setView(std::unique_ptr<IView> view);
	void transitionTo(IState* state);
	void update(long double thisTime);
	void keyDown(bool state);
	void keyUp(bool state);
	void keyEscape(bool state);
	void keyPressed(void);
	void setAfterUpdateTime(long double timeAfterUpdate);
	void initialize(
		int newXOffset,
		int newYOffset,
		int newWidth,
		int newHeight);
	void shutdown(void);
};

#endif