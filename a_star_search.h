#pragma once


#include "graphics_utility.h"
#include "graphics_colour.h"

#include <string>
#include <vector>
#include <set>
#include <iostream>



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

	bool										update_search();
	void										reset();

	void										set_start_location(int i, int j);
	void										set_goal_location(int i, int j);
	void										set_wall_location(int i, int j);
	void										set_empty_location(int i, int j);

	bool										is_visited(int i, int j);
	bool										is_open(int i, int j);
	bool										has_open_node() { return !m_open.empty(); }


	BLOCK_TYPE									get_block_type(int i, int j)	{ return m_grid[i][j].type; }
	std::pair<int, int>							get_start_location()			{ return m_start_loc; }
	std::pair<int, int>							get_goal_location()				{ return m_goal_loc; }
	std::pair<int, int>							get_current_location()			{ return m_current_loc; }
	std::vector<GRID_BLOCK*>					get_current_path()				{ return m_grid[m_current_loc.first][m_current_loc.second].path; }

protected:
	void										add_neighbours(int current_i, int current_j);

	void										add_open(int i, int j, float move_distance);
	void										remove_open(int i, int j);

	void										remove_visited(int i, int j);

	void										find_smallest_open();
	void										update_heuristic_values();

	std::map<int, std::map<int, GRID_BLOCK>>	m_grid;
	std::vector<GRID_BLOCK*>					m_visited;
	std::vector<GRID_BLOCK*>					m_open;
	std::pair<int, int>							m_start_loc{ 3,4 };
	std::pair<int, int>							m_goal_loc{ 36,22 };
	std::pair<int, int>							m_current_loc;
	float										m_current_loc_step{ 0.0f };

};
