#pragma once

#include "physics_entity.h"
#include "physics_integration_verlet.h"
#include <algorithm>

class PHYSICS_BICYCLE : public PHYSICS_ENTITY {
public:
	void							set_target_speed(const float target_speed);
	float							target_speed() const;
	float							current_speed() const;

	void							set_centre(const MATH_VECTOR_2D& centre);
	const MATH_VECTOR_2D&			centre() const;

	const float						wheel_distance() const;
	const MATH_VECTOR_2D			front_axle() const;
	const MATH_VECTOR_2D			rear_axle() const;

	const MATH_VECTOR_2D&			acceleration() const;

	void							update(float dt);
	void							reset_physics();

	float							m_front_axle = 4.0f;
	float							m_rear_axle = -4.0f;
	float							m_target_steering_angle = 0.0f;
	float							m_current_steering_angle{ 0.0f };
	float							m_max_acceleartion_mss = 10.0f;
	float							m_min_decelration_mss = -10.0f;
	float							m_max_speed_ms = 90.0f;

protected:
	float							m_rotation{ 0.0f }; // Current bike rotation in radians
	float							m_current_turn_circle_rotation{ 0.0f }; // in radians

	// DEBUG:
	MATH_VECTOR_2D					m_rotation_circle_center{ 0,0 };
	MATH_VECTOR_2D					m_last_target_pos{ 0,0 };
	MATH_VECTOR_2D					m_last_acceleration{ 0,0 };

private:

	float							m_target_speed{ 0.0f };
	float							m_current_speed{ 0.0f };
	PHYSICS_INTEGRATION_VERLET		m_physics_integration_verlet;
};


inline const MATH_VECTOR_2D PHYSICS_BICYCLE::front_axle() const
{
	MATH_VECTOR_2D v{ 0, m_front_axle };
	v.RotateR( m_rotation );

	return centre() + v;
}

inline const MATH_VECTOR_2D PHYSICS_BICYCLE::rear_axle() const
{
	MATH_VECTOR_2D v{ 0, m_rear_axle };
	v.RotateR( m_rotation );

	return centre() + v;
}

inline void PHYSICS_BICYCLE::set_target_speed(const float target_speed) {
	m_target_speed = std::max(std::min(target_speed, m_max_speed_ms), 0.0f);
}

inline float PHYSICS_BICYCLE::target_speed() const {
	return m_target_speed;
}

inline void PHYSICS_BICYCLE::set_centre(const MATH_VECTOR_2D& centre)
{
	m_position = centre;
}

inline const MATH_VECTOR_2D& PHYSICS_BICYCLE::centre() const {
	return m_position;
}

inline const float PHYSICS_BICYCLE::wheel_distance() const
{
	return m_front_axle - m_rear_axle;
}

inline const MATH_VECTOR_2D& PHYSICS_BICYCLE::acceleration() const
{
	return m_acceleration;
}

inline float PHYSICS_BICYCLE::current_speed() const
{
	return m_current_speed;
}
