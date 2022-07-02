#include "main_panel.h"
#include "imgui.h"
#include "implot.h"
#include "fundamental_delta_time.h"

#include <iostream>


int get_box_location(int coord) {
	return coord * g_box_size + (-g_grid_size / 2.0f * g_box_size);
}

int get_coord_from_location(int location) {
	return (location - (-g_grid_size / 2.0f * g_box_size)) / g_box_size;
}

void MAIN_PANEL::controls(GLFWwindow* window, int key, int scancode, int action, int mods) {
}

void MAIN_PANEL::mouse_click_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		m_left_down = action == GLFW_PRESS;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		m_right_down = action == GLFW_PRESS;
	}
}

void MAIN_PANEL::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	m_curosr_x = xpos;
	m_curosr_y = ypos;

	if (m_left_down || m_right_down) {
		float location_x = m_curosr_x - g_panel_width / 2.0f;
		float location_y = -m_curosr_y + g_panel_height / 2.0f;

		int grid_x = get_coord_from_location(location_x);
		int grid_y = get_coord_from_location(location_y);


		if (m_grid[grid_x][grid_y].type != GOAL || m_grid[grid_x][grid_y].type != START) {
			m_grid[grid_x][grid_y].type = m_left_down ? BLOCKED : EMPTY;
		}
	}
}
void MAIN_PANEL::init(void) {

	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {
			m_grid[i][j].type = EMPTY;
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

	reset();
}

void MAIN_PANEL::reset(void) {
	m_open.clear();
	m_visited.clear();

	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {
			m_grid[i][j].cost = -1.0f;
			m_grid[i][j].h_value = 0.0f;
			m_grid[i][j].g_value = 0.0f;
			m_grid[i][j].path.clear();
		}
	}

	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {
			m_grid[i][j].h_value = (MATH_VECTOR_2D(float(i), float(j)) - MATH_VECTOR_2D(float(m_goal_loc.first), float(m_goal_loc.second))).GetLength();
		}
	}
	m_current_loc = m_start_loc;
}

void ImGuiToggleButton(const char* str_id, bool* v) {


}

void MAIN_PANEL::draw_ui(void) {
	ImGui::Begin("A* Search");
	ImGui::TextWrapped("Start graph traversal and the path search algorithm based on A* Search");

	if (m_start_search) {
		if (ImGui::Button("Pause Search")) {
			m_start_search = false;
		}
	}
	else {
		if (ImGui::Button("Start Search")) {
			m_start_search = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset")) {
		reset();
		m_start_search = false;
	}

	ImGui::Text("Help:");
	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextWrapped("Left/Right Mouse button: Add/Remove wall");

	ImGui::End();
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
	draw_ui();

	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {

			BLOCK_TYPE type = m_grid[i][j].type;

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

			GRAPHICS_UTILITY::draw_rectangle(MATH_VECTOR_2D(get_box_location(i), get_box_location(j)), g_box_size, g_box_size, true, colour);
		}
	}

	for (size_t i = 0; i < m_grid[m_current_loc.first][m_current_loc.second].path.size(); i++) {
		int current_i = m_grid[m_current_loc.first][m_current_loc.second].path[i]->i_value;
		int current_j = m_grid[m_current_loc.first][m_current_loc.second].path[i]->j_value;


		GRAPHICS_UTILITY::draw_rectangle(MATH_VECTOR_2D(get_box_location(current_i), get_box_location(current_j)), g_box_size, g_box_size, true, GRAPHICS_COLOUR::Orange());
	}
}


void MAIN_PANEL::update() {

	if (m_start_search) {

		// did we reach the goal yet
		if (m_current_loc.first != m_goal_loc.first || m_current_loc.second != m_goal_loc.second) {

			// add all neighbouring blocks as open nodes if they are not blocked/visited/opened
			add_neighbours(m_current_loc.first, m_current_loc.second);

			// find the open node with the smallest cost and add it as visited
			find_smallest_open();
		}

	}
}


