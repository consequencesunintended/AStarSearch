#include "a_star_search.h"

#include <iostream>


void A_STAR_SEARCH::set_goal_location(int i, int j) {
	if (m_grid[i][j].type != START) {
		m_grid[m_goal_loc.first][m_goal_loc.second].type = EMPTY;
		m_grid[i][j].type = GOAL;
		m_goal_loc.first = i;
		m_goal_loc.second = j;
		update_heuristic_values();
	}
}

void A_STAR_SEARCH::set_start_location(int i, int j) {
	if (m_grid[i][j].type != GOAL) {
		m_grid[m_start_loc.first][m_start_loc.second].type = EMPTY;
		m_grid[i][j].type = START;
		m_start_loc.first = i;
		m_start_loc.second = j;
		m_current_loc = m_start_loc;
		update_heuristic_values();
	}
}

void A_STAR_SEARCH::set_empty_location(int i, int j) {
	if (m_grid[i][j].type != GOAL || m_grid[i][j].type != START) {
		m_grid[i][j].type = EMPTY;
		m_grid[i][j].i_value = i;
		m_grid[i][j].j_value = j;
	}
}

void A_STAR_SEARCH::set_wall_location(int i, int j) {
	if (m_grid[i][j].type != GOAL || m_grid[i][j].type != START) {
		m_grid[i][j].type = BLOCKED;
		m_grid[i][j].i_value = i;
		m_grid[i][j].j_value = j;
	}
}


void A_STAR_SEARCH::update_heuristic_values() {

	for (auto& entity : m_grid) {
		for (auto& internal_entity : entity.second) {
			internal_entity.second.h_value = (MATH_VECTOR_2D(float(internal_entity.second.i_value), float(internal_entity.second.j_value)) - MATH_VECTOR_2D(float(m_goal_loc.first), float(m_goal_loc.second))).GetLength();
		}
	}
}

void A_STAR_SEARCH::reset(void) {
	m_open.clear();
	m_visited.clear();

	for (auto &entity: m_grid) {
		for(auto &internal_entity: entity.second) {
			internal_entity.second.cost = -1.0f;
			internal_entity.second.h_value = 0.0f;
			internal_entity.second.g_value = 0.0f;
			internal_entity.second.path.clear();
		}
	}
	update_heuristic_values();
	m_current_loc = m_start_loc;
}


bool  A_STAR_SEARCH::is_visited(int i, int j) {
	for (int k = 0; k < m_visited.size(); k++) {
		if ((m_visited[k]->i_value == i) && (m_visited[k]->j_value == j)) {
			return true;
		}
	}
	return false;
}

bool  A_STAR_SEARCH::is_open(int i, int j) {
	for (int k = 0; k < m_open.size(); k++) {
		if ((m_open[k]->i_value == i) && (m_open[k]->j_value == j)) {
			return true;
		}
	}
	return false;
}

void  A_STAR_SEARCH::add_open(int i, int j, float move_distance) {

	if (m_grid[i][j].type != BLOCKED) {

		float new_g_value = float(m_current_loc_step) + move_distance;
		float cost = new_g_value + m_grid[i][j].h_value;

		if (m_grid[i][j].cost == -1.0f || cost < m_grid[i][j].cost) {
			m_grid[i][j].g_value = new_g_value;
			m_grid[i][j].cost = m_grid[i][j].g_value + m_grid[i][j].h_value;

			m_grid[i][j].path = m_grid[m_current_loc.first][m_current_loc.second].path;
			m_grid[i][j].path.push_back(&m_grid[m_current_loc.first][m_current_loc.second]);

			remove_visited(i, j);
		}

		if (!is_open(i, j) && !is_visited(i, j)) {
			m_open.push_back(&m_grid[i][j]);
		}
	}
}

void  A_STAR_SEARCH::remove_visited(int i, int j) {

	for (size_t k = 0; k < m_visited.size(); k++) {
		if (m_visited[k]->i_value == i && m_visited[k]->j_value == j) {
			m_visited.erase(m_visited.begin() + k);
			break;
		}
	}
}

void  A_STAR_SEARCH::remove_open(int i, int j) {

	for (size_t k = 0; k < m_open.size(); k++) {
		if (m_open[k]->i_value == i && m_open[k]->j_value == j) {
			m_open.erase(m_open.begin() + k);
			break;
		}
	}
}

void A_STAR_SEARCH::find_smallest_open() {
	float cost = 100000.0;
	GRID_BLOCK* smallest_block = nullptr;

	for (size_t k = 0; k < m_open.size(); k++) {

		if (m_open[k]->cost < cost || (smallest_block && m_open[k]->cost == smallest_block->cost && m_open[k]->h_value < smallest_block->h_value)) {
			cost = m_open[k]->cost;
			smallest_block = m_open[k];
		}
	}

	if (smallest_block) {
		m_current_loc.first = smallest_block->i_value;
		m_current_loc.second = smallest_block->j_value;
		m_current_loc_step = smallest_block->g_value;

		m_visited.push_back(smallest_block);
		remove_open(smallest_block->i_value, smallest_block->j_value);
	}
}

void  A_STAR_SEARCH::add_neighbours(int current_i, int current_j) {
	add_open(current_i + 1, current_j, 1.0f);
	add_open(current_i - 1, current_j, 1.0f);
	add_open(current_i, current_j + 1, 1.0f);
	add_open(current_i, current_j - 1, 1.0f);

	add_open(current_i + 1, current_j + 1, 1.5f);
	add_open(current_i + 1, current_j - 1, 1.5f);
	add_open(current_i - 1, current_j + 1, 1.5f);
	add_open(current_i - 1, current_j - 1, 1.5f);
}


bool A_STAR_SEARCH::update_search() {
	if (m_current_loc.first != m_goal_loc.first || m_current_loc.second != m_goal_loc.second) {

		// add all neighbouring blocks as open nodes if they are not blocked/visited/opened
		add_neighbours(m_current_loc.first, m_current_loc.second);

		// find the open node with the smallest cost and add it as visited
		find_smallest_open();

		return true;
	}
	return false;
}
