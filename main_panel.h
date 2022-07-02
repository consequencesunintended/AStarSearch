#pragma once


#include "graphics_utility.h"
#include "graphics_colour.h"
#include "physics_entity.h"
#include "physics_bicycle.h"
#include "graphics_clipboard.h"
#include "graphics_image_pixel_loader.h"

#include <string>
#include <vector>
#include <set>
#include <iostream>

constexpr float				g_panel_width{ 1024.0f };
constexpr float				g_panel_height{ 620.0f };
constexpr int				g_grid_size{ 40 };


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


	bool			is_visited(int i, int j);
	bool			is_open(int i, int j);
	void			add_neighbours(int current_i, int current_j);
	void			add_open(int i, int j, float move_distance);
	void			remove_open(int i, int j);
	void			remove_visited(int i, int j);
	void			find_smallest_open();

private:

	enum BLOCK_TYPE
	{
		BLOCKED,
		EMPTY,
		START,
		GOAL
	};

	struct GRID_BLOCK {
		float		cost;
		BLOCK_TYPE	type;
		float		h_value;
		int			i_value;
		int			j_value;
		float		g_value;
		std::vector<GRID_BLOCK*> path;
	};
	std::map<int, std::map<int, GRID_BLOCK>>	m_grid;
	std::vector<GRID_BLOCK*>					m_visited;
	std::vector<GRID_BLOCK*>					m_open;
	std::pair<int, int>							m_start_loc{ 3,4 };
	std::pair<int, int>							m_goal_loc{ 36,22 };
	std::pair<int, int>							m_current_loc;
	float										m_current_loc_step = 0;

};
