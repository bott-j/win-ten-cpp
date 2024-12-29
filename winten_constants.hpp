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
	const float FIELD_BORDER = 0.025f;

	// Paddle
	const float PADDLE_HEIGHT = 0.15f;
	const float PADDLE_WIDTH = 0.015f;
	const float PADDLE_MAX_Y = 1.0f - 0.0075f - 0.15f;
	const float PADDLE_MIN_Y = 0.0075f + 0.15f;
	const float PADDLE_SPEED = 0.5f;
	const float PADDLE_X_NPC = 0.1f;
	const float PADDLE_X_PLAYER = 0.9f;

	// Ball
	const float BALL_DIAMETER = 0.015f;
	const float BALL_MAX_X = 1.0f - 0.015f/2.0f - 0.05f;
	const float BALL_MIN_X = 0.015f/2.0f + 0.05f;
	const float BALL_MAX_Y = 1.0f - 0.015f/2.0f - 0.05f;
	const float BALL_MIN_Y = 0.015f/2.0f + 0.05f;

	const float BALL_MAX_THETA = 3.14f / 3.0f;
	const float BALL_MIN_THETA = -3.14f / 3.0f;
	const float BALL_SPEED = 0.5f;
	const float BALL_ANGLE_NOISE = -3.14f / 10.0f;
	const float NPC_HORIZON = 0.3f;
	const float PI = 3.141592654f;
	const float ASPECT_RATIO = 640.0f / 480.0f;
}

#endif
