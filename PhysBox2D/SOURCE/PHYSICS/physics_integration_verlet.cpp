#include "physics_integration_verlet.h"
#include <algorithm>


void PHYSICS_INTEGRATION_VERLET::Update(PHYSICS_ENTITY& physics_entity, const float delta_time) {
	auto new_pos = physics_entity.m_velocity * delta_time + physics_entity.m_acceleration * delta_time * delta_time * 0.5f;
	auto new_vel = physics_entity.m_acceleration * delta_time;

	physics_entity.m_position = physics_entity.m_position + new_pos;
	physics_entity.m_velocity = physics_entity.m_velocity + new_vel;
}
