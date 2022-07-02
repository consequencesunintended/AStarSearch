#include "main_panel.h"
#include "imgui.h"
#include "implot.h"
#include "fundamental_delta_time.h"

#include <iostream>


constexpr int box_size = 10.0f;


void MAIN_PANEL::controls(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_UP) {



	}
	if (key == GLFW_KEY_DOWN) {
	}

	if (key == GLFW_KEY_LEFT) {
	}
	if (key == GLFW_KEY_RIGHT) {
	}

	if (key == GLFW_KEY_E && action == 0) {

	}
}

void MAIN_PANEL::mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {

}

void MAIN_PANEL::mouse_callback(GLFWwindow* window, double xpos, double ypos) {

}
void MAIN_PANEL::init(void) {
	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {
			m_grid[i][j].type = EMPTY;
			m_grid[i][j].cost = -1.0f;
			m_grid[i][j].i_value = i;
			m_grid[i][j].j_value = j;
		}
	}
	m_grid[m_start_loc.first][m_start_loc.second].type = START;
	m_grid[m_goal_loc.first][m_goal_loc.second].type = GOAL;

	for (int i = 4; i < 25; i++) {
		m_grid[20][i].type = BLOCKED;
	}


	for (int i = 4; i < 25; i++) {
		m_grid[i][25].type = BLOCKED;
	}

	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {
			m_grid[i][j].h_value = (MATH_VECTOR_2D(i, j) - MATH_VECTOR_2D(m_goal_loc.first, m_goal_loc.second)).GetLength();
		}
	}
	m_current_loc = m_start_loc;
}

void MAIN_PANEL::reset(void) {
}

void ImGuiToggleButton(const char* str_id, bool* v) {


}

void MAIN_PANEL::draw_ui(void) {

}

bool  MAIN_PANEL::is_visited(int i, int j) {
	for (int k = 0; k < m_visited.size(); k++) {
		if ((m_visited[k]->i_value == i) && (m_visited[k]->j_value == j)) {
			return true;
		}
	}
	return false;
}

bool  MAIN_PANEL::is_open(int i, int j) {
	for (int k = 0; k < m_open.size(); k++) {
		if ((m_open[k]->i_value == i) && (m_open[k]->j_value == j)) {
			return true;
		}
	}
	return false;
}

void  MAIN_PANEL::add_open(int i, int j, float move_distance) {

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

void  MAIN_PANEL::remove_visited(int i, int j) {

	for (size_t k = 0; k < m_visited.size(); k++) {
		if (m_visited[k]->i_value == i && m_visited[k]->j_value == j) {
			m_visited.erase(m_visited.begin() + k);
			break;
		}
	}
}

void  MAIN_PANEL::remove_open(int i, int j) {

	for (size_t k = 0; k < m_open.size(); k++) {
		if (m_open[k]->i_value == i && m_open[k]->j_value == j) {
			m_open.erase(m_open.begin() + k);
			break;
		}
	}
}

void MAIN_PANEL::find_smallest_open() {
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

void  MAIN_PANEL::add_neighbours(int current_i, int current_j) {
	add_open(current_i + 1,current_j, 1.0f);
	add_open(current_i - 1,current_j, 1.0f);
	add_open(current_i,current_j + 1, 1.0f);
	add_open(current_i,current_j - 1, 1.0f);

	add_open(current_i + 1, current_j + 1, 1.5f);
	add_open(current_i + 1, current_j - 1, 1.5f);
	add_open(current_i - 1, current_j + 1, 1.5f);
	add_open(current_i - 1, current_j - 1, 1.5f);
}

void MAIN_PANEL::draw(void) {
	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {

			BLOCK_TYPE type = m_grid[i][j].type;
			glPushMatrix();
			glTranslatef(-g_grid_size / 2.0f * box_size, -g_grid_size / 2.0f * box_size, 0.0f);


			GRAPHICS_COLOUR colour = GRAPHICS_COLOUR::Grey();


			if (type == START) {
				colour = GRAPHICS_COLOUR::Blue();
			}
			else if (type == GOAL) {
				colour = GRAPHICS_COLOUR::Red();
			} else if (type == BLOCKED) {
				colour = GRAPHICS_COLOUR::Black();
			} else if (is_visited(i, j)) {
				colour = GRAPHICS_COLOUR::Brown();
			} else if (is_open(i, j)) {
				colour = GRAPHICS_COLOUR::Yellow();
			}

			GRAPHICS_UTILITY::draw_rectangle(MATH_VECTOR_2D(i * box_size, j * box_size), box_size, box_size, true, colour);
			glPopMatrix();
		}
	}

	for (size_t i = 0; i < m_grid[m_current_loc.first][m_current_loc.second].path.size(); i++) {
		int current_i = m_grid[m_current_loc.first][m_current_loc.second].path[i]->i_value;
		int current_j = m_grid[m_current_loc.first][m_current_loc.second].path[i]->j_value;


		glPushMatrix();
		glTranslatef(-g_grid_size / 2.0f * box_size, -g_grid_size / 2.0f * box_size, 0.0f);

		GRAPHICS_UTILITY::draw_rectangle(MATH_VECTOR_2D(current_i * box_size, current_j * box_size), box_size, box_size, true, GRAPHICS_COLOUR::Orange());
		glPopMatrix();
	}
}


void MAIN_PANEL::update() {
	// did we reach the goal yet
	if (m_current_loc.first != m_goal_loc.first || m_current_loc.second != m_goal_loc.second) {

		// add all neighbouring blocks as open nodes if they are not blocked/visited/opened
		add_neighbours(m_current_loc.first, m_current_loc.second);

		// find the open node with the smallest cost and add it as visited
		find_smallest_open();
	}
}


