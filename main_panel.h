#pragma once


#include "graphics_utility.h"
#include "graphics_colour.h"
#include "a_star_search.h"

#include <string>
#include <vector>
#include <set>
#include <iostream>

constexpr float				g_panel_width{ 1024.0f };
constexpr float				g_panel_height{ 620.0f };
constexpr int				g_grid_size{ 40 };
constexpr int				g_box_size = 10.0f;

class MAIN_PANEL : public PANEL_INTERFACE {
public:
	void			init() override;
	void			reset();
	void			draw_ui();
	void			draw() override;
	void			update() override;
	void			controls(GLFWwindow* window, int key, int scancode, int action, int mods) override;
	void			mouse_callback(GLFWwindow* window, double xpos, double ypos) override;
	void			mouse_click_callback(GLFWwindow* window, int button, int action, int mods) override;

private:

	A_STAR_SEARCH								m_a_star_search;

	float										m_curosr_x;
	float										m_curosr_y;
	bool										m_start_search{ false };
	bool										m_initial_step_done{ false };
	bool										m_left_down{ false };
	bool										m_right_down{ false };
	bool										m_shift_down{ false };
	bool										m_ctrl_down{ false };
	bool										m_goal_reached{ false };

};
