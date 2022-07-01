#pragma once

#include "physics_integration.h"
#include "physics_entity.h"

class PHYSICS_INTEGRATION_VERLET : public PHYSICS_INTEGRATION {

public:

	PHYSICS_INTEGRATION_VERLET(void) = default;
	~PHYSICS_INTEGRATION_VERLET(void) = default;

	void Update(PHYSICS_ENTITY& physics_entity, const float delta_time) override;
};