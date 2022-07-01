#pragma once

// -- LOCAL

// .. REFERENCES

#include "physics_entity.h"

// -- GLOBAL

// .. TYPES

class PHYSICS_COLLISION_2D
{
	// -- PUBLIC

	public:

	// .. OPERATIONS

	void CheckForCollision(
		PHYSICS_ENTITY& entity,
		const float damping_coefficient,
		const float min_x_value,
		const float max_x_value,
		const float min_y_value,
		const float max_y_value,
		const MATH_POINT_2D& position = MATH_POINT_2D( 0.0f, 0.0f )
	)
	{
		if ( entity.m_position.Y < min_y_value + position.Y )
		{
			if ( fabs( entity.m_velocity.Y ) < 1.0f )
			{
				entity.m_velocity = MATH_VECTOR_2D( entity.m_velocity.X, 0.0f );
				entity.m_position = MATH_VECTOR_2D( entity.m_position.X, min_y_value );
			}
			else
			{
				entity.m_velocity = MATH_VECTOR_2D( entity.m_velocity.X, -entity.m_velocity.Y * damping_coefficient );
				entity.m_position = MATH_VECTOR_2D( entity.m_position.X, min_y_value + 0.001f );
			}
		}
	}
};