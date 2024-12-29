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
#ifndef WINTEN_IVIEW_HPP
#define WINTEN_IVIEW_HPP

// Include project header files
#include "istate.hpp"

/// <summary>
///		Interface class for the view object.
/// </summary>
class IView
{
public:
	virtual void DrawAll(
		IState* state,
		float fps,
		float latency) = 0;
	virtual void initialize(
		int newXOffset,
		int newYOffset,
		int newWidth,
		int newHeight) = 0;
	virtual void shutdown(void) = 0;
};

#endif