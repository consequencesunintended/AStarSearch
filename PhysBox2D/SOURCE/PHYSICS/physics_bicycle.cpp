#include "physics_bicycle.h"


void PHYSICS_BICYCLE::update(float dt) {

	if (dt > 0.0f) 
	{
		// FIXME: LH: in the end - reevaluate the need for this check

		if( m_current_steering_angle != 0 && m_current_speed != 0 )
		{
			// Re-evaluate turn circle, only if steering angle changes - 
			// otherwise, it should be constant and the car should continue to rotate around it
			if ( m_target_steering_angle > m_current_steering_angle )
			{
				m_current_turn_circle_rotation = 0;

				// Convert steering angle to radians
				const float m_current_steering_angle_rad = m_current_steering_angle * 3.14f / 180;

				// Calculate turn radius and back wheel radius
				const float turn_radius = wheel_distance() / sin( m_current_steering_angle_rad );
				const float back_wheel_r = wheel_distance() / tan( m_current_steering_angle_rad );

				// Find centre of turning radius circle
				const MATH_VECTOR_2D debug_rear_axle = rear_axle();
				float new_x = m_current_steering_angle_rad > 0 ? debug_rear_axle.X + back_wheel_r : debug_rear_axle.X - back_wheel_r;
				const MATH_VECTOR_2D rotation_circle_center{ new_x, debug_rear_axle.Y };

				m_rotation_circle_center = rotation_circle_center;

			}

			
			// Rotate forward acceleration vector around said circle
			MATH_VECTOR_2D acceleration{ 0, ( m_target_speed - m_current_speed ) / dt };

			const float one_degree_in_rad{ 0.0174533 };
			m_current_turn_circle_rotation += one_degree_in_rad;

			acceleration.RotateAroundOther( m_current_turn_circle_rotation, m_rotation_circle_center /*rotation_circle_center*/ );

			// if < 0, we're turning counterclockwise, else we're turning clockwise
			//const float target_x = m_current_steering_angle_rad > 0 ? cosf( m_current_turn_circle_rotation ) - sinf( m_current_turn_circle_rotation ) : cosf( m_current_turn_circle_rotation ) + sinf( m_current_turn_circle_rotation );
			//const float target_y = m_current_steering_angle_rad > 0 ? sinf( m_current_turn_circle_rotation ) + cosf( m_current_turn_circle_rotation ) : - sinf(m_current_turn_circle_rotation) + cosf( m_current_turn_circle_rotation );

			//const MATH_VECTOR_2D target_pos{ target_x, target_y };
			m_last_target_pos = acceleration; //target_pos;


			// FIXME: LH: do we need to do this?
			// Get the target direction & multiply by the acceleration magnitude
			MATH_VECTOR_2D target_direction{ acceleration - front_axle() }; //target_pos - front_axle() };
			target_direction.normalise();

			m_acceleration = target_direction * acceleration.GetLength();


			// FIXME: LH: do we need to do this actually? As the other acceleration vector is already rotated?
			
			// Rotate acceleration vector to match the car's rotation
			//m_acceleration.RotateR( m_rotation );

			m_last_acceleration = m_acceleration;


			// FIXME: LH: the car's rotation needs to be equal to the angle between 0 and the acceleration vector (the one rotating around the circle)
			

			//m_rotation += m_current_steering_angle_rad;
			//m_rotation = m_current_steering_angle_rad;
		}

		//else
		//{
		//	MATH_VECTOR_2D acceleration{ 0, 0 };
		//	acceleration.Y = ( m_target_speed - m_current_speed ) / dt;
		//	acceleration.RotateR( m_rotation );

		//	m_acceleration = acceleration;
		//}

		//m_acceleration.X = 0;
		//m_acceleration.Y = ( m_target_speed - m_current_speed ) / dt;
		//m_acceleration.RotateR( m_rotation );

		// Remove lateral velocity
		//MATH_VECTOR_2D right_normal{ 1,0 };
		//right_normal.RotateR( m_rotation );

		//const float lateral_velocity = right_normal.GetDotProduct( m_velocity );
		//const MATH_VECTOR_2D& scaled_lateral_velocity = right_normal * 0.95f * lateral_velocity;

		//m_velocity -= scaled_lateral_velocity;

		m_current_steering_angle += ( m_target_steering_angle - m_current_steering_angle );
	}

	m_physics_integration_verlet.Update(*this, dt);
	m_current_speed += ( m_target_speed - m_current_speed );
}

void PHYSICS_BICYCLE::reset_physics() {
	m_position = { 0.0f, 0.0f };
	m_acceleration = { 0.0f, 0.0f };
	m_velocity = { 0.0f, 0.0f };
	m_current_speed = 0.0f;
	m_target_speed = 0.0f;
	m_target_steering_angle = 0.0f;
	m_current_steering_angle = 0.0f;
	m_rotation = 0.0f;

	// DEBUG
	m_rotation_circle_center.Assign( 0, 0 );
	m_last_target_pos.Assign( 0, 0 );
}
