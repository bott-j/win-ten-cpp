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
#ifndef WINTEN_WINTEN_CONSTANTS_HPP
#define WINTEN_WINTEN_CONSTANTS_HPP

namespace winten_constants {
	
	// Field
	const float FIELD_BORDER = 0.025;

	// Paddle
	const float PADDLE_HEIGHT = 0.15;
	const float PADDLE_WIDTH = 0.015;
	const float PADDLE_MAX_Y = 1.0 - 0.0075 - 0.15;
	const float PADDLE_MIN_Y = 0.0075 + 0.15;
	const float PADDLE_SPEED = 0.5;
	const float PADDLE_X_NPC = 0.1;
	const float PADDLE_X_PLAYER = 0.9;

	// Ball
	const float BALL_DIAMETER = 0.015;
	const float BALL_MAX_X = 1 - 0.015/2 - 0.05;
	const float BALL_MIN_X = 0.015/2 + 0.05;
	const float BALL_MAX_Y = 1 - 0.015/2 - 0.05;
	const float BALL_MIN_Y = 0.015/2 + 0.05;

	const float BALL_MAX_THETA = 3.14 / 3;
	const float BALL_MIN_THETA = -3.14 / 3;
	const float BALL_SPEED = 0.5;
	const float BALL_ANGLE_NOISE = -3.14 / 10;
	const float NPC_HORIZON = 0.3;
}

#endif
