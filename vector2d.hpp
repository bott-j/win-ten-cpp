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
#ifndef WINTEN_VECTOR2D_HPP
#define WINTEN_VECTOR2D_HPP

/// <summary>
///     Defines a point in 2D coordinates.
/// </summary>
template <typename T>
class vector2d
{
public:
    T x;
    T y;

    /// <summary>
    ///   Class constructor.
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    vector2d(T x, T y)
        : x(x)
        , y(y)
    {
    }

    /// <summary>
    ///   Default class constructor.
    /// </summary>
    vector2d()
        : x(0)
        , y(0)
    {
    }

    /// <summary>
    ///   Addition operator overload.
    /// </summary>
    /// <param name="other">Right operand.</param>
    /// <returns>The resulting vector.</returns>
    vector2d operator +(vector2d const& other) 
    {
        vector2d result;
        result.x = x + other.x;
        result.y = y + other.y;
        return result;
    }

    /// <summary>
    ///   Subtraction operator overload.
    /// </summary>
    /// <param name="other">Right operand.</param>
    /// <returns>The resulting vector.</returns>
    vector2d operator -(vector2d const& other) 
    {
        vector2d result;
        result.x = x - other.x;
        result.y = y - other.y;
        return result;
    }
};

#endif
