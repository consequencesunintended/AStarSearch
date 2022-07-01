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
			grid[i][j].type = EMPTY;
			grid[i][j].cost = -1.0f;
			grid[i][j].i_value = i;
			grid[i][j].j_value = j;
		}
	}
	grid[st_loc.first][st_loc.second].type = START;
	grid[gl_loc.first][gl_loc.second].type = GOAL;

	for (int i = 4; i < 25; i++) {
		grid[20][i].type = BLOCKED;
	}

	for (int i = 4; i < 25; i++) {
		grid[i][25].type = BLOCKED;
	}

	for (int i = 0; i < g_grid_size; i++) {
		for (int j = 0; j < g_grid_size; j++) {
			grid[i][j].h_value = (MATH_VECTOR_2D(i, j) - MATH_VECTOR_2D(gl_loc.first, gl_loc.second)).GetLength();
		}
	}
	current_loc = st_loc;
}

void MAIN_PANEL::reset(void) {
}

void ImGuiToggleButton(const char* str_id, bool* v) {


}

void MAIN_PANEL::draw_ui(void) {

}

bool  MAIN_PANEL::is_visited(int i, int j) {
	for (int k = 0; k < visited.size(); k++) {
		if ((visited[k]->i_value == i) && (visited[k]->j_value == j)) {
			return true;
		}
	}
	return false;
}

bool  MAIN_PANEL::is_open(int i, int j) {
	for (int k = 0; k < open.size(); k++) {
		if ((open[k]->i_value == i) && (open[k]->j_value == j)) {
			return true;
		}
	}
	return false;
}

void  MAIN_PANEL::add_open(int i, int j, float move_distance) {
	if (!is_visited(i, j) && grid[i][j].type != BLOCKED) {

		float cost = current_loc_step + grid[i][j].h_value;

		if (grid[i][j].cost == -1.0f || cost < grid[i][j].cost) {
			grid[i][j].g_value = float(current_loc_step) + move_distance;
			grid[i][j].cost = grid[i][j].g_value + grid[i][j].h_value;

			grid[i][j].path = grid[current_loc.first][current_loc.second].path;
			grid[i][j].path.push_back(&grid[current_loc.first][current_loc.second]);
		}

		if (!is_open(i, j) && !is_visited(i, j)) {
			open.push_back(&grid[i][j]);
		}
	}
}

void  MAIN_PANEL::remove_visited(int i, int j) {

	for (size_t k = 0; k < visited.size(); k++) {
		if (visited[k]->i_value == i && visited[k]->j_value == j) {
			visited.erase(visited.begin() + k);
			break;
		}
	}
}

void  MAIN_PANEL::remove_open(int i, int j) {

	for (size_t k = 0; k < open.size(); k++) {
		if (open[k]->i_value == i && open[k]->j_value == j) {
			open.erase(open.begin() + k);
			break;
		}
	}
}

void MAIN_PANEL::find_smallest_open() {
	float cost = 100000.0;
	GRID_BLOCK* smallest_block = nullptr;

	for (size_t k = 0; k < open.size(); k++) {

		if (open[k]->cost < cost || (smallest_block && open[k]->cost == smallest_block->cost && open[k]->h_value < smallest_block->h_value)) {
			cost = open[k]->cost;
			smallest_block = open[k];
		}
	}
	

	if (smallest_block) {
		current_loc.first = smallest_block->i_value;
		current_loc.second = smallest_block->j_value;
		current_loc_step = smallest_block->g_value;

		visited.push_back(smallest_block);
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

			BLOCK_TYPE type = grid[i][j].type;
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

	for (size_t i = 0; i < grid[current_loc.first][current_loc.second].path.size(); i++) {
		int current_i = grid[current_loc.first][current_loc.second].path[i]->i_value;
		int current_j = grid[current_loc.first][current_loc.second].path[i]->j_value;


		glPushMatrix();
		glTranslatef(-g_grid_size / 2.0f * box_size, -g_grid_size / 2.0f * box_size, 0.0f);

		GRAPHICS_UTILITY::draw_rectangle(MATH_VECTOR_2D(current_i * box_size, current_j * box_size), box_size, box_size, true, GRAPHICS_COLOUR::Orange());
		glPopMatrix();
	}
}


void MAIN_PANEL::update() {
	if (current_loc.first != gl_loc.first || current_loc.second != gl_loc.second) {
		add_neighbours(current_loc.first, current_loc.second);
		find_smallest_open();
	}
}


