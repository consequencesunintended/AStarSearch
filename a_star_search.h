#pragma once


#include "graphics_utility.h"
#include "graphics_colour.h"

#include <string>
#include <vector>
#include <set>
#include <iostream>

constexpr int				g_grid_size{ 40 };
constexpr int				g_box_size = 10.0f;



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

class A_STAR_SEARCH {
public:

	bool			update_search();
	void			reset();

	void			set_start_location(int i, int j);
	void			set_goal_location(int i, int j);
	void			set_wall_location(int i, int j);
	void			set_empty_location(int i, int j);

	bool			is_visited(int i, int j);
	bool			is_open(int i, int j);

protected:
	void			add_neighbours(int current_i, int current_j);

	void			add_open(int i, int j, float move_distance);
	void			remove_open(int i, int j);

	void			remove_visited(int i, int j);

	void			find_smallest_open();
	void			update_heuristic_values();


public:
	std::map<int, std::map<int, GRID_BLOCK>>	m_grid;
	std::vector<GRID_BLOCK*>					m_visited;
	std::vector<GRID_BLOCK*>					m_open;
	std::pair<int, int>							m_start_loc{ 3,4 };
	std::pair<int, int>							m_goal_loc{ 36,22 };
	std::pair<int, int>							m_current_loc;
	float										m_current_loc_step{ 0.0f };

};
