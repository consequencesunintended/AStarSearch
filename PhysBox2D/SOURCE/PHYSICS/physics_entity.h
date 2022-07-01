
#pragma once

// -- LOCAL

// .. REFERENCES

#include "../MATH/math_vector_2d.h"
#include <assert.h>

// -- GLOBAL

// .. TYPES

struct ENTITY
{
	MATH_VECTOR_2D	m_position;
};

struct PHYSICS_ENTITY : public ENTITY
{
	MATH_VECTOR_2D	m_velocity;
	MATH_VECTOR_2D	m_acceleration;
};
