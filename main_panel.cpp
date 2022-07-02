#include "main_panel.h"
#include "imgui.h"

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

	m_shift_down =	mods == GLFW_MOD_SHIFT;
	m_ctrl_down	 =	mods == GLFW_MOD_CONTROL;

	float location_x = m_curosr_x - g_panel_width / 2.0f;
	float location_y = -m_curosr_y + g_panel_height / 2.0f;

	int grid_x = get_coord_from_location(location_x);
	int grid_y = get_coord_from_location(location_y);

	if (m_shift_down && m_a_star_search.m_grid[grid_x][grid_y].type != GOAL) {
		m_a_star_search.set_goal_location(grid_x, grid_y);
	}

	if (m_ctrl_down &&  m_a_star_search.m_grid[grid_x][grid_y].type != START) {
		m_a_star_search.set_start_location(grid_x, grid_y);
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


		if ( m_a_star_search.m_grid[grid_x][grid_y].type != GOAL ||  m_a_star_search.m_grid[grid_x][grid_y].type != START) {

			if (!m_shift_down && !m_ctrl_down) {
				 m_a_star_search.m_grid[grid_x][grid_y].type = m_left_down ? BLOCKED : EMPTY;
			}
		}
	}
}
void MAIN_PANEL::init(void) {

	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {
			 m_a_star_search.m_grid[i][j].type = EMPTY;
			 m_a_star_search.m_grid[i][j].i_value = i;
			 m_a_star_search.m_grid[i][j].j_value = j;
		}
	}
	 m_a_star_search.m_grid[m_a_star_search.m_start_loc.first][m_a_star_search.m_start_loc.second].type = START;
	 m_a_star_search.m_grid[m_a_star_search.m_goal_loc.first][m_a_star_search.m_goal_loc.second].type = GOAL;

	for (int i = 4; i < 25; i++) {
		 m_a_star_search.m_grid[20][i].type = BLOCKED;
	}

	for (int i = 4; i < 25; i++) {
		 m_a_star_search.m_grid[i][25].type = BLOCKED;
	}
	m_a_star_search.reset();
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
		m_a_star_search.reset();
		m_start_search = false;
		m_goal_reached = false;
	}

	ImGui::Text("Help:");
	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextWrapped("Left/Right Mouse button: Add/Remove wall");

	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextWrapped("Shift + Mouse button: To Change Start");

	ImGui::BulletText("");
	ImGui::SameLine();
	ImGui::TextWrapped("Ctrl + Mouse button: To Change Goal");

	ImGui::End();
}


void MAIN_PANEL::draw(void) {
	draw_ui();

	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {

			BLOCK_TYPE type = m_a_star_search.m_grid[i][j].type;

			GRAPHICS_COLOUR colour = GRAPHICS_COLOUR::Grey();


			if (type == START) {
				colour = GRAPHICS_COLOUR::Blue();
			}
			else if (type == GOAL) {
				colour = GRAPHICS_COLOUR::Red();
			} else if (type == BLOCKED) {
				colour = GRAPHICS_COLOUR::Black();
			} else if (m_a_star_search.is_visited(i, j)) {
				colour = GRAPHICS_COLOUR::Brown();
			} else if (m_a_star_search.is_open(i, j)) {
				colour = GRAPHICS_COLOUR::Yellow();
			}

			GRAPHICS_UTILITY::draw_rectangle(MATH_VECTOR_2D(get_box_location(i), get_box_location(j)), g_box_size, g_box_size, true, colour);
		}
	}

	MATH_VECTOR_2D start_coord = MATH_VECTOR_2D{ float(m_a_star_search.m_start_loc.first), float(m_a_star_search.m_start_loc.second) };
	MATH_VECTOR_2D start_loc;
	MATH_VECTOR_2D end_loc;

	if (m_goal_reached) {
		for (size_t i = 0; i < m_a_star_search.m_grid[m_a_star_search.m_current_loc.first][m_a_star_search.m_current_loc.second].path.size(); i++) {
			int current_i = m_a_star_search.m_grid[m_a_star_search.m_current_loc.first][m_a_star_search.m_current_loc.second].path[i]->i_value;
			int current_j = m_a_star_search.m_grid[m_a_star_search.m_current_loc.first][m_a_star_search.m_current_loc.second].path[i]->j_value;

			start_loc = MATH_VECTOR_2D(get_box_location(start_coord.X), get_box_location(start_coord.Y));
			end_loc = MATH_VECTOR_2D(get_box_location(current_i), get_box_location(current_j));

			GRAPHICS_UTILITY::draw_line(start_loc, end_loc);

			start_coord.X = current_i;
			start_coord.Y = current_j;
			start_loc = end_loc;
		}
		end_loc = MATH_VECTOR_2D(get_box_location(m_a_star_search.m_goal_loc.first), get_box_location(m_a_star_search.m_goal_loc.second));
		GRAPHICS_UTILITY::draw_line(start_loc, end_loc);
	}

}


void MAIN_PANEL::update() {

	if (m_start_search) {
		m_goal_reached = !m_a_star_search.update_search();
	}
}


