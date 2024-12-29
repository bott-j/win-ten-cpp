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
#ifndef WINTEN_ISTATE_HPP
#define WINTEN_ISTATE_HPP

// Include external header files
#include <string>

// Include project header files
#include "vector2d.hpp"

/// <summary>
///		Defines the state interface for the state pattern.
/// </summary>
class IState
{
public:
	vector2d<double> player;
	vector2d<double> npc;
	vector2d<double> ball;
	std::string message;
	int scorePlayer;
	int scoreNpc;

	virtual IState *update(
		double deltaT,
		bool keyUp,
		bool keyDown,
		bool keyEscape,
		bool keyPressed) = 0;
};

#endif