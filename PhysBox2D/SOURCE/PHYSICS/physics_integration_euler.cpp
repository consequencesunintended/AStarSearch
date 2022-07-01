#include "physics_integration_euler.h"

void PHYSICS_INTEGRATION_EULER::Update( PHYSICS_ENTITY& physics_entity, const float delta_time )
{
#ifdef ENABLE_SEMI_EULER
	UpdatePosition( physics_entity, delta_time );
	UpdateVelocity( physics_entity, delta_time );
#else
	UpdateVelocity( physics_entity, delta_time );
	UpdatePosition( physics_entity, delta_time );
#endif
}

void PHYSICS_INTEGRATION_EULER::UpdateVelocity( PHYSICS_ENTITY& physics_entity, const float delta_time )
{
	physics_entity.m_velocity = physics_entity.m_acceleration * delta_time + physics_entity.m_velocity;
}

void PHYSICS_INTEGRATION_EULER::UpdatePosition( PHYSICS_ENTITY& physics_entity, const float delta_time )
{
	physics_entity.m_position = physics_entity.m_velocity * delta_time + physics_entity.m_position;
}
