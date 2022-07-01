#pragma once

#include "GLFW/glfw3.h"
#include "graphics_colour.h"
#include "math_vector_2d.h"

#include <string>
#include <vector>
#include <map>

class PANEL_INTERFACE
{
public:
	virtual void init(void) = 0;
	virtual void draw(void) = 0;
	virtual void update(void) = 0;
	virtual void controls(GLFWwindow* window, int key, int scancode, int action, int mods) {
	}
	virtual void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	}
	virtual void mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {
	}
};

class GRAPHICS_UTILITY
{
public:
	static void push_translation_matrix(const MATH_VECTOR_2D& translation) {
		glPushMatrix();
		glTranslatef(translation.X, translation.Y, 0.0f);
	}

	static void pop_matrix() {
		glPopMatrix();
	}

	static void draw_line(const MATH_VECTOR_2D& start, const MATH_VECTOR_2D& end, const GRAPHICS_COLOUR& colour = GRAPHICS_COLOUR::White()) {

		glColor4fv(colour.GetRGBA());
		glBegin(GL_LINES);

		glVertex2f(start.X, start.Y);
		glVertex2f(end.X, end.Y);

		glEnd();

	}

	static void draw_line( const MATH_VECTOR_2D& start, const MATH_VECTOR_2D& end, const float rotation, const MATH_VECTOR_2D& rotation_point, const GRAPHICS_COLOUR& colour = GRAPHICS_COLOUR::White() ) {

		glColor4fv( colour.GetRGBA() );
		glBegin( GL_LINES );

		auto draw_rotated_point = [&]( const GLfloat x, const GLfloat y )
		{
			MATH_VECTOR_2D p{ x,y };
			p.RotateAroundOther( rotation, rotation_point );
			glVertex2f( p.X, p.Y );
		};

		draw_rotated_point( start.X, start.Y );
		draw_rotated_point( end.X, end.Y );

		glEnd();
	}

	static void draw_point(const MATH_VECTOR_2D& point, const GRAPHICS_COLOUR& colour = GRAPHICS_COLOUR::White()) {
		glColor4fv(colour.GetRGBA());
		glPointSize(1.0f);
		glBegin(GL_POINTS);

		glVertex2f(point.X, point.Y);

		glEnd();
	}

	static void draw_rectangle(const MATH_VECTOR_2D& centre, const float width, const float height, const bool fill = true, const GRAPHICS_COLOUR& colour = GRAPHICS_COLOUR::White()) {
		glColor4fv(colour.GetRGBA());
		if (fill) {
			glBegin(GL_QUADS);
		}
		else {
			glBegin(GL_LINE_LOOP);
		}

		glVertex2f(centre.X - width / 2.0f, centre.Y - height / 2.0f);
		glVertex2f(centre.X + width / 2.0f, centre.Y - height / 2.0f);
		glVertex2f(centre.X + width / 2.0f, centre.Y + height / 2.0f);
		glVertex2f(centre.X - width / 2.0f, centre.Y + height / 2.0f);

		glEnd();
	}

	static void draw_rectangle( const MATH_VECTOR_2D& centre, const float width, const float height, const float rotation, const MATH_VECTOR_2D& rotation_point, const bool fill = true, const GRAPHICS_COLOUR& colour = GRAPHICS_COLOUR::White() ) {
		glColor4fv( colour.GetRGBA() );
		if ( fill ) {
			glBegin( GL_QUADS );
		}
		else {
			glBegin( GL_LINE_LOOP );
		}

		auto draw_rotated_point = [&]( const GLfloat x, const GLfloat y )
		{
			MATH_VECTOR_2D p{ x,y };
			p.RotateAroundOther( rotation, rotation_point );
			glVertex2f( p.X, p.Y );
		};

		draw_rotated_point( centre.X - width / 2.0f, centre.Y - height / 2.0f );
		draw_rotated_point( centre.X + width / 2.0f, centre.Y - height / 2.0f );
		draw_rotated_point( centre.X + width / 2.0f, centre.Y + height / 2.0f );
		draw_rotated_point( centre.X - width / 2.0f, centre.Y + height / 2.0f );

		glEnd();
	}

};
