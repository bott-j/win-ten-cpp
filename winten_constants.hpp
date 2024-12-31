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

#include <ctime>

namespace winten_constants {
	// General
	const float PI = 3.141592654f;
	// Canvas size
	const float W = 640.0f;
	const float H = 480.0f;
	const float ASPECT_RATIO =  W / H;
	// Border
	const float FIELD_BORDER = 15.0f;
	// Paddle
	const float PADDLE_HEIGHT = 40.0f;
	const float PADDLE_WIDTH = 10.0f;
	const float PADDLE_MAX_Y = H - FIELD_BORDER - PADDLE_HEIGHT/2.0f;
	const float PADDLE_MIN_Y = FIELD_BORDER + PADDLE_HEIGHT/2.0f;
	const float PADDLE_SPEED = H / 2.0f;
	const float PADDLE_X_NPC = 2.0f * FIELD_BORDER;
	const float PADDLE_X_PLAYER = W - 2.0f * FIELD_BORDER;
	// Ball
	const float BALL_DIAMETER = 10.0f;
	const float BALL_MAX_X = W - BALL_DIAMETER/2.0f - FIELD_BORDER;
	const float BALL_MIN_X = BALL_DIAMETER/2.0f + FIELD_BORDER;
	const float BALL_MAX_Y = H - BALL_DIAMETER / 2.0f - FIELD_BORDER;
	const float BALL_MIN_Y = BALL_DIAMETER / 2.0f + FIELD_BORDER;
	const float BALL_MAX_THETA = PI / 3.0f;
	const float BALL_MIN_THETA = -PI / 3.0f;
	const float BALL_SPEED = W/2.0f;
	const float BALL_ANGLE_NOISE = -PI / 10.0f;
	const float NPC_HORIZON = 0.3f * W;
	// Number of second before demo starts in intro screen
	const time_t DELAY_DEMO = 10; 
}

#endif
